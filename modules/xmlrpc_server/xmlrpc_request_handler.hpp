#ifndef XMLRPC_REQUEST_HANDLER_HPP
#define XMLRPC_REQUEST_HANDLER_HPP

#include <xmlrpc_server/libxmlrpc_server_config.h>

#include <http_server/http_request_handler_if.hpp>
#include <xmlrpc_server/xmlrpc_common_def.hpp>

namespace FBi
{

/// XMLRPC request handler for the HTTP server.
class LIBXMLRPCSERVER_DECL XMLRPC_RequestHandler : public IHttpRequestHandler
{
public:
	XMLRPC_RequestHandler(const MethodCallMap&);
	~XMLRPC_RequestHandler();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, bool, const HttpConnectionInfo&, INetworkPushDataConnection*);

private:
	// For XMLRPC
	void call_xmlrpc_method(MethodCall&, HttpResponseMsg&, const HttpConnectionInfo&);
	void set_xmlrpc_http_response(MethodResponse&, HttpResponseMsg&, int);

private:
	/// Reference to the map of the XMLRPC method call.
	const MethodCallMap& method_call_map_;

};

}

#endif

