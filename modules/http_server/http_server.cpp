#include <http_server/http_server.hpp>

namespace FBi
{
/// The service engine with ASIO.
static ServiceEngine service_engine_;
/// The SSL context.
static boost::asio::ssl::context ssl_context_(boost::asio::ssl::context::sslv23);

/**
 * @brief Constructor for HTTP server.
 *
 * @param[in] request_dispatcher The HTTP request message dispatcher.
 * @param[in] address IP address for server to listen. When it is empty string, it means any address.
 * @param[in] port Port for server to listen.
 * @param[in] sock_mode The mode of socket. It can be HttpServer::SSL or HttpServer::TCP.
 * @param[in] ssl_context_info The information about the SSL context.
 * @param[in] max_connection The maximum connection of server.
 */
HttpServer::HttpServer(const boost::shared_ptr<IHttpRequestDispatcher>& request_dispatcher, const std::string& address, int port, const SSLContextInfo& ssl_context_info, int max_connection)
	: http_request_dispatcher_(request_dispatcher),
	acceptor_(service_engine_.GetIOService()),
	max_connection_num_(max_connection),
	is_started_(false),
	is_processing_start_server_(false),
	connection_counter_(0)
{
	set_ssl_context_info(ssl_context_info, ssl_context_);

	// Create new connection.
	new_connection_.reset(this->create_connection());

	// Set the endpoint with the address and port.
	char port_buf[33] = {'\0'};
	std::string port_str;
#ifdef WIN32
	_snprintf(port_buf, 32, "%d", port);
#else
	snprintf(port_buf, 32, "%d", port);
#endif
	port_str = port_buf;

	if(!address.empty())
	{
		boost::asio::ip::tcp::resolver resolver(acceptor_.get_io_service());
		boost::asio::ip::tcp::resolver::query query(address, port_str);
		current_endpoint_ = *resolver.resolve(query);
	}
	else
	{
		current_endpoint_ = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	}

	// Create callback for acceptor.
	accept_callback_ = boost::bind(&HttpServer::handle_accept, this, boost::asio::placeholders::error);
}

HttpServer::~HttpServer()
{
	// For safety, we clear it again. To prevent the WaitToStop function does not be called.
	this->StopServer();

	while(!thread_id_map_.empty())
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));

#ifdef DEBUG_2
	printf("~HttpServer() exit\n");
#endif
}

/// Function to start the HTTP server and Service Engine.
void HttpServer::StartServer()
{
	boost::mutex::scoped_lock lock(mutex_);
	if(is_started_ || is_processing_start_server_)
		return;

	is_processing_start_server_ = true;

	// Wait the previous StopServer to reclaim thread and resource.
	lock.unlock();
	while(thread_id_map_.size() > 0)
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	lock.lock();

	// Bind the address and port (endpoint).
	acceptor_.open(current_endpoint_.protocol());
	acceptor_.set_option(boost::asio::socket_base::enable_connection_aborted(true));
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(current_endpoint_);

	// Start listen.
	acceptor_.listen(max_connection_num_);

	is_started_ = true;
	// Prepare to accept the connection.
	if(new_connection_)
		acceptor_.async_accept(new_connection_->GetTCPSocket(),
			make_custom_alloc_handler(allocator_,
				accept_callback_));
	else
		printf("[HTTP server]: Connection is NULL.\n");

	service_engine_.StartService();
	is_processing_start_server_ = false;
}

/// Function to stop the HTTP server.
void HttpServer::StopServer()
{
	boost::mutex::scoped_lock lock(mutex_);
	if(!is_started_)
		return;

	acceptor_.close();

	ThreadIdMapIter iter = thread_id_map_.begin();
	for(; iter != thread_id_map_.end(); iter++)
		if(iter->second.connection)
			iter->second.connection->StopConnection();

	while(is_started_)
	{
#ifdef WIN32
		::Sleep(100);
#else
		::usleep(100000);
#endif
	}
#ifdef DEBUG_2
	printf("Stop Server exit\n");
#endif
}

/**
 * @brief Function to register the callback function for the max connection number fit the limitation.
 *
 * @param[in] callback The callback function to be registered.
 */ 
void HttpServer::RegisterMeetMaxConnectionCallback(const MeetMaxConnectionNumCallback& callback)
{
	meet_max_connection_callback_ = callback;
}

/**
 * @brief Function to set the SSL context according to the information of SSL context.
 *
 * @param[in] info The information about the SSL context.
 * @param[out] ssl_context The SSL context for ASIO.
 */ 
void HttpServer::set_ssl_context_info(const SSLContextInfo& info, boost::asio::ssl::context& ssl_context)
{
	ssl_context.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::single_dh_use);

	ssl_private_key_password_ = info.private_key_password;

	if(info.is_use_private_key_password_in_code)
	{
		// We must set callback of password before we set other context of SSL.
		// If we do not register the callback, it will require us to enter password when we execute the server.
		ssl_context.set_password_callback(boost::bind(&HttpServer::get_ssl_private_key_password, this));
	}

	if(info.rsa_private_key_file.length() > 0)
		ssl_context.use_rsa_private_key_file(info.rsa_private_key_file, boost::asio::ssl::context::pem);

	if(info.private_key_file.length() > 0)
		ssl_context.use_private_key_file(info.private_key_file, boost::asio::ssl::context::pem);

	if(info.certificate_file.length() > 0)
	{
		ssl_context.use_certificate_chain_file(info.certificate_file);
		//ssl_context.use_certificate_file(info.certificate_file, boost::asio::ssl::context::pem);
	}

	if(info.is_use_default_verify_paths)
	{
		boost::system::error_code error;
		ssl_context.set_default_verify_paths(error);
		if(error)
			printf("In set_ssl_context_info, Error: %s\n", error.message().c_str());
	}

	if(info.certificate_authority_file.length() > 0)
		ssl_context.load_verify_file(info.certificate_authority_file);

	if(info.ssl_dh_file.length() > 0)
		ssl_context.use_tmp_dh_file(info.ssl_dh_file);
}

/// The callback function for the NetworkConnection. When the thread exit, this function will be called.
void HttpServer::thread_exit_callback_func()
{
#ifdef DEBUG_2
	printf("Enter thread_exit_callback_func()\n");
#endif
	boost::mutex::scoped_lock lock(mutex_);

	// Find the thread in the map
	ThreadIdMapIter iter = thread_id_map_.find(boost::this_thread::get_id());
	assert(iter != thread_id_map_.end());

	// Detach thread.
	if(iter->second.thread)
		iter->second.thread->detach();

	// Erase the information about the thread and connection in the map.
	thread_id_map_.erase(iter);

	--connection_counter_;

#ifdef DEBUG_2
	printf("Num of Connection: %u\n", connection_counter_);
	printf("Exit thread_exit_callback_func()\n");
#endif
}

/// The callback function for the NetworkConnection. When the acceptor accept the connection, this function will be called.
void HttpServer::handle_accept(const boost::system::error_code& error)
{

	bool trigger_callback = false;
	if (!error)
	{
#ifdef DEBUG_2
		printf("Prepare to start one new connection.\n");
#endif
		boost::mutex::scoped_lock lock(mutex_);

		if(!is_started_)
			return;

		if(new_connection_)
		{
			// Register the callback function for thread exit.
			new_connection_->SetThreadExitCallback(boost::bind(&HttpServer::thread_exit_callback_func, this));

			// FixMe: We must implement max connection in the future.
			// Create one thread to run the connection.
			ThreadPtr temp_thread = ThreadPtr(new boost::thread(boost::bind(&INetworkConnection::StartConnection, new_connection_)));
			{
				// Check the thread id in the map.
				ThreadIdMapIter iter = thread_id_map_.find(temp_thread->get_id());
				assert(iter == thread_id_map_.end());
				if(iter != thread_id_map_.end())
				{
					printf("[HTTP server]: The thread id has already exist in the thread_id_map_\n");
					return;
				}

				// Store the thread id, thread and connection in the map.
				ThreadConnection thread_connection;
				thread_connection.thread = temp_thread;
				thread_connection.connection = new_connection_;
				thread_id_map_[temp_thread->get_id()] = thread_connection;
			}

			++connection_counter_;
#ifdef DEBUG_2
			printf("Num of Connection: %u\n", connection_counter_);
#endif
		}

		// Create new connection.
		new_connection_.reset(this->create_connection());
		// Prepare to accept the new connection.
		if(new_connection_)
			acceptor_.async_accept(new_connection_->GetTCPSocket(),
				make_custom_alloc_handler(allocator_, accept_callback_));
		else
			printf("[HTTP server]: Connection is NULL.\n");

		trigger_callback = (connection_counter_ >= (unsigned int)max_connection_num_);
	}
	else
	{
		// Handle error
		printf("In handle_accept, Error: %s\n", error.message().c_str());
		is_started_ = false;
		return;
	}

	if(trigger_callback && meet_max_connection_callback_)
	{
		meet_max_connection_callback_();
	}

}

/**
 * @brief The simple factory method for HTTP connecetion.
 * @return The pointer of network connection.
 */
HttpConnection* HttpServer::create_connection()
{
	// Create new connection
	return ( new HttpConnection(http_request_dispatcher_, service_engine_.GetIOService(), ssl_context_) );
}

/// Callback function for SSL connection.
std::string HttpServer::get_ssl_private_key_password() const
{
	return ssl_private_key_password_;
}
} // namespace FBi
