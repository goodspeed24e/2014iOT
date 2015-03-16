#include <libtransport/rpc_session.hpp>

#include <iostream>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

/**
 * RPC Session
 * Design spec:
 * a. running_: true: it means asio's io_service has job(s)
 * b. active_: false: in destroy stage.
 * c. all taskes should return response to user-land caller
 * d. when any error occours, all task request should be canceled.
 **/ 

using namespace std;
using boost::asio::ip::tcp;

#define BUFFER_SIZE 1023

namespace FBi
{

static ServiceEngine service_engine_(4);
static boost::asio::ssl::context ssl_ctx_(boost::asio::ssl::context::sslv3_client);

template<typename T>
T* CreateSessionSock(boost::asio::io_service&, boost::asio::ssl::context* = NULL);

/**
 * @brief Function to create SSL socket.
 *
 * @param[in] io_service ASIO service.
 * @param[in] ssl_context ASIO SSL context.
 */
template<> SSL_SOCK* CreateSessionSock<SSL_SOCK>(boost::asio::io_service& io_service, boost::asio::ssl::context* ssl_context)
{
#ifdef DEBUG_2
        printf("SSL CreateSock\n");
#endif
        if(ssl_context)
                return new SSL_SOCK(io_service, *ssl_context);
        else
                return NULL;
}

/**
 * @brief Function to create TCP socket.
 *
 * @param[in] io_service ASIO service.
 * @param[in] ssl_context ASIO SSL context (We can pass NULL here. Because we don't use it when creating the TCP socket).
 */
template<> TCP_SOCK* CreateSessionSock(boost::asio::io_service& io_service, boost::asio::ssl::context* /*ssl_context*/)
{
#ifdef DEBUG_2
        printf("TCP CreateSock\n");
#endif
        return new TCP_SOCK(io_service);
}

template<typename T>
void ShutdownSession(T*);

/**
 * @brief Function to shutdown the SSL socket.
 *
 * @param[in] sock The SSL socket.
 */
template<> void ShutdownSession<SSL_SOCK>(SSL_SOCK* sock)
{
	if (!sock->lowest_layer().is_open())
		return;

	boost::system::error_code error1;
//	sock->shutdown(error1);
	sock->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
	sock->lowest_layer().close(error1);
}

/**
 * @brief Function to shutdown the TCP socket.
 *
 * @param[in] sock The TCP socket.
 */
template<> void ShutdownSession<TCP_SOCK>(TCP_SOCK* sock)
{
	if (!sock->is_open())
		return;

	boost::system::error_code error1;
	sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
	sock->close(error1);
}

template<typename T>
void HandleStage(RPCSession<T>*);

template<> void HandleStage<SSL_SOCK>(RPCSession<SSL_SOCK>* session)
{
	session->sock_->async_handshake(boost::asio::ssl::stream_base::client,
		make_custom_alloc_handler(session->allocator_,
		boost::bind(&RPCSession<SSL_SOCK>::handle_ssl_handshake,
		session, boost::asio::placeholders::error)));
}

template<> void HandleStage<TCP_SOCK>(RPCSession<TCP_SOCK>* session)
{
	if (session->timeout_)
		session->timer_.cancel();
	session->do_request();
}

template<typename T>
void RPCSession<T>::SetTimeoutValue(unsigned int v)
{
	timeout_ = v;
}

template<typename T>
RPCSession<T>::RPCSession(const std::string& host, int port, bool keep_alive, bool compression)
	: service_(service_engine_.GetIOService()),
	  sock_(CreateSessionSock<T>(service_, &ssl_ctx_)), timer_(service_),
	  remote_host_(host), pos_(NULL), timeout_(0),
	  active_(true), running_(false), keep_alive_(keep_alive), compression_(compression)
{
	//m_callback_queue = new std::deque<std::pair<MethodCallPtr, RPCSessionFuncCallback> >();

	char port_ptr[6];
	sprintf(port_ptr, "%d",port);
	remote_port_.assign(port_ptr);
	m_BlockingFunction  = boost::bind(&RPCSession::blocking_callback, this, _1);
	service_engine_.StartService();
}

template<typename T>
RPCSession<T>::~RPCSession()
{
	{
		boost::mutex::scoped_lock lock(mutex_);
		active_ = false;

		timer_.cancel();

		if (sock_->lowest_layer().is_open())
			sock_->lowest_layer().cancel(); 		 

		std::map<boost::asio::deadline_timer*, boost::shared_ptr<boost::asio::deadline_timer> >::iterator itor =
			delay_timer_map_.begin();
		for(; itor != delay_timer_map_.end(); ++itor)
			itor->second->cancel();

	}

	//make sure the session has no active task.
	while(running_ || !m_blocking_queue.empty() || !delay_timer_map_.empty())
	{
#ifdef WIN32
		::Sleep(100);
#else
		::usleep(100000);
#endif
		boost::mutex::scoped_lock lock(mutex_); 		 

		if (sock_->lowest_layer().is_open()) 		 
			sock_->lowest_layer().cancel();
	}

	ShutdownSession(sock_);

	std::map<boost::condition_variable*, MethodResponsePtr>::iterator itor = response_map_.begin();
	for(; itor != response_map_.end(); ++itor)
	{
		delete itor->first;
	}

	delete sock_;
}

template<typename T>
MethodResponsePtr  RPCSession<T>::Call(const MethodCallPtr& call)
{
	boost::mutex::scoped_lock lock(mutex_);
	MethodResponsePtr resp;
	if (!active_)
		return resp;

	boost::condition_variable wait_cond;
	m_callback_queue.push_back(std::make_pair(call, m_BlockingFunction));
	m_blocking_queue.push_back(std::make_pair(&wait_cond, resp));

	do_async_call();

	wait_cond.wait(lock);
	resp = m_blocking_queue.front().second;
	m_blocking_queue.pop_front();
	return resp;
}

// return NULL: operation failed
// return Non-NULL: operation successful
template<typename T>
void* RPCSession<T>::Call(const MethodCallPtr& call, bool getResp)
{
	boost::mutex::scoped_lock lock(mutex_);
	if (!active_)
		return NULL;

	boost::condition_variable* wait_cond = new boost::condition_variable;
	MethodResponsePtr resp(new MethodResponse);

	m_callback_queue.push_back(std::make_pair(call, m_BlockingFunction));
	m_blocking_queue.push_back(std::make_pair(wait_cond, resp));

	do_async_call();

	if(getResp)
	{
		resp.reset();
		response_map_.insert(make_pair(wait_cond, resp));
	}

	return wait_cond;
}

template<typename T>
MethodResponsePtr RPCSession<T>::GetResponse(void* ticket)
{
	
	std::map<boost::condition_variable*, MethodResponsePtr>::iterator itor;
	MethodResponsePtr resp;
	for(;;)
	{
		boost::mutex::scoped_lock lock(mutex_);
		itor = response_map_.find((boost::condition_variable*)ticket);
		if(itor == response_map_.end())
			return resp;

		if (itor->second.get())
		{
			resp = itor->second;
			delete itor->first;
			response_map_.erase(itor);
			return resp;
		}
		itor->first->wait(lock);
	}
}

template<typename T>
void RPCSession<T>::DelayCall(const MethodCallPtr& call, const RPCSessionFuncCallback& callback, int delay)
{
	boost::mutex::scoped_lock lock(mutex_);
	if (!active_)
		return;

	if (delay <= 0)
	{
		m_callback_queue.push_back(std::make_pair(call, callback));

		do_async_call();
		return;
	}

	boost::shared_ptr<boost::asio::deadline_timer> delay_timer(new boost::asio::deadline_timer(service_));
	delay_timer_map_.insert(make_pair(delay_timer.get(), delay_timer));

	delay_timer->expires_from_now(boost::posix_time::seconds(delay));
	delay_timer->async_wait(boost::bind(&RPCSession::delay_call, this, boost::asio::placeholders::error,
				call, callback, delay_timer));
}

template<typename T>
void RPCSession<T>::delay_call(const boost::system::error_code& err, const MethodCallPtr& call, 
							   const RPCSessionFuncCallback& callback,const
							   boost::shared_ptr<boost::asio::deadline_timer>& delay_timer)
{
	boost::mutex::scoped_lock lock(mutex_);

	std::map<boost::asio::deadline_timer*, boost::shared_ptr<boost::asio::deadline_timer> >::iterator itor =
		delay_timer_map_.find(delay_timer.get());
	if (itor != delay_timer_map_.end())
		delay_timer_map_.erase(itor);

	if (err || !active_)
	{
		MethodResponsePtr failed_respnse;
		callback(failed_respnse);
		return;
	}

	m_callback_queue.push_back(std::make_pair(call, callback));

	do_async_call();
}


template<typename T>
RPCSessionFuncCallback* RPCSession<T>::Call(const MethodCallPtr& call, const RPCSessionFuncCallback& callback)
{
	boost::mutex::scoped_lock lock(mutex_);
	if (!active_)
		return NULL;

	m_callback_queue.push_back(std::make_pair(call, callback));

	do_async_call();

	return &(m_callback_queue.back().second);
}

template<typename T>
void RPCSession<T>::do_async_call()
{
	if(!running_)
	{
		running_ = true;
		service_.post(boost::bind(&RPCSession::async_call, this));
	}
}

template<typename T>
void RPCSession<T>::async_call()
{
	if(check_connect())
		do_request();
}

template<typename T>
void RPCSession<T>::blocking_callback(const MethodResponsePtr& resp)
{
	boost::mutex::scoped_lock lock(mutex_);
	if (m_blocking_queue.front().second.get()) //ticket call.
	{
		std::map<boost::condition_variable*, MethodResponsePtr>::iterator itor
			= response_map_.find(m_blocking_queue.front().first);
		if (itor != response_map_.end())
		{
			//conditional variable will be deleted at GetResponse();
			itor->second = resp;
			itor->first->notify_all();
		}
		else
		{
			//don't care response.
			delete m_blocking_queue.front().first;
		}

		m_blocking_queue.pop_front();
	}
	else //blocking call
	{
		m_blocking_queue.front().second = resp;
		m_blocking_queue.front().first->notify_all();
	}
}

template<typename T>
bool RPCSession<T>::check_connect()
{
	bool is_error_occur = true;
	boost::system::error_code err_code;
	{
		boost::mutex::scoped_lock lock(mutex_);

		if(active_)
		{
			if(sock_->lowest_layer().is_open())
				return true;

			boost::asio::ip::tcp::resolver resolver_handle(service_);
			tcp::resolver::query query(remote_host_, remote_port_);
			try
			{
				tcp::resolver::iterator endpoint_iterator = resolver_handle.resolve(query);
				tcp::endpoint endpoint = *endpoint_iterator;
				sock_->lowest_layer().async_connect(endpoint,
					make_custom_alloc_handler(allocator_,
					boost::bind(&RPCSession::handle_connect, this,
					boost::asio::placeholders::error, ++endpoint_iterator)));

				if (timeout_)
				{
					timer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
					timer_.async_wait(boost::bind(&RPCSession::handle_timeout, this, boost::asio::placeholders::error));
				}
				is_error_occur = false;
			}
			catch(const boost::system::system_error& err)
			{
				err_code = err.code();
			}
		}
	}

	if(is_error_occur)
		handle_error(err_code); //error handling.

	return false;
}

template<typename T>
void RPCSession<T>::handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err && active_)
	{
		//set socket option.
		boost::system::error_code ec;
		sock_->lowest_layer().set_option(boost::asio::socket_base::keep_alive(true), ec);
		sock_->lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true), ec);
		
		//perform SSL handshaking
		//sock_->async_handshake(boost::asio::ssl::stream_base::client,
		//	boost::bind(&RPCSession::handle_ssl_handshake,
		//	this, boost::asio::placeholders::error));
		if (!ec)
		{
			HandleStage(this);
			return;
		}
	}
	else if (endpoint_iterator != tcp::resolver::iterator() && active_)
	{
		//The connection failed. Try the next endponit in the list.
		sock_->lowest_layer().close();
		tcp::endpoint endpoint = *endpoint_iterator;
		sock_->lowest_layer().async_connect(endpoint,
			make_custom_alloc_handler(allocator_,
			boost::bind(&RPCSession::handle_connect, this,
				boost::asio::placeholders::error, ++endpoint_iterator)));
		return;
	}

	if (timeout_)
		timer_.cancel();
	handle_error(err);
}

template<typename T>
void RPCSession<T>::handle_ssl_handshake(const boost::system::error_code& err)
{
	if (timeout_)
		timer_.cancel();
	if (!err && active_)
	{
		do_request();
	}
	else
	{
		printf("ggggggggggggggggggggggggg\n");
		handle_error(err);
	}
}

template<typename T>
void RPCSession<T>::do_request()
{
	{
		assert(!m_callback_queue.empty());
		call_buf_ = m_callback_queue.front().first->GetXML();
	}

	
	int len = sprintf(buffer_,
		"POST /RPC2 HTTP/1.1\r\nUser-Agent: FBi-Client\r\nHost: %s\r\n",
		remote_host_.c_str());

	if (keep_alive_)
		len += sprintf(buffer_ + len, "Connection: keep-alive\r\n");
	else
		len += sprintf(buffer_ + len, "Connection: close\r\n");

	if(compression_)
	{
		// We accept the body in response message with gzip encoding.
		len += sprintf(buffer_ + len, "Accept-Encoding: gzip\r\n");

		// Compress the body of request message.
		try
		{
			if(call_buf_.length() > 0)
			{
				string temp_str;
				boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
				out.push(boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_compression));
				out.push(boost::iostreams::back_inserter(temp_str));
				boost::iostreams::copy(boost::make_iterator_range(call_buf_), out);
				call_buf_ = temp_str;

				// We encode the body of request message with gzip.
				len += sprintf(buffer_ + len, "Content-Encoding: gzip\r\n");
			}
		}
		catch(BOOST_IOSTREAMS_FAILURE err)
		{
			printf("Can not compress the body of request message.\n");
		}
	}


	len += sprintf(buffer_ + len, "Content-Type: text/xml\r\nContent-Length: %u\r\n\r\n",
		(unsigned int)call_buf_.length());
	/*
	std::ostream request_stream(&request_);
	request_stream << "POST /RPC2 HTTP/1.1\r\n";
	request_stream << "User-Agent: FBi-Client\r\n";
	request_stream << "Host: " << remote_host_ << "\r\n";
	
	if (keep_alive_)
		request_stream << "Connection: keep-alive\r\n";
	else
		request_stream << "Connection: close\r\n";

	request_stream << "Content-Type: text/xml\r\n";
	request_stream << "Content-Length: " << call_buf_.length() << "\r\n\r\n";
	*/

	std::vector<boost::asio::const_buffer> request_buffers;
	//request_buffers.push_back(boost::asio::buffer(request_.data(), request_.size()));
	request_buffers.push_back(boost::asio::buffer(buffer_, len));
	request_buffers.push_back(boost::asio::buffer(call_buf_.c_str(), call_buf_.length()));

	//initialize buffer variables for each request;
	buffer_len_ = 0;
	body_len_ = 0;
	
	boost::asio::async_write(*sock_, request_buffers,
		make_custom_alloc_handler(allocator_,
		boost::bind(&RPCSession::read_http_response, this,
			boost::asio::placeholders::error)));
}

template<typename T>
void RPCSession<T>::read_http_response(const boost::system::error_code& err)
{
	//request_.consume(request_.size());
	if (!err && active_)
	{
		if (timeout_)
		{
			timer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
			timer_.async_wait(boost::bind(&RPCSession::handle_timeout, this, boost::asio::placeholders::error));
		}
		//read the response header
		assert(buffer_len_ < 1024);
		sock_->async_read_some(boost::asio::buffer(buffer_ + buffer_len_,
					BUFFER_SIZE - buffer_len_),
				make_custom_alloc_handler(allocator_,
				boost::bind(&RPCSession::handle_read_status_line, this, 		 
					boost::asio::placeholders::error, 		 
					boost::asio::placeholders::bytes_transferred)));
	}
	else
	{
		handle_error(err);
	}
}

template<typename T>
void RPCSession<T>::handle_read_status_line(const boost::system::error_code& err, std::size_t bytes)
{
	if (timeout_)
		timer_.cancel();

	if (!err && active_)
	{
		buffer_len_ += bytes;
		if (buffer_len_ < 17)
		{
			read_http_response(err);
			return;
		}
		//expect: HTTP/1.x 200 OK\r\n (length: 17)
		buffer_[15] = 0;
		pos_ = strstr(buffer_, "200 OK");
		if (pos_ == NULL)
		{
			handle_error(err);
			return;
		}

		pos_ = buffer_ + 16;
		parse_header(err, 0);
	}
	else
	{
		handle_error(err);
	}
}

template<typename T>
void RPCSession<T>::parse_header(const boost::system::error_code& err, std::size_t bytes)
{
	if (timeout_)
		timer_.cancel();

	if (err)
	{
		handle_error(err);
		return;
	}

	buffer_len_ += bytes;

	char* line_end;
	char* keyword;
	bool handle_body = false;
	size_t line_len;
	bool parse_connection = true;
	is_body_encoded_ = false;

	for(;;)
	{
		line_end = strchr(pos_, '\n');
		if (!line_end)
			break;

		
		*line_end = '\0';
		line_len = line_end - pos_;
		//printf("line len: %d %d\n", line_len, line_end - buffer_);
		//printf("->%s\n", pos_);

		pos_ = line_end + 1;

		if (line_len == 1)
		{
			handle_body = true;
			break;
		}
		if (static_cast<size_t>(pos_ - buffer_) >= buffer_len_)
			break;
		if (!body_len_ && (keyword = strstr(pos_, "ength")))
		{
			//Content-Length:
			body_len_ = atoi(keyword + 7);
			
		}
		else if (parse_connection && (keyword = strstr(pos_, "ection")))
		{
			parse_connection = false;
			//Connection:
			if (strstr(keyword + 6, "eep-") == NULL) 
			{
				//keep-alive string
				boost::system::error_code ec;
				sock_->lowest_layer().close(ec);
			}
		}
		else if((!is_body_encoded_) && (keyword = strstr(pos_, "ncoding")))
		{
			// Is there any encoding in the body of response message?
			if (strstr(keyword + 7, "gzip"))
				is_body_encoded_ = true;
		}
	}

	if (handle_body)
	{
		assert(body_len_ > 0);
		pos_ = line_end + 1;
		buffer_len_ -= pos_ - buffer_;

		method_resp_.reset(new MethodResponse());
		boost::system::error_code err; 
		parse_response_tree(err);      
	}
	else
	{
		assert(buffer_len_ < 1024);
		sock_->async_read_some(boost::asio::buffer(buffer_ + buffer_len_, BUFFER_SIZE - buffer_len_),
			make_custom_alloc_handler(allocator_,
			boost::bind(&RPCSession::parse_header, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));

		if (timeout_)
		{
			timer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
			timer_.async_wait(boost::bind(&RPCSession::handle_timeout, this, boost::asio::placeholders::error));
		}
	}
}

template<typename T>
bool RPCSession<T>::check_request()
{
	return (running_ = !m_callback_queue.empty());
}

template<typename T>
void RPCSession<T>::parse_response_tree(const boost::system::error_code& err)
{
	if (timeout_)
		timer_.cancel();

	if(err || !active_)
	{
		handle_error(err);
		return;
	}

	try
	{
		if (buffer_len_ > 0)
		{
			pos_[buffer_len_] = '\0';
			if(is_body_encoded_)
			{
				// Collect the encoded body of response message.
				encoded_data_.append(pos_, buffer_len_);
			}
			else
			{
				method_resp_->Parse(pos_);
			}
			body_len_ -= buffer_len_;
		}

		if (body_len_ > 0)
		{
			buffer_len_ = (body_len_ > BUFFER_SIZE) ? BUFFER_SIZE : body_len_;
			pos_ = buffer_;
			boost::asio::async_read(*sock_, boost::asio::buffer(buffer_, buffer_len_),
				make_custom_alloc_handler(allocator_,
				boost::bind(&RPCSession::parse_response_tree, this,
				boost::asio::placeholders::error)));

			if (timeout_)
			{
				timer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
				timer_.async_wait(boost::bind(&RPCSession::handle_timeout, this, boost::asio::placeholders::error));
			}
			return;
		}
		// Decompress the encoded body of response message.
		if(is_body_encoded_)
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::gzip_decompressor());
			in.push(boost::make_iterator_range(encoded_data_));
			std::string decompressed;
			boost::iostreams::copy(in, boost::iostreams::back_inserter(decompressed));
			method_resp_->Parse(decompressed.c_str());
			encoded_data_.clear();
		}

		method_resp_->ComposeTree();

		RPCSessionFuncCallback callback;
		{
			boost::mutex::scoped_lock lock(mutex_);
			callback = m_callback_queue.front().second;
			m_callback_queue.pop_front();
		}

		if (callback)
			callback(method_resp_);

		{
			boost::mutex::scoped_lock lock(mutex_);

			if (!keep_alive_)
				ShutdownSession(sock_);

			if(!check_request())
				return;
		}

		if(check_connect())
			do_request();

	}
	catch(const boost::system::system_error& err)
	{
		//n = response_.size();
		//response_.consume(n);

		handle_error(err.code());
	}
	catch(BOOST_IOSTREAMS_FAILURE err)
	{
		printf("Can not decompress the body of response message.\n");
		const boost::system::error_code ec;
		handle_error(ec);
	}
	catch(...)
	{
		const boost::system::error_code ec;
		handle_error(ec);
	}
}

template<typename T>
void RPCSession<T>::handle_timeout(const boost::system::error_code& err)
{
	if (err == boost::asio::error::operation_aborted)
                return;

	boost::mutex::scoped_lock lock(mutex_);

	if(!sock_->lowest_layer().is_open())
		return;

	boost::system::error_code ec;
	sock_->lowest_layer().cancel(ec);
}

template<typename T>
void RPCSession<T>::handle_error(const boost::system::error_code& /*err*/)
{
	printf("RPCSession<T>::handle_error: %s \n", remote_host_.c_str());

	std::list<std::pair<MethodCallPtr, RPCSessionFuncCallback> > temp_callback_queue;
	RPCSessionFuncCallback callback;
	{
		boost::mutex::scoped_lock lock(mutex_);
		temp_callback_queue.splice(temp_callback_queue.end(), m_callback_queue);

		method_resp_.reset();
	}

	if (!temp_callback_queue.empty())
	{
		std::list<std::pair<MethodCallPtr, RPCSessionFuncCallback> >::iterator iter = temp_callback_queue.begin();
		MethodResponsePtr failed_respnse;
		for(; iter != temp_callback_queue.end(); ++iter)
		{
			iter->second(failed_respnse);
		}
	}

	{
		boost::mutex::scoped_lock lock(mutex_);

		if (sock_->lowest_layer().is_open())
		{
			boost::system::error_code ec;
			sock_->lowest_layer().close(ec);
		}

		delete sock_;
		sock_ = CreateSessionSock<T>(service_, &ssl_ctx_);

		if(!check_request())
			return;
	}

	if(check_connect())
		do_request();

}

//////////////////////////////////////////
NotifySession::NotifySession(const std::string& host, int port, const NotifySessionCallback& callback)
	: service_(service_engine_.GetIOService()),
	  sock_(new SSL_SOCK(service_, ssl_ctx_)),
	  callback_(callback),
	  remote_host_(host), pos_(NULL),
	  active_(false), running_(false)
{
	char port_ptr[6];
	sprintf(port_ptr, "%d",port);
	remote_port_.assign(port_ptr);
	service_engine_.StartService();
}
	
NotifySession::~NotifySession()
{
	this->Stop();
	delete sock_;
}

void NotifySession::Start(const string& path)
{
	{
		boost::mutex::scoped_lock lock(mutex_);
		if (active_)
			return;

		active_ = true;
		path_ = path;
	}

	start_connect();
}

void NotifySession::Stop()
{
	{
		boost::mutex::scoped_lock lock(mutex_);
		active_ = false;

		if (sock_->lowest_layer().is_open())
			sock_->lowest_layer().cancel();
	}

	while(running_)
	{
#ifdef WIN32
		::Sleep(100);
#else
		::usleep(100000);
#endif
		boost::mutex::scoped_lock lock(mutex_); 		 

		if (sock_->lowest_layer().is_open()) 		 
			sock_->lowest_layer().cancel();
	}

	ShutdownSession(sock_);
}

void NotifySession::start_connect()
{
	bool is_error_occur = false;
	boost::system::error_code err_code;

	{
		boost::mutex::scoped_lock lock(mutex_);
		if(!active_)
		{
			running_ = false;
			return;
		}	

		if(sock_->lowest_layer().is_open())
			return;

		running_ = true;
		boost::asio::ip::tcp::resolver resolver_handle(service_);
		tcp::resolver::query query(remote_host_, remote_port_);
		try
		{
			tcp::resolver::iterator endpoint_iterator = resolver_handle.resolve(query);
			tcp::endpoint endpoint = *endpoint_iterator;
			sock_->lowest_layer().async_connect(endpoint,
				make_custom_alloc_handler(allocator_,
				boost::bind(&NotifySession::handle_connect, this,
				boost::asio::placeholders::error, ++endpoint_iterator)));
		}
		catch(const boost::system::system_error& err)
		{
			is_error_occur = true;
			err_code = err.code();
		}
	}

	if(is_error_occur)
		handle_error(err_code); //error handling.
}

void NotifySession::handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err && active_)
	{
		//set socket option.
		boost::system::error_code ec;
		sock_->lowest_layer().set_option(boost::asio::socket_base::keep_alive(true), ec);
		sock_->lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true), ec);
		
		if (!ec)
		{
			sock_->async_handshake(boost::asio::ssl::stream_base::client,
				make_custom_alloc_handler(allocator_,
				boost::bind(&NotifySession::handle_ssl_handshake,
					this, boost::asio::placeholders::error)));
		}
		else
			handle_error(ec);
	}
	else if (endpoint_iterator != tcp::resolver::iterator() && active_)
	{
		//The connection failed. Try the next endponit in the list.
		sock_->lowest_layer().close();
		tcp::endpoint endpoint = *endpoint_iterator;
		sock_->lowest_layer().async_connect(endpoint,
			make_custom_alloc_handler(allocator_,
			boost::bind(&NotifySession::handle_connect, this,
				boost::asio::placeholders::error, ++endpoint_iterator)));
	}
	else
	{
		handle_error(err);
	}
}

void NotifySession::handle_ssl_handshake(const boost::system::error_code& err)
{
	if (!err && active_)
	{
		int len = sprintf(buffer_,
			"GET %s HTTP/1.1\r\nUser-Agent: FBi-Client\r\nHost: %s\r\n",
			path_.c_str(), remote_host_.c_str());
		len += sprintf(buffer_ + len, "Connection: keep-alive\r\n");		
		len += sprintf(buffer_ + len, "Content-Length: 0\r\n\r\n");
	
		//initialize buffer variables for each request;
		buffer_len_ = 0;
		body_len_ = 0;
		
		boost::asio::async_write(*sock_, boost::asio::buffer(buffer_, len),
			make_custom_alloc_handler(allocator_,
			boost::bind(&NotifySession::read_http_response, this,
				boost::asio::placeholders::error)));
	}
	else
	{
		handle_error(err);
	}
}

void NotifySession::read_http_response(const boost::system::error_code& err)
{
	if (!err && active_)
	{
		//read the response header
		assert(buffer_len_ < 1024);
		sock_->async_read_some(boost::asio::buffer(buffer_ + buffer_len_,
				BUFFER_SIZE - buffer_len_),
			make_custom_alloc_handler(allocator_,
			boost::bind(&NotifySession::handle_read_status_line, this,
				boost::asio::placeholders::error, 		 
				boost::asio::placeholders::bytes_transferred)));
	}
	else
	{
		handle_error(err);
	}
}

void NotifySession::handle_read_status_line(const boost::system::error_code& err, std::size_t bytes)
{
	if (!err && active_)
	{
		buffer_len_ += bytes;
		if (buffer_len_ < 17)
		{
			read_http_response(err);
			return;
		}
		//expect: HTTP/1.x 200 OK\r\n (length: 17)
		buffer_[15] = 0;
		pos_ = strstr(buffer_, "200 OK");
		if (pos_ == NULL)
		{
			handle_error(err);
			return;
		}

		pos_ = buffer_ + 16;
		parse_header(err, 0);
	}
	else
	{
		handle_error(err);
	}
}

void NotifySession::parse_header(const boost::system::error_code& err, std::size_t bytes)
{
	if (err || !active_)
	{
		handle_error(err);
		return;
	}

	buffer_len_ += bytes;

	char* line_end;
	char* keyword;
	bool handle_body = false;
	size_t line_len;

	for(;;)
	{
		line_end = strchr(pos_, '\n');
		if (!line_end)
			break;
		
		*line_end = '\0';
		line_len = line_end - pos_;
		//printf("line len: %d %d\n", line_len, line_end - buffer_);
		//printf("xx->%s\n", pos_);
		
		if (line_len == 1)
		{
			if (body_len_)
			{
				handle_body = true;
				break;
			}
		}
		else if (static_cast<size_t>(pos_ - buffer_) >= buffer_len_)
			break;
		else if (!body_len_ && (keyword = strstr(pos_, "ength")))
		{
			body_len_ = atoi(keyword + 7);
		}

		pos_ = line_end + 1;
	}

	if (handle_body)
	{
		assert(body_len_ > 0);
		pos_ = line_end + 1;
		buffer_len_ -= pos_ - buffer_;

		this->handle_body(err);
	}
	else
	{
		assert(buffer_len_ < 1024);
		buffer_len_ = 0;
		pos_ = buffer_;
		sock_->async_read_some(boost::asio::buffer(buffer_ + buffer_len_, BUFFER_SIZE - buffer_len_),
			make_custom_alloc_handler(allocator_,
			boost::bind(&NotifySession::parse_header, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
	}
}

void NotifySession::handle_body(const boost::system::error_code& err)
{
	if (err || !active_)
	{
		handle_error(err);
		return;
	}

	try
	{
		if (buffer_len_ > 0)
		{
			if (buffer_len_ > body_len_)
			{
				pos_[body_len_] = '\0';
				buffer_len_ -= body_len_;
				resp_data_.append(pos_);
				pos_ += body_len_ + 1;
				body_len_ = 0;
			}
			else
			{
				pos_[buffer_len_] = '\0';
				body_len_ -= buffer_len_;
				buffer_len_ = 0;
				resp_data_.append(pos_);
				pos_ = buffer_;
			}
		}

		if (body_len_ > 0)
		{
			buffer_len_ = (body_len_ > BUFFER_SIZE) ? BUFFER_SIZE : body_len_;
			pos_ = buffer_;

			boost::asio::async_read(*sock_, boost::asio::buffer(buffer_, buffer_len_),
				make_custom_alloc_handler(allocator_,
				boost::bind(&NotifySession::handle_body, this,
				boost::asio::placeholders::error)));
			return;
		}

		if (callback_ && !resp_data_.empty())
			callback_(resp_data_.c_str(), resp_data_.length());

		resp_data_.clear();
		body_len_ = 0;
		parse_header(err, 0);
	}
	catch(const boost::system::system_error& err)
	{
		handle_error(err.code());
	}
	catch(...)
	{
		const boost::system::error_code ec;
		handle_error(ec);
	}
}

void NotifySession::handle_error(const boost::system::error_code& /*err*/)
{
	//printf("NotifySession::handle_error: %s \n", err.message().c_str());

	// Important: We can not call NotifySession::Stop function in the callback function because we have not set the running_ flag to false.
	// If we call it, the callback function will not return to this function.
	// We also set the running_ flag to false in the last of this function.
	// Otherwise, it may use destroyed object in this function when we use NotifySession in the multi-thread environment.
	if (callback_)
	{
		callback_(NULL, 0);
	}

	boost::mutex::scoped_lock lock(mutex_);

	if (sock_->lowest_layer().is_open())
	{
		boost::system::error_code ec;
		sock_->lowest_layer().close(ec);
	}

	delete sock_;
	sock_ = new SSL_SOCK(service_, ssl_ctx_);

	running_ = false;
}

/////////////////////////////////////////
template class RPCSession<SSL_SOCK>;

template class RPCSession<TCP_SOCK>;

}
