#include <http_server/http_connection.hpp>
#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <cassert>

#include <cstdio>

namespace FBi
{

/*static void internal_ssl_shutdown(boost::asio::ssl::stream<boost::asio::ip::tcp::socket&>* sock, const boost::system::error_code& error)
{
	if(!error)
	{
		boost::system::error_code error1;
		sock->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
		sock->lowest_layer().close(error1);
	}
}
*/

/**
 * @brief Constructor of HTTP connection.
 *
 * @param[in] request_handler The HTTP request handler.
 * @param[in] io_service The IO service of ASIO.
 * @param[in] ssl_context The SSL context of ASIO (It can pass NULL when using the TCP connection).
 */
HttpConnection::HttpConnection(const boost::shared_ptr<IHttpRequestDispatcher>& request_handler, boost::asio::io_service& io_service, boost::asio::ssl::context& ssl_context)
	: http_request_handler_(request_handler),
	tcp_socket_(io_service),
	ssl_socket_(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket&>(tcp_socket_, ssl_context)),
	is_ssl_(false),
	keep_alive_(true),
	http_msg_parser_(HttpMsgParser::Request_Mode),
	previous_unhandled_data_index_(0),
	previous_unhandled_data_size_(0),
	thread_exit_callback_(NULL),
	is_running_(false),
	state_(receive_new_request),
	is_body_compressed_(false),
	is_100_continue_(false),
	is_server_push_enabled_(false)
{
#ifdef DEBUG_2
	printf("HttpConnection constructor\n");
#endif

	// Prepare callbacks for parser
	callbacks_.method_callback = boost::bind(&HttpConnection::method_callback, this, _1);
	callbacks_.uri_callback = boost::bind(&HttpConnection::uri_callback, this, _1);
	callbacks_.http_version_callback = boost::bind(&HttpConnection::http_version_callback, this, _1, _2);
	//callbacks_.status_code_callback = NULL;
	//callbacks_.reason_phrase_callback = NULL;
	callbacks_.one_header_callback = boost::bind(&HttpConnection::one_header_callback, this, _1, _2);

	// Regsiter callbacks for parser
	http_msg_parser_.SetCallbacks(callbacks_);

	reset_unhandled_data_info();
}

HttpConnection::~HttpConnection()
{
#ifdef DEBUG_2
	printf("HttpConnection destructor\n");
#endif
	is_running_ = false;
	shutdown_sock();

	// Wait some tasks to complete ?
	// No.

	if(ssl_socket_)
	{
		delete ssl_socket_;
		ssl_socket_ = NULL;
	}
#ifdef DEBUG_2
	printf("HttpConnection destructor exit\n");
#endif
}

/// Function to shutdown the socket.
void HttpConnection::shutdown_sock()
{
	if(is_ssl_)
	{
		if(ssl_socket_)
		{
			if(!ssl_socket_->lowest_layer().is_open())
				return;
#ifdef DEBUG_2
			printf("SSL ShutdownSock\n");
#endif

			boost::system::error_code error1;
			ssl_socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
			ssl_socket_->lowest_layer().close(error1);

			/*
			ssl_socket_->async_shutdown(boost::bind(internal_ssl_shutdown, ssl_socket_, _1));
#ifndef WIN32
			sleep(1);
#else
			Sleep(1000);
#endif
			if(ssl_socket_->lowest_layer().is_open())
			{
				ssl_socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
				ssl_socket_->lowest_layer().close(error1);
			}
			*/

		}
		else
		{
			printf("HttpConnection::shutdown_sock => SSL Socket is NULL.\n");
		}
	}
	else
	{
		if(!tcp_socket_.lowest_layer().is_open())
			return;
#ifdef DEBUG_2
		printf("TCP ShutdownSock\n");
#endif

		boost::system::error_code error1;
		tcp_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error1);
		tcp_socket_.close(error1);
	}
}

/**
 * @brief Callback function for the HTTP parser.
 *
 * @param[in] method The method name of HTTP request message.
 */
void HttpConnection::method_callback(const std::string& method)
{
	request_msg_.method = method;
}

/**
 * @brief Callback function for the HTTP parser.
 *
 * @param[in] uri The URI of HTTP request message.
 */
void HttpConnection::uri_callback(const std::string& uri)
{
#ifdef DEBUG_2
	printf("Raw URI = %s\n", uri.c_str());
#endif

	if(!HTTP_URL_Utility::ParseHttpUrl(uri, request_msg_.uri))
		printf("HttpConnection::uri_callback => Parse HTTP URL Failed.\n");
}

/**
 * @brief Callback function for the HTTP parser.
 *
 * @param[in] major The major number of HTTP version number.
 * @param[in] minor The minor number of HTTP version number.
 */
void HttpConnection::http_version_callback(int major, int minor)
{
	request_msg_.http_version_major = major;
	request_msg_.http_version_minor = minor;
}

/**
 * @brief Callback function for the HTTP parser.
 *
 * @param[in] field_name The field name of one HTTP header.
 * @param[in] field_value The field value of one HTTP header.
 */
void HttpConnection::one_header_callback(const std::string& field_name, const std::string& field_value)
{
	(request_msg_.headers)[field_name] = field_value;
}

/// Function to start this connection.
void HttpConnection::StartConnection()
{
	is_ssl_ = is_ssl_protocol();

#ifdef DEBUG_2
	if(is_ssl_)
		printf("HttpConnection::StartConnection => SSL.\n");
	else
		printf("HttpConnection::StartConnection => TCP.\n");
#endif

	try
	{
		boost::system::error_code err_code;
		boost::asio::ip::tcp::socket::endpoint_type remote_endpoint = tcp_socket_.lowest_layer().remote_endpoint(err_code);
		if(!err_code)
		{
			connection_info_.remote_address = remote_endpoint.address().to_string(err_code);
			connection_info_.remote_port = remote_endpoint.port();
		}

		tcp_socket_.lowest_layer().set_option(boost::asio::socket_base::keep_alive(keep_alive_));
		boost::asio::socket_base::linger linger_opt(true, 10);
		tcp_socket_.lowest_layer().set_option(linger_opt);
		tcp_socket_.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));

		if(is_ssl_)
		{
			if(ssl_socket_)
			{
				ssl_socket_->handshake(boost::asio::ssl::stream_base::server);
				thread_main_loop();
			}
			else
			{
				printf("SSL Socket is NULL.\n");
			}
		}
		else
		{
			thread_main_loop();
		}

	}
	catch(...)
	{
		printf("HttpConnection::StartConnection error\n");
	}

	if(thread_exit_callback_)
		thread_exit_callback_();
}

/// Function to stop this connection.
void HttpConnection::StopConnection()
{
	is_running_ = false;

	if(force_exit_push_mode_callback_)
		force_exit_push_mode_callback_(request_msg_.uri.query_strings);

	shutdown_sock();
}

/**
 * @brief Function to detect whether the current session is SSLv2 or not.
 *
 * @param[in] data The data which should be scanned.
 * @param[in] bytes The number of bytes of data.
 * @return true: The current session is SSLv2. false: The current session is not SSLv2.
 */
bool HttpConnection::is_sslv2(const char* data, size_t bytes)
{
	// We can test with "openssl s_client -connect 127.0.0.1:60000 -ssl2"
	if( (bytes >= 3) && 
			((data[0] & 0x80) == 0x80) && 
			(((data[0] & 0x7f) << 8 | data[1]) > 9) && 
			(data[2] == 0x01) )
		return true;

	return false;

}

/**
 * @brief Function to detect whether the current session is SSLv3, TLSv1.0, TLSv1.1 or not.
 *
 * @param[in] data The data which should be scanned.
 * @param[in] bytes The number of bytes of data.
 * @return true: The current session is SSLv3, TLSv1.0 or TLSv1.1. false: The current session is not SSLv3, TLSv1.0 and TLSv1.1.
 */
bool HttpConnection::is_sslv3_or_tlsv1_0_or_tlsv1_1(const char* data, size_t bytes)
{
	// We can test with "openssl s_client -connect 127.0.0.1:60000"
	if( (bytes >= 6) && (data[0] == 0x16) && (data[5] == 0x01) )
		return true;

	return false;
}

/**
 * @brief Function to detect whether the current session is SSL or not.
 *
 * @return true: The current session is SSL. false: The current session is not SSL.
 */
bool HttpConnection::is_ssl_protocol()
{
	char ssl_headers[6] = {'\0'};
	boost::system::error_code err_code;
	size_t bytes_transferred = tcp_socket_.receive(boost::asio::buffer(ssl_headers), boost::asio::socket_base::message_peek, err_code);

	if(err_code)
	{
		printf("HttpConnection::is_ssl_protocol => Can not peek ssl headers\n");
		return false;
	}

#ifdef DEBUG_2
	printf("peek bytes = %ld\n", bytes_transferred);
	printf("Is SSL 3.0 or TLS 1.0 and 1.1 ? %d\n", HttpConnection::is_sslv3_or_tlsv1_0_or_tlsv1_1(ssl_headers, bytes_transferred));
	printf("Is SSL 2 ? %d\n", HttpConnection::is_sslv2(ssl_headers, bytes_transferred));

	for(size_t i = 0; i < 6; ++i)
	{
		printf("%x\n", ssl_headers[i]);
	}
#endif

	return ( HttpConnection::is_sslv3_or_tlsv1_0_or_tlsv1_1(ssl_headers, bytes_transferred) || HttpConnection::is_sslv2(ssl_headers, bytes_transferred) );
}

/**
 * @brief Function to get the internal ASIO TCP socket.
 *
 * @return true: The ASIO TCP socket.
 */
boost::asio::ip::tcp::socket& HttpConnection::GetTCPSocket()
{
	return tcp_socket_;
}

/**
 * @brief Function to register the ThreadExitCallback function. (This function should be called before calling StartConnection())
 *
 * @param[in] callback The callback function to be registered. It will be called when the thread of connection prepares to exit and end.
 */
void HttpConnection::SetThreadExitCallback(const ThreadExitCallback& callback)
{
	// This function should be called before start connection.
	thread_exit_callback_ = callback;
}

/**
 * @brief Function to register the ServerPushDataCallback function.
 *
 * @param[in] callback The callback function to be registered. It will be called when server want to get data in the server push mode.
 */
void HttpConnection::RegisterServerPushDataCallback(const ServerPushDataCallback& callback)
{
	server_push_data_callback_ = callback;
}

/**
 * @brief Function to register the ForceExitPushModeCallback function.
 *
 * @param[in] callback The callback function to be registered. It will be called when this connection will be stopped.
 */
void HttpConnection::RegisterForceExitPushModeCallback(const ForceExitPushModeCallback& callback)
{
	force_exit_push_mode_callback_ = callback;
}

/// Function to enable server push mode.
void HttpConnection::EnableServerPushMode()
{
	is_server_push_enabled_ = true;
}

/**
 * @brief Function to transfer the string to the zero or positive integer.
 *
 * @param[in] str The string need to be transfered.
 * @param[out] result The output integer.
 */
bool HttpConnection::string_to_size_t(const std::string& str, size_t& result)
{
	result = 0;
	for(size_t i = 0; i < str.length(); ++i)
	{
		if((str[i] >= '0') && (str[i] <= '9'))

		{
			result = result*10 + static_cast<size_t>(str[i] - '0');
		}
		else
		{
			result = 0;
			return false;
		}
	}
	return true;
}

/**
 * @brief Function to decompress the body of request message.
 *
 * @return true if it is successful, and false if it is failed.
 */
bool HttpConnection::decompress_body_of_request_msg()
{
	// Parse the Content-Encoding string into vector.
	std::vector<std::string> encoding_vector;
	HttpMsgUtility::ParseEnconding(request_msg_.headers["content-encoding"], encoding_vector);
	//Decompress the entity body of HTTP request message.
	HttpMsgUtility::CompressedResult compressed_result = HttpMsgUtility::DecompressBodyOfHttpMsg(encoding_vector, request_msg_);

	if(compressed_result == HttpMsgUtility::CompressedFailed)
	{
		// Internal Server Error
		generate_simple_http_response(500);
		return false;
	}
	else if(compressed_result == HttpMsgUtility::CompressedUnsupported)
	{
		// 415 Unsupported Media Type
		generate_simple_http_response(415);
		return false;
	}

	return true;
}

/// Function to reset the HTTP messages and parser.
void HttpConnection::reset_before_receive_new_request()
{
	// Reset the HTTP request message struct
	request_msg_.Reset();

	// Reset the HTTP response message struct
	response_msg_.Reset();

	// Reset the state of HTTP message parser
	http_msg_parser_.ResetState(HttpMsgParser::Request_Mode);

	is_body_compressed_ = false;
}

/// Function to reset the information about the unhandled data. They store some information about the unhandled data in the buffer.
void HttpConnection::reset_unhandled_data_info()
{
	// Set invalid index.
	previous_unhandled_data_index_ = CONNECTION_BUFFER_SIZE;

	// Set zero size.
	previous_unhandled_data_size_ = 0;
}

/**
 * @brief Function to generate the HTTP response message corresponds with the input status code.
 *
 * @param[in] status_code The status code of HTTP response message.
 */
void HttpConnection::generate_simple_http_response(int status_code)
{
	HttpMsgUtility::SetDefaultStatusCode(response_msg_, status_code);
	response_msg_.entity_body.clear();
}

/// The main loop of thread for this connection.
void HttpConnection::thread_main_loop()
{
	is_running_ = true;
	size_t index = 0;
	size_t size = 0;
	state_ = receive_new_request;

	while(is_running_)
	{
		switch(state_)
		{
		case receive_new_request:
			index = size = 0; // Reset
			reset_before_receive_new_request();
			state_ = read_http_headers;
		case read_http_headers:
			read_http_msg_headers(index, size);
			break;
		case processing_http_headers:
			// The index and size are passed from read_http_msg_headers
			size = processing_http_msg_headers(index, size);
			break;
		case read_http_body:
			read_http_msg_body(size);
			break;
		case send_response:
			processing_send_http_response(index, size);
			break;
		case send_response_then_shutdown:
			keep_alive_ = false; // We want to close this connection.
			send_http_response(shutdown);
			break;
		case error_to_shutdown:
			is_running_ = false;
			handle_asio_error(latest_error_);
			break;
		case shutdown:
			is_running_ = false;
			break;
		case server_push:
#ifdef DEBUG_2
			printf("================= Server push Start. =================\n");
#endif
			server_push_main_loop();
#ifdef DEBUG_2
			printf("================= Server push End. =================\n");
#endif
			break;
		default:
			is_running_ = false;
			break;
		}
	}
}

/// The main loop for the server push mode.
void HttpConnection::server_push_main_loop()
{
#ifdef DEBUG_2
	printf("\n================After decompression (Server Push Request) ============");
	print_http_request_msg();
#endif

	ProcessingState temp_next_state = receive_new_request;
	std::map<std::string, std::string> push_headers;
	std::string data;
	bool is_end = false;
	std::string complete_result_body;

	// Generate the boundary.
	std::string boundary_str = HttpMsgUtility::GenerateServerPushBoundary();

	// Add HTTP header.
	HttpMsgUtility::SetGeneralHttpResponseHeaders(response_msg_);
	// Status code: 200 OK
	HttpMsgUtility::SetDefaultStatusCode(response_msg_, 200);
	response_msg_.headers["Content-Type"] = "multipart/x-mixed-replace;boundary=";
	response_msg_.headers["Content-Type"] += boundary_str;
	response_msg_.headers["Server"] = "FBi-HTTP-Server";

	// Add first boundary.
	response_msg_.entity_body.clear();
	response_msg_.entity_body = "--";
	response_msg_.entity_body += boundary_str;
	response_msg_.entity_body += "\r\n";

	// Get first server push body.
	server_push_data_callback_(request_msg_.uri.query_strings, false, push_headers, data, is_end);
	HttpMsgUtility::ComposeServerPushBody(push_headers, data, boundary_str, is_end, complete_result_body);
	response_msg_.entity_body += complete_result_body;

	// Send first push data.
	std::string response;
	HttpMsgUtility::ComposeHttpResponseMsg(response_msg_, response);

#ifdef DEBUG_2
	for(size_t i = 0; i < response.length(); ++i)
		printf("%c", response[i]);
#endif

	if(is_ssl_)
	{
		if(ssl_socket_)
			boost::asio::write(*ssl_socket_, boost::asio::buffer(response.c_str(), response.length()), latest_error_);
		else
			printf("HttpConnection<T>::server_push_main_loop => SSL Socket is NULL.\n");
	}
	else
	{
		boost::asio::write(tcp_socket_, boost::asio::buffer(response.c_str(), response.length()), latest_error_);
	}

	if(latest_error_)
	{
		state_ = error_to_shutdown;
		is_server_push_enabled_ = false;
		return;
	}

	// Continue send push data.
	while(is_running_ && !is_end)
	{
		push_headers.clear();
		data.clear();
		complete_result_body.clear();

		server_push_data_callback_(request_msg_.uri.query_strings, true, push_headers, data, is_end);
		HttpMsgUtility::ComposeServerPushBody(push_headers, data, boundary_str, is_end, complete_result_body);

#ifdef DEBUG_2
		for(size_t i = 0; i < complete_result_body.length(); ++i)
			printf("%c", complete_result_body[i]);
#endif

		if(is_ssl_)
		{
			if(ssl_socket_)
				boost::asio::write(*ssl_socket_, boost::asio::buffer(complete_result_body.c_str(), complete_result_body.length()), latest_error_);
			else
				printf("HttpConnection<T>::server_push_main_loop => SSL Socket is NULL.\n");
		}
		else
		{
			boost::asio::write(tcp_socket_, boost::asio::buffer(complete_result_body.c_str(), complete_result_body.length()), latest_error_);
		}

		if(latest_error_)
		{
			temp_next_state = error_to_shutdown;
			break;
		}
	}

	is_server_push_enabled_ = false;
	state_ = temp_next_state;
}

/**
 * @brief Function to read the method name, URI, http version and headers of HTTP request message from network or buffer.
 *
 * @param[out] unhandled_index The index point to the start index of unhandled data.
 * @param[out] unhandled_size The size of the unhandled data (bytes).
 */
void HttpConnection::read_http_msg_headers(size_t& unhandled_index, size_t& unhandled_size)
{
	size_t index = 0;
	size_t size = 0;

	while(is_running_)
	{
		if((previous_unhandled_data_size_ == 0) || (previous_unhandled_data_index_ >= CONNECTION_BUFFER_SIZE))
		{
			// Read data from network
			index = 0;

			if(is_ssl_)
			{
				if(ssl_socket_)
					size = ssl_socket_->read_some(boost::asio::buffer(buffer_), latest_error_);
				else
					printf("HttpConnection::read_http_msg_headers => SSL Socket is NULL.\n");
			}
			else
			{
				size = tcp_socket_.read_some(boost::asio::buffer(buffer_), latest_error_);
			}

			if(latest_error_)
			{
				// Handle error
				state_ = error_to_shutdown;
				return;
			}
		}
		else
		{
			// Read data from buffer
			index = previous_unhandled_data_index_;
			size = previous_unhandled_data_size_;
		}

		// Parsing headers
		size_t last_index = 0;
		HttpMsgParser::ParsingResult parsing_result = http_msg_parser_.Parse(buffer_ + index, size, last_index);

		if(parsing_result == HttpMsgParser::Parse_Processing)
		{
			// If it is in this mode. It means the data in the buffer is already processing.
			// We need to read data from network. So we reset the information about the unhandled data.
			reset_unhandled_data_info();
			// Continue read HTTP headers
			continue;
		}
		else if(parsing_result == FBi::HttpMsgParser::Parse_OK)
		{
			// After processing the HTTP message headers, we may know the length of body. Then we can process the body.
			state_ = processing_http_headers;
			// The index and size pass to next state
			unhandled_index = index + last_index;
			unhandled_size = size - last_index;
			return;
		}
		//else if(parsing_result == FBi::HttpMsgParser::Parse_Fail)
		else
		{
			printf("Parse_Fail or Unknown state.\n");
			// Handle error!
			// Send 400 (bad request)
			generate_simple_http_response(400);
		
                        state_ = send_response_then_shutdown;
			return;
		}

	}
}

/**
 * @brief Function to process the headers of HTTP request message and get the length of entity body of HTTP message.
 *
 * @param[in] unhandled_index The index point to the start index of unhandled data.
 * @param[in] unhandled_size The size of the unhandled data (bytes).
 *
 * @return The total bytes of body in request message.
 */
size_t HttpConnection::processing_http_msg_headers(size_t unhandled_index, size_t unhandled_size)
{
	assert(request_msg_.entity_body.length() == 0);

	HTTPMsgHeadersIter iter;

	// Handle 100 continue request.
	if(is_100_continue_)
	{
		// If we enter this block, it means we already handle the 100 continue request headers and response it.
		// So we need to receive and handle the entity body now.
		// Prepare handle body.
		is_100_continue_ = false;
		response_msg_.Reset();
	}
	else if(request_msg_.headers.end() != (iter = request_msg_.headers.find("expect")))
	{
		std::string temp_str;
		for(size_t i = 0; i < (*iter).second.length(); ++i)
			temp_str.push_back(tolower((*iter).second[i]));

		if(iter->second.find("100-continue") != std::string::npos)
		{
			// FixMe: We need to check some headers can be supported or not.

			// Prepare send 100 continue response.
			is_100_continue_ = true;
			generate_simple_http_response(100);
		}
		else
		{
			// We do not handle other expect headers now.
			printf("Can not handle expect header!\n");
			generate_simple_http_response(417);
		}
		state_ = send_response;

		// Remember the unhandled index and size for next turn.
		previous_unhandled_data_index_ = unhandled_index;
		previous_unhandled_data_size_ = unhandled_size;
		return 0;
	}

	// Handle connection header.
	iter = request_msg_.headers.find("connection");
	if(request_msg_.headers.end() != iter)
	{
		std::string temp_str;
		for(size_t i = 0; i < (*iter).second.length(); ++i)
			temp_str.push_back(tolower((*iter).second[i]));

		if( temp_str.compare("keep-alive") == 0 )
			keep_alive_ = true;
		//else if( temp_str.compare("close") == 0 )
		else
			keep_alive_ = false;
	}

	/*
	if(request_msg_.headers.end() != (iter = request_msg_.headers.find("transfer-encoding")) )
	{
		if( (*iter).second.compare("chunked") == 0 )
		{
			// Transfer-Encoding: chunked
			// We do not handle it now.
		}
		else
		{
			// We do not handle it now.
		}
	}
	*/

	// Check the body is compressed or not.
	if( request_msg_.headers.end() != (iter = request_msg_.headers.find("content-encoding")) )
	{
		is_body_compressed_ = true;
	}

	if( request_msg_.headers.end() != (iter = request_msg_.headers.find("content-length")) )
	{
		size_t body_total_bytes = 0;
		// If the Content-Length is valid
		if(string_to_size_t((*iter).second, body_total_bytes))
		{
			if(unhandled_size < body_total_bytes)
			{
				// Record the start index of unhandled data in entity body then append the unhandled data to entity body.
				// Call the request handler to handle the data.
				if(unhandled_size > 0)
				{
					size_t now_unhandled_index = request_msg_.entity_body.length();
					request_msg_.entity_body.append(buffer_ + unhandled_index, unhandled_size);
					if(http_request_handler_ && (!is_body_compressed_))
						http_request_handler_->HandlePartialBody(request_msg_, response_msg_, now_unhandled_index, unhandled_size, false, connection_info_, this);
				}

				// If we have not gotten the complete entity body of HTTP message then we continue read the data.
				if(request_msg_.entity_body.length() < body_total_bytes)
				{
					state_ = read_http_body;
					return body_total_bytes; // Pass the length of body to read_http_msg_body function.
				}
				else
				{
					// It never go into this block, if we have reset the message struct before receive new request.
					// Handle error!
					// Send 500 (Internal Server Error)
					printf("Server error!!\n");
					generate_simple_http_response(500);
				
					state_ = send_response_then_shutdown;
					return 0;
				}
			}
			else
			{
				// We get complete body here.
				// We remember the index that point to the unhandled data and the size of unhandled data.
				// Importantance: We can not change the data in buffer_ before we processing the 
				// unhandled data. Otherwise, we need addtional variable to store the unhandled data.
				previous_unhandled_data_index_ = unhandled_index + body_total_bytes;
				previous_unhandled_data_size_ = unhandled_size - body_total_bytes;	// Because we read this body first time. So we do this calculation.
				size_t now_unhandled_index = request_msg_.entity_body.length();
				request_msg_.entity_body.append(buffer_ + unhandled_index, body_total_bytes);
				if(http_request_handler_)
				{
					if(is_body_compressed_)
					{
						if(decompress_body_of_request_msg())
						{
							http_request_handler_->HandlePartialBody(request_msg_, response_msg_, 0, request_msg_.entity_body.length(), true, connection_info_, this);
						}
						else
						{
							// Decompression failed. Can we continue receive message here?
							state_ = send_response;
							return 0;
						}
					}
					else
					{
						http_request_handler_->HandlePartialBody(request_msg_, response_msg_, now_unhandled_index, body_total_bytes, true, connection_info_, this);
					}
				}

				// Send response.
				state_ = send_response;

				// We return zero because we got the complete body. We don't need to read the body.
				return 0;
			}

		}
		else
		{
			// Handle error!
			// Send 411 (length required)
			generate_simple_http_response(411);

			state_ = send_response_then_shutdown;
			return 0;
		}

	}

	// We think it is nobody
	if(http_request_handler_)
	{
		previous_unhandled_data_index_ = unhandled_index;
		previous_unhandled_data_size_ = unhandled_size;

		http_request_handler_->HandlePartialBody(request_msg_, response_msg_, 0, request_msg_.entity_body.length(), true, connection_info_, this);
		// Send response.
		state_ = send_response;

		// We return zero because we got the complete body. We don't need to read the body.
		return 0;
	}

	// Handle error!
	// Send 411 (length required)
	generate_simple_http_response(411);

	state_ = send_response_then_shutdown;
	return 0;
}

/**
 * @brief Function to read the entity body of HTTP message.
 *
 * @param[in] total_bytes The length of entity body (bytes).
 */
void HttpConnection::read_http_msg_body(size_t total_bytes)
{

	size_t bytes_transferred = 0;

	while(is_running_)
	{
		if(is_ssl_)
		{
			if(ssl_socket_)
				bytes_transferred = ssl_socket_->read_some(boost::asio::buffer(buffer_), latest_error_);
			else
				printf("HttpConnection::read_http_msg_body => SSL Socket is NULL.\n");
		}
		else
		{
			bytes_transferred = tcp_socket_.read_some(boost::asio::buffer(buffer_), latest_error_);
		}

		if(!latest_error_)
		{
			assert(request_msg_.entity_body.length() <= total_bytes);

			size_t temp_size = request_msg_.entity_body.length() + bytes_transferred;
			if(temp_size < total_bytes)
			{
				// Get partial HTTP message body
				size_t now_unhandled_index = request_msg_.entity_body.length();
				request_msg_.entity_body.append(buffer_, bytes_transferred);
				if(http_request_handler_ && (!is_body_compressed_))
					http_request_handler_->HandlePartialBody(request_msg_, response_msg_, now_unhandled_index, bytes_transferred, false, connection_info_, this);

				// Continue receive HTTP message body
				continue;
			}
			else
			{
				// We get complete body here.
				// We remember the index that point to the unhandled data and the size of unhandled data.
				// Importantance: We can not change the data in buffer_ before we processing the 
				// unhandled data. Otherwise, we need addtional variable to store the unhandled data.
				// total_bytes - request_msg_.entity_body.length() before append data:
				// 1. It is previous_unhandled_data_index_
				// 2. It is the un-append size of body, too.
				previous_unhandled_data_index_ = total_bytes - request_msg_.entity_body.length();
				previous_unhandled_data_size_ = bytes_transferred - previous_unhandled_data_index_;
				size_t now_unhandled_index = request_msg_.entity_body.length();
				request_msg_.entity_body.append(buffer_, previous_unhandled_data_index_);
				if(http_request_handler_)
				{
					if(is_body_compressed_)
					{
						if(decompress_body_of_request_msg())
						{
							http_request_handler_->HandlePartialBody(request_msg_, response_msg_, 0, request_msg_.entity_body.length(), true, connection_info_, this);
						}
						else
						{
							// Decompression failed. Can we continue receive message here?
							state_ = send_response;
							return;
						}
					}
					else
					{
						http_request_handler_->HandlePartialBody(request_msg_, response_msg_, now_unhandled_index, previous_unhandled_data_index_, true, connection_info_, this);
					}
				}

				// Send response.
				state_ = send_response;
				return;
			}
		}
		else
		{
			state_ = error_to_shutdown;
			return;
		}
	}
}

/**
 * @brief Function to prepare to send HTTP response message to client.
 *
 * @param[in] index The index point to the start index of unhandled data.
 * @param[in] size The size of the unhandled data (bytes).
 */
void HttpConnection::processing_send_http_response(size_t& index, size_t& size)
{
	ProcessingState after_send_response_state = receive_new_request;

	if(!http_request_handler_)
	{
		printf("There is no a request handler callback to be registered.\n");
		// We can not find the HTTP request handler.
		// Send 503 (Service Unavailable)
		generate_simple_http_response(503);
		after_send_response_state = receive_new_request;
	}
	else if(is_100_continue_)
	{
		// Remember the unhandled index and size for next turn to handle headers.
		index = previous_unhandled_data_index_;
		size = previous_unhandled_data_size_;
		// If it is 100 continue request, the next data will be entity body.
		// So we set the state to process HTTP headers then receive body.
		after_send_response_state = processing_http_headers;
	}
	else if(is_server_push_enabled_)
	{
		if(server_push_data_callback_)
		{
			state_ = server_push;
			return;
		}

		printf("There is no a server push callback to be registered.\n");
		// We can not find the HTTP server push handler.
		// Send 503 (Service Unavailable)
		generate_simple_http_response(503);
		after_send_response_state = receive_new_request;
		is_server_push_enabled_ = false;
		return;
	}
	else if(keep_alive_)
	{
		after_send_response_state = receive_new_request;
	}
	else
	{
		after_send_response_state = shutdown;
	}

	send_http_response(after_send_response_state);
}

/**
 * @brief Function to send the HTTP response message to client.
 *
 * @param[in] next_state_when_success The next state when sending step is successful.
 */
void HttpConnection::send_http_response(enum ProcessingState next_state_when_success)
{
#ifdef DEBUG_2
	printf("\n================After decompression============");
	print_http_request_msg();
#endif

	// Set the general HTTP headers
	HttpMsgUtility::SetGeneralHttpResponseHeaders(response_msg_);

	// Set default server name
	if(response_msg_.headers["Server"].empty())
		response_msg_.headers["Server"] = "FBi-HTTP-Server";

	// Set the length of content.
	HttpMsgUtility::SetContentLength(response_msg_);

	// Attach the keep alive header.
	if(keep_alive_)
		response_msg_.headers["Connection"] = "keep-alive";
	else
		response_msg_.headers["Connection"] = "close";

#ifdef DEBUG_2
	printf("\n================Before compression============");
	print_http_response_msg();
#endif

	// Compress the content (entity body) of response message.
	if(response_msg_.entity_body.length() > 0)
	{
		HttpMsgUtility::CompressedResult temp_result = HttpMsgUtility::CompressBodyOfResponseMsg(request_msg_, response_msg_);

		if(temp_result == HttpMsgUtility::CompressedSuccess)
		{
			// Set the length of content.
			HttpMsgUtility::SetContentLength(response_msg_);

#ifdef DEBUG_2
			printf("\n================After compression============");
			print_http_response_msg();
#endif
		}
	}

	// Create the HTTP response string then send it.
	std::string response;
	HttpMsgUtility::ComposeHttpResponseMsg(response_msg_, response);

	if(is_ssl_)
	{
		if(ssl_socket_)
			boost::asio::write(*ssl_socket_, boost::asio::buffer(response.c_str(), response.length()), latest_error_);
		else
			printf("HttpConnection::send_http_response => SSL Socket is NULL.\n");
	}
	else
	{
		boost::asio::write(tcp_socket_, boost::asio::buffer(response.c_str(), response.length()), latest_error_);
	}

	if(!latest_error_)
	{
		state_ = next_state_when_success;
		return;
	}
	else
	{
		state_ = error_to_shutdown;
		return;
	}
}

/**
 * @brief Function to handle the error of the ASIO.
 *
 * @param[in] error Error of ASIO.
 */
void HttpConnection::handle_asio_error(const boost::system::error_code& error)
{
	printf("handle_asio_error\n");
	// When asio error occurs, we do not send the response back to client.
	if(error == boost::asio::error::eof)
	{
		// When the EOF occurs, the client may shutdown the connection (TCP).
		printf("EOF\n");
		printf("Error: %s\n", error.message().c_str());
	}
	else
	{
		// When the short read occurs, the client may shutdown the connection (SSL).
		// Handle error here
		printf("Error: %s\n", error.message().c_str());
	}
}

void HttpConnection::print_http_request_msg()
{
	printf("\n\n*************Request Message*************\n");
	printf("\nRequest Line ====>\n");
	printf("Method = %s\\n\n", request_msg_.method.c_str());
	URI_Utility::PrintURI(request_msg_.uri);
	printf("version = %d.%d\\n\n",request_msg_.http_version_major, request_msg_.http_version_minor);

	HTTPMsgHeadersIter iter;
	printf("\nHeaders =======>\n");
	for (iter = request_msg_.headers.begin(); iter != request_msg_.headers.end(); iter++)
		printf("%s:%s\\n\n", (*iter).first.c_str(), (*iter).second.c_str());

	printf("\nBody =======> Real length of decompressed body = %lu\n", request_msg_.entity_body.length());
	for(size_t i = 0; i < request_msg_.entity_body.length(); ++i)
		printf("%c",request_msg_.entity_body[i]);
	//printf("%s", request_msg_.entity_body.c_str());
	printf("\n=====================\n");
}

void HttpConnection::print_http_response_msg()
{
	printf("\n\n*************Response Message*************\n");
	printf("\nStatus Line ====>\n");
	printf("version = %d.%d\\n\n",response_msg_.http_version_major, response_msg_.http_version_minor);
	printf("Status Code = %d\\n\n", response_msg_.status_code);
	printf("Reason phrase = %s\\n\n", response_msg_.reason_phrase.c_str());

	HTTPMsgHeadersIter iter;
	printf("\nHeaders =======>\n");
	for (iter = response_msg_.headers.begin(); iter != response_msg_.headers.end(); iter++)
		printf("%s:%s\\n\n", (*iter).first.c_str(), (*iter).second.c_str());

	printf("\nBody =======>\n");
	for(size_t i = 0; i < response_msg_.entity_body.length(); ++i)
		printf("%c",response_msg_.entity_body[i]);
	//printf("%s", response_msg_.entity_body.c_str());
	printf("\n=====================\n");
}

}
