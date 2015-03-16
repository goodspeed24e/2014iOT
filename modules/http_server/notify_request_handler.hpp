#ifndef NOTIFY_REQUEST_HANDLER_HPP
#define NOTIFY_REQUEST_HANDLER_HPP

#include <http_server/libhttp_server_config.h>

#include <http_server/http_request_handler_if.hpp>
#include <http_server/network_connection_if.hpp>

namespace FBi
{

/// Notify request handler for the HTTP server.
class LIBHTTPSERVER_DECL NotifyRequestHandler : public IHttpRequestHandler
{
public:
	NotifyRequestHandler(const ServerPushDataCallback&, const ForceExitPushModeCallback&);
	~NotifyRequestHandler();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, bool, const HttpConnectionInfo&, INetworkPushDataConnection*);

private:
	/// The callback function to getting the data for server push mode.
	ServerPushDataCallback server_push_callback_;

	/// The callback function to force exit the server push mode.
	ForceExitPushModeCallback exit_push_mode_callback_;
};

}

#endif

