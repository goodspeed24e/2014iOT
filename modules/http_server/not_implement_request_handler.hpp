#ifndef NOT_IMPLEMENT_REQUEST_HANDLER_HPP
#define NOT_IMPLEMENT_REQUEST_HANDLER_HPP

#include <http_server/libhttp_server_config.h>

#include <http_server/http_request_handler_if.hpp>

namespace FBi
{

/// Not implement request handler for the HTTP server.
class LIBHTTPSERVER_DECL NotImplementRequestHandler : public IHttpRequestHandler
{
public:
	NotImplementRequestHandler();
	~NotImplementRequestHandler();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, bool, const HttpConnectionInfo&, INetworkPushDataConnection*);
};

}

#endif

