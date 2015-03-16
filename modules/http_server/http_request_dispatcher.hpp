#ifndef HTTP_REQUEST_DISPATCHER_HPP
#define HTTP_REQUEST_DISPATCHER_HPP

#include <http_server/libhttp_server_config.h>

#include <http_server/http_request_handler_if.hpp>
#include <http_server/not_implement_request_handler.hpp>
#include <unordered_map>

namespace FBi
{

/// HTTP request dispatcher for the HTTP server.
class LIBHTTPSERVER_DECL HttpRequestDispatcher : public IHttpRequestDispatcher
{
public:
	typedef std::unordered_map< std::string, boost::shared_ptr<IHttpRequestHandler> > URLHandlerContainer;
	typedef std::unordered_map< std::string, URLHandlerContainer > RequestHandlerContainer;
public:
	HttpRequestDispatcher();
	~HttpRequestDispatcher();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, bool, const HttpConnectionInfo&, INetworkPushDataConnection*);
	void RegisterHttpRequestHandler(const std::string& method, const std::string& url, const boost::shared_ptr<IHttpRequestHandler>& request_handler);
	void UnRegisterHttpRequestHandler(const std::string& method, const std::string& url);

private:
	RequestHandlerContainer request_handler_map_;
	NotImplementRequestHandler not_implement_request_handler_;
};

}

#endif

