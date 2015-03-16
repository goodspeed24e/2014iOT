#ifndef FBi_HTTP_SERVER_HPP
#define FBi_HTTP_SERVER_HPP

#include <http_server/libhttp_server_config.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <libcommon/service_engine.hpp>
#include <http_server/http_connection.hpp>
#include <http_server/server_if.hpp>
#include <http_server/ssl_context_info.hpp>

namespace FBi
{

/// HTTP server class
class LIBHTTPSERVER_DECL HttpServer : public IServer, private boost::noncopyable
{
typedef boost::shared_ptr<boost::thread> ThreadPtr;
typedef boost::shared_ptr<HttpConnection> NetworkConnectionPtr;

/// Struct to store the information of thread and connection.
struct ThreadConnection
{
	ThreadPtr thread;
	NetworkConnectionPtr connection;
};

typedef std::map< boost::thread::id, ThreadConnection > ThreadIdMap;
typedef std::map< boost::thread::id, ThreadConnection >::iterator ThreadIdMapIter;

public:
	HttpServer(const boost::shared_ptr<IHttpRequestDispatcher>&, const std::string&, int, const SSLContextInfo&, int = 511);
	~HttpServer();

	void StartServer();
	void StopServer();
	void RegisterMeetMaxConnectionCallback(const MeetMaxConnectionNumCallback& callback);

protected:
	HttpConnection* create_connection();

private:
	void set_ssl_context_info(const SSLContextInfo&, boost::asio::ssl::context&);
	void handle_accept(const boost::system::error_code&);
	void thread_exit_callback_func();
	std::string get_ssl_private_key_password() const;

private:
	/// The HTTP request dispatcher.
	boost::shared_ptr<IHttpRequestDispatcher> http_request_dispatcher_;

	/// The acceptor of ASIO.
	boost::asio::ip::tcp::acceptor acceptor_;

	/// Accept callback function.
	AcceptorCallback accept_callback_;

	/// The maximum connection number.
	int max_connection_num_;

	/// Network connection.
	NetworkConnectionPtr new_connection_;

	/// Thread id, thread and connection  map.
	ThreadIdMap thread_id_map_;

	/// Protect the thread_id_map_ and new_connection_ etc. and also for wait condition.
	boost::mutex mutex_;

	/// Flag to indicate the server is started or not.
	bool is_started_;

	/// Password of private key file for SSL connection.
	std::string ssl_private_key_password_;

	/// The endpoint that server binds.
	boost::asio::ip::tcp::endpoint current_endpoint_;

	/// The flag to indicate the server is handling the "Start Server" now.
	bool is_processing_start_server_;

	/// The counter for the number of connection.
	unsigned int connection_counter_;

	/// The callback function for the max number of connection is met.
	MeetMaxConnectionNumCallback meet_max_connection_callback_;

	///custom memory allocator
	sb_allocator allocator_;
};

} // namespace FBi

#endif
