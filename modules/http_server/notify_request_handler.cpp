#include <http_server/notify_request_handler.hpp>

namespace FBi
{

/**
 * @brief Constructor of http request handler for the notification.
 *
 * @param[in] server_push_callback
 * @param[in] exit_push_mode_callback
 */
NotifyRequestHandler::NotifyRequestHandler(const ServerPushDataCallback& server_push_callback, const ForceExitPushModeCallback& exit_push_mode_callback)
	: server_push_callback_(server_push_callback),
	exit_push_mode_callback_(exit_push_mode_callback)
{
}

NotifyRequestHandler::~NotifyRequestHandler()
{
}

/**
 * @brief Function to handle the partial http entity body.
 *
 * @param[in] request_msg The HTTP request message.
 * @param[out] response_msg The HTTP response message.
 * @param[int] start_index The start index point to the unhandle body in the HTTP request message.
 * @param[in] size The size of unhandle body in the HTTP request message.
 * @param[in] is_end true: If this is the last unhandle body in one HTTP request message. Otherwise, it is false.
 * @param[in] connection_info The information about the connection.
 * @param[in] push_data_connection The connection is used for request handler to set some callbacks and tell HTTP server to change server push mode.
 */
void NotifyRequestHandler::HandlePartialBody(const HttpRequestMsg& /*request_msg*/, HttpResponseMsg& /*response_msg*/, size_t /*start_index*/, size_t /*size*/, bool is_end, const HttpConnectionInfo& /*connection_info*/, INetworkPushDataConnection* push_data_connection)
{
	if(is_end)
	{
		// Register callbacks and enable the server push mode.
		push_data_connection->RegisterServerPushDataCallback(server_push_callback_);
		push_data_connection->RegisterForceExitPushModeCallback(exit_push_mode_callback_);
		push_data_connection->EnableServerPushMode();
	}
}

}
