#include <http_server/not_implement_request_handler.hpp>
#include <http_server/http_msg.hpp>

namespace FBi
{

/// Constructor of http request handler for the not implement case.
NotImplementRequestHandler::NotImplementRequestHandler()
{
}

NotImplementRequestHandler::~NotImplementRequestHandler()
{
}

/**
 * @brief Function to handle the partial http entity body (Set the "501 Not implemented" for the HTTP response message).
 *
 * @param[in] request_msg The HTTP request message.
 * @param[out] response_msg The HTTP response message.
 * @param[int] start_index The start index point to the unhandle body in the HTTP request message.
 * @param[in] size The size of unhandle body in the HTTP request message.
 * @param[in] is_end true: If this is the last unhandle body in one HTTP request message. Otherwise, it is false.
 * @param[in] connection_info The information about the connection.
 * @param[in] push_data_connection The connection is used for request handler to set some callbacks and tell HTTP server to change server push mode.
 */
void NotImplementRequestHandler::HandlePartialBody(const HttpRequestMsg& /*request_msg*/, HttpResponseMsg& response_msg, size_t /*start_index*/, size_t /*size*/, bool is_end, const HttpConnectionInfo& /*connection_info*/, INetworkPushDataConnection* /*push_data_connection*/)
{
	if(is_end)
	{
		// We do not implement other funtions now.
		// So we set "501 Not implemented"
		HttpMsgUtility::SetDefaultStatusCode(response_msg, 501);
		response_msg.entity_body.clear();
	}
}

}
