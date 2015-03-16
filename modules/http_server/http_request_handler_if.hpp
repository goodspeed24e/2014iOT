#ifndef HTTP_REQUEST_HANDLER_IF_HPP
#define HTTP_REQUEST_HANDLER_IF_HPP

#include <http_server/libhttp_server_config.h>

#include <http_server/http_msg.hpp>
#include <http_server/http_connection_info.hpp>
#include <http_server/network_connection_if.hpp>
#include <boost/shared_ptr.hpp>

namespace FBi
{

/// Abstract interfaces for the HTTP request handler.
class LIBHTTPSERVER_DECL IHttpRequestHandler
{
public:
	virtual ~IHttpRequestHandler(){}

	/**
	 * @brief Interface for request handler to handle the 
	 * partial body (chunked etc.).
	 *
	 * @param[in] request HTTP request message.
	 * @param[out] response HTTP response message.
	 * @param[in] start_index The index point to the start index 
	 * of the unhandled body in the HTTP request message.
	 * @param[in] size The size of the unhandled body in the HTTP request message.
	 * @param[in] is_end (true)If it is last unhandled body. Otherwise, the value is false.
	 * @param[in] connection_info The information about the connection.
	 * @param[in] push_data_connection The connection is used for request handler to set some callbacks and tell HTTP server to change to server push mode.
	 */
	virtual void HandlePartialBody(const HttpRequestMsg& request,
			HttpResponseMsg& response,
			size_t start_index,
			size_t size,
			bool is_end,
			const HttpConnectionInfo& connection_info,
			INetworkPushDataConnection* push_data_connection) = 0;
};


/// Abstract interfaces for the HTTP request dispatcher.
class LIBHTTPSERVER_DECL IHttpRequestDispatcher : public IHttpRequestHandler
{
public:
	virtual ~IHttpRequestDispatcher(){}

	/**
	 * @brief Interface for register the request handler for each HTTP message.
	 *
	 * @param[in] method The method of HTTP request message that the register handler want to handler.
	 * @param[in] url The URL of HTTP request message that the register handler want to handler.
	 * @param[in] request_handler The register request handler.
	 */
	virtual void RegisterHttpRequestHandler(const std::string& method, const std::string& url, const boost::shared_ptr<IHttpRequestHandler>& request_handler) = 0;

	/**
	 * @brief Interface for unregister the request handler for each HTTP message.
	 *
	 * @param[in] method The method of HTTP request message that the register handler want to handler.
	 * @param[in] url The URL of HTTP request message that the register handler want to handler.
	 */
	virtual void UnRegisterHttpRequestHandler(const std::string& method, const std::string& url) = 0;
};

}

#endif

