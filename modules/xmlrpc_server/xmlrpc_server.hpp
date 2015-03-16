#ifndef FBi_XMLRPC_SERVER_HPP
#define FBi_XMLRPC_SERVER_HPP

#include <xmlrpc_server/libxmlrpc_server_config.h>

#include <boost/noncopyable.hpp>
#include <xmlrpc_server/xmlrpc_common_def.hpp>
#include <xmlrpc_server/xmlrpc_fault.hpp>
#include <http_server/server_if.hpp>
#include <http_server/http_request_handler_if.hpp>

namespace FBi
{

class LIBXMLRPCSERVER_DECL XMLRPC_Server : public IServer, private boost::noncopyable
{
public:
	XMLRPC_Server(const boost::shared_ptr<IServer>&, const boost::shared_ptr<IHttpRequestDispatcher>&);
	~XMLRPC_Server();

	void StartServer();
	void StopServer();
	void RegisterMeetMaxConnectionCallback(const MeetMaxConnectionNumCallback& callback);

	void AddMethod(const XMLRPC_Method_Callback&, const std::string&, const std::string&, const std::string&, const std::string&, bool = true);
	void RemoveMethod(const std::string&);

private:
	void signature_tokenizer(const std::string&, std::vector<std::string>&);
	void internal_add_method(const XMLRPC_Method_Callback&, const std::string&, const std::string&, const std::string&, const std::string&, bool);
	void add_introspective_methods();

	void system_list_methods(const XMLRPC_Value_List&, MethodResponse&);
	void system_method_signature(const XMLRPC_Value_List&, MethodResponse&);
	void system_method_help(const XMLRPC_Value_List&, MethodResponse&);
private:
	/// HTTP server.
	boost::shared_ptr<IServer> http_server_;

	/// XMLRPC method call map.
	MethodCallMap method_call_map_;
};

} // namespace FBi

#endif
