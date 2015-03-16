#ifndef FBi_HTTP_CONNECTION_HPP
#define FBi_HTTP_CONNECTION_HPP

#include <http_server/libhttp_server_config.h>
#include <libtransport/socket_define.h>

#include <boost/asio/ssl.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <string>

#include <http_server/http_msg_parser.hpp>
#include <http_server/http_msg.hpp>

#include <http_server/network_connection_if.hpp>
#include <http_server/http_request_handler_if.hpp>

#define CONNECTION_BUFFER_SIZE 1024


namespace FBi
{

/// The HTTP connection class for HTTP server
class LIBHTTPSERVER_DECL HttpConnection : public INetworkConnection, public INetworkPushDataConnection, private boost::noncopyable
{
public:
	HttpConnection(const boost::shared_ptr<IHttpRequestDispatcher>&, boost::asio::io_service&, boost::asio::ssl::context&);
	~HttpConnection();

	void StartConnection();
	void StopConnection();
	void SetThreadExitCallback(const ThreadExitCallback&);

	// For server push.
	void RegisterServerPushDataCallback(const ServerPushDataCallback& callback);
	void RegisterForceExitPushModeCallback(const ForceExitPushModeCallback& callback);
	void EnableServerPushMode();

	// For acceptor.
	boost::asio::ip::tcp::socket& GetTCPSocket();

private:
	enum ProcessingState
	{
		receive_new_request,
		read_http_headers,
		processing_http_headers,
		read_http_body,
		send_response,
		send_response_then_shutdown,
		error_to_shutdown,
		shutdown,
		server_push
	};

private:
/*	HttpConnection(const HttpConnection&);
	HttpConnection& operator=(const HttpConnection&);*/

	void shutdown_sock();

	// For parser.
	void method_callback(const std::string&);
	void uri_callback(const std::string&);
	void http_version_callback(int, int);
	void one_header_callback(const std::string&, const std::string&);

	// Transfer string to integer.
	bool string_to_size_t(const std::string&, size_t&);

	// For decompress
	bool decompress_body_of_request_msg();

	// Reset.
	void reset_before_receive_new_request();
	void reset_unhandled_data_info();

	// Main thread loop function.
	void thread_main_loop();

	// Read request message and parse headers.
	void read_http_msg_headers(size_t&, size_t&);
	size_t processing_http_msg_headers(size_t, size_t);

	// Read and process entity body of HTTP request message.
	void read_http_msg_body(size_t);

	// Send and generate HTTP response message.
	void processing_send_http_response(size_t&, size_t&);
	void send_http_response(enum ProcessingState);
	void generate_simple_http_response(int);

	// For handle errors.
	void handle_asio_error(const boost::system::error_code&);

	// For debug.
	void print_http_request_msg();
	void print_http_response_msg();

	// For server push.
	void server_push_main_loop();

	// For detecting whether current session is SSL or not.
	static bool is_sslv2(const char* data, size_t bytes);
	static bool is_sslv3_or_tlsv1_0_or_tlsv1_1(const char* data, size_t bytes);
	bool is_ssl_protocol();

private:
	/// HTTP request handler.
	boost::shared_ptr<IHttpRequestHandler> http_request_handler_;

	/// The network TCP socket of ASIO.
	boost::asio::ip::tcp::socket tcp_socket_;

	/// The network SSL socket of ASIO.
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket&>* ssl_socket_;

	/// The flag to indicate whether using the SSL socket or not.
	bool is_ssl_;

	/// Buffer to store the received data.
	char buffer_[CONNECTION_BUFFER_SIZE];

	/// Flag of the keep alive of HTTP.
	bool keep_alive_;

	/// HTTP message parser.
	HttpMsgParser http_msg_parser_;

	/// Callbacks for the HTTP message parser.
	HttpMsgParser::HttpMsgParserCallbacks callbacks_;

	/// HTTP request message.
	HttpRequestMsg request_msg_;

	/// HTTP response message.
	HttpResponseMsg response_msg_;

	/// Index to point to the start of unhandled data in the buffer.
	size_t previous_unhandled_data_index_;

	/// Size of unhandled data in the buffer.
	size_t previous_unhandled_data_size_;

	/// Callback function for thread clean up.
	ThreadExitCallback thread_exit_callback_;

	/// Flag to control the main thread loop.
	bool is_running_;

	/// The state to recording the process.
	ProcessingState state_;

	/// For store the latest ASIO error.
	boost::system::error_code latest_error_;

	/// Flag to indicate the body is compressed or not.
	bool is_body_compressed_;

	/// The information about this connection.
	HttpConnectionInfo connection_info_;

	/// The flag to indicate this request expect 100 continue or not.
	bool is_100_continue_;

	/// The flag to indicate the server push mode is enable or not.
	bool is_server_push_enabled_;

	/// The callback function to get the data for server push mode.
	ServerPushDataCallback server_push_data_callback_;

	/// The callback function to tell modules must be interrupted in the "server push data callback". So this connection will not be blocked when this connection will be stopped etc.
	ForceExitPushModeCallback force_exit_push_mode_callback_;
};

} // namespace FBi

#endif
