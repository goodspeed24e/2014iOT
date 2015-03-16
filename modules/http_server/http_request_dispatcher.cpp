#include <http_server/http_request_dispatcher.hpp>
#include <http_server/http_msg.hpp>
#include <cstdio>


namespace FBi
{

/// Constructor of http request dispatcher.
HttpRequestDispatcher::HttpRequestDispatcher()
{
}

/// Destructor of http request dispatcher.
HttpRequestDispatcher::~HttpRequestDispatcher()
{
	request_handler_map_.clear();
}

/**
 * @brief Function to dispatcher messages to each request handler.
 *
 * @param[in] request_msg The HTTP request message.
 * @param[out] response_msg The HTTP response message.
 * @param[int] start_index The start index point to the unhandle body in the HTTP request message.
 * @param[in] size The size of unhandle body in the HTTP request message.
 * @param[in] is_end true: If this is the last unhandle body in one HTTP request message. Otherwise, it is false.
 * @param[in] connection_info The information about the connection.
 * @param[in] push_data_connection The connection is used for request handler to set some callbacks and tell HTTP server to change server push mode.
 */
void HttpRequestDispatcher::HandlePartialBody(const HttpRequestMsg& request_msg, HttpResponseMsg& response_msg, size_t start_index, size_t size, bool is_end, const HttpConnectionInfo& connection_info, INetworkPushDataConnection* push_data_connection)
{
	// Dispatch HTTP request message.
	URLHandlerContainer::iterator iter = request_handler_map_[request_msg.method].find(request_msg.uri.path);
	if( (iter != request_handler_map_[request_msg.method].end()) && (iter->second) )
	{
		iter->second->HandlePartialBody(request_msg, response_msg, start_index, size, is_end, connection_info, push_data_connection);
		return;
	}

	printf("HttpRequestDispatcher::HandlePartialBody => Use not implement request handler. method = %s\n", request_msg.method.c_str());
	URI_Utility::PrintURI(request_msg.uri);
	not_implement_request_handler_.HandlePartialBody(request_msg, response_msg, start_index, size, is_end, connection_info, push_data_connection);
}

/**
 * @brief Interface for register the request handler for each HTTP message.
 *
 * @param[in] method The method of HTTP request message that the register handler want to handler.
 * @param[in] url The URL of HTTP request message that the register handler want to handler.
 * @param[in] request_handler The register request handler.
 */
void HttpRequestDispatcher::RegisterHttpRequestHandler(const std::string& method, const std::string& url, const boost::shared_ptr<IHttpRequestHandler>& request_handler)
{
	URLHandlerContainer::iterator iter = request_handler_map_[method].find(url);

	if(iter != request_handler_map_[method].end())
	{
		printf("HttpRequestDispatcher::RegisterHttpRequestHandler => method = %s, url = %s\n", method.c_str(), url.c_str());

		throw "There is one request handler has already registered.";
	}

	request_handler_map_[method][url] = request_handler;
}

/**
 * @brief Interface for unregister the request handler for each HTTP message.
 *
 * @param[in] method The method of HTTP request message that the register handler want to handler.
 * @param[in] url The URL of HTTP request message that the register handler want to handler.
 */
void HttpRequestDispatcher::UnRegisterHttpRequestHandler(const std::string& method, const std::string& url)
{
	URLHandlerContainer::iterator iter = request_handler_map_[method].find(url);

	if(iter != request_handler_map_[method].end())
	{
		request_handler_map_[method].erase(iter);
	}
}

}
