#ifndef RPC_SESSION_HPP_
#define RPC_SESSION_HPP_

#include <libtransport/libtransport_config.h>
#include <libcommon/service_engine.hpp>
#include <libtransport/socket_define.h>
#include <boost/asio/ssl.hpp>
#include <libtransport/method.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition_variable.hpp>
#include <list>

namespace FBi
{

typedef boost::function<void(const MethodResponsePtr&)> RPCSessionFuncCallback;

template<typename T>
class LIBTRANSPORT_DECL RPCSession
{
	template<typename F>
	friend void HandleStage(RPCSession<F>*);
public:

	RPCSession(const std::string& host, int port, bool keep_alive, bool compression = true);
	~RPCSession();

	void DelayCall(const MethodCallPtr& call, const RPCSessionFuncCallback& callback, int delay);
	RPCSessionFuncCallback* Call(const MethodCallPtr& call, const RPCSessionFuncCallback& callback);
	MethodResponsePtr  Call(const MethodCallPtr& method);
	void* Call(const MethodCallPtr& call, bool getResp);
	MethodResponsePtr GetResponse(void* ticket);

	std::string get_host(){return this->remote_host_;}
	std::string get_host_port(){return this->remote_port_;}
	void SetTimeoutValue(unsigned int);

protected:
	void blocking_callback(const MethodResponsePtr&);
	bool check_connect();
	void handle_connect(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_ssl_handshake(const boost::system::error_code&);
	void do_request();
	bool check_request();
	void read_http_response(const boost::system::error_code&);
	void handle_read_status_line(const boost::system::error_code&, std::size_t);
	void parse_header(const boost::system::error_code&, std::size_t);
	void parse_response_tree(const boost::system::error_code&);
	void handle_error(const boost::system::error_code&);
	void handle_timeout(const boost::system::error_code&);
	void delay_call(const boost::system::error_code&, const MethodCallPtr&, const RPCSessionFuncCallback&, const boost::shared_ptr<boost::asio::deadline_timer>&);
	
	//non-thread-safe function
	void do_async_call();
	void async_call();

	boost::asio::io_service& service_;	
	T* sock_;
	boost::asio::deadline_timer timer_;
	boost::mutex mutex_;
	std::list<std::pair<MethodCallPtr, RPCSessionFuncCallback> > m_callback_queue;
	std::deque<std::pair<boost::condition_variable*, MethodResponsePtr> > m_blocking_queue;
	std::map<boost::condition_variable*, MethodResponsePtr>  response_map_;
	std::map<boost::asio::deadline_timer*, boost::shared_ptr<boost::asio::deadline_timer> > delay_timer_map_;
	RPCSessionFuncCallback m_BlockingFunction;

	std::string remote_host_;
	std::string remote_port_;
	std::string call_buf_;
		
	char buffer_[1024];
	char* pos_;
	std::size_t buffer_len_;
	std::size_t body_len_;
	unsigned int timeout_;

	bool active_;
	bool running_;
	bool keep_alive_;

	/// Flag to indicate the entity body of HTTP response message is encoded or not.
	bool is_body_encoded_;
	/// The flag to control using compression in HTTP message or not.
	bool compression_;
	MethodResponsePtr method_resp_;
	std::string encoded_data_;
	sb_allocator allocator_;
};

typedef boost::function<void(const char*, size_t)> NotifySessionCallback;

class LIBTRANSPORT_DECL NotifySession
{
public:
	NotifySession(const std::string& host, int port, const NotifySessionCallback&);
	~NotifySession();
	void Start(const std::string& path);
	void Stop();
private:
	void start_connect();
	void handle_connect(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_ssl_handshake(const boost::system::error_code&);
	void read_http_response(const boost::system::error_code&);
	void handle_read_status_line(const boost::system::error_code&, std::size_t);
	void handle_body(const boost::system::error_code&);
	void parse_header(const boost::system::error_code&, std::size_t);
	void handle_error(const boost::system::error_code&);

	boost::asio::io_service& service_;	
	SSL_SOCK* sock_;
	NotifySessionCallback callback_;
	boost::mutex mutex_;
	std::string remote_host_;
	std::string remote_port_;
		
	char buffer_[1024];
	char* pos_;
	size_t buffer_len_;
	size_t body_len_;
	std::string path_;
	std::string resp_data_;

	bool active_;
	bool running_;
	sb_allocator allocator_;
};
	
}

#endif
