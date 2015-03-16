#ifndef FBi_NETWORK_CONNECTION_IF_HPP
#define FBi_NETWORK_CONNECTION_IF_HPP

#include <string>
#include <map>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>

#include <http_server/libhttp_server_config.h>

#include <http_server/uri.hpp>

namespace FBi
{

typedef boost::function<void(const boost::system::error_code&)> AcceptorCallback;
typedef boost::function<void(void)> ThreadExitCallback;

/// The ServerPushDataCallback callback function type. Parameters: 1) The URI query string. 2) The flag to indicate whether it's first call or not. 3) The MIME headers. 4) The body. 5) The flag to indicate whether it is end of push mode.
typedef boost::function<void(const URI_Struct::Query_Str_Container& query_strings, bool not_first_call, std::map<std::string, std::string>& output_headers, std::string& output_body, bool& is_end)> ServerPushDataCallback;
/// The ForceExitPushModeCallback callback function type. Parameters: 1) The URI query string.
typedef boost::function<void(const URI_Struct::Query_Str_Container& query_strings)> ForceExitPushModeCallback;

/// Abstract interface for the network connection when the server push mode is enabled.
class LIBHTTPSERVER_DECL INetworkPushDataConnection
{
public:
	/// For the destructor of child can be called correctly.
	virtual ~INetworkPushDataConnection() {}

	/**
	 * @brief Function to register the ServerPushDataCallback function.
	 *
	 * @param[in] callback The callback function to be registered. It will be called when server push mode is enabled.
	 */
	virtual void RegisterServerPushDataCallback(const ServerPushDataCallback& callback) = 0;

	/**
	 * @brief Function to register the ForceExitPushModeCallback function.
	 *
	 * @param[in] callback The callback function to be registered. It will be called when this connection will be stopped.
	 */
	virtual void RegisterForceExitPushModeCallback(const ForceExitPushModeCallback& callback) = 0;

	/// Function to enable server push mode.
	virtual void EnableServerPushMode() = 0;
};

/// Abstract interface for the network connection.
class LIBHTTPSERVER_DECL INetworkConnection
{
public:
	/// For the destructor of child can be called correctly.
	virtual ~INetworkConnection() {}

	/// Interface to make the connection to enter the main loop to do works.
	virtual void StartConnection() = 0;

	/// Interface to make the connection to exit the main loop.
	virtual void StopConnection() = 0;

	/**
	 * @brief Interface for setting the callback function. 
	 *
	 * @param[in] callback The callback function will be called, when thread prepare to exit.
	 * Note: We don't use the at_thread_exit() here. Becasue we want to control it ourselves.
	 */
	virtual void SetThreadExitCallback(const ThreadExitCallback& /*callback*/) {}
};

} // namespace FBi

#endif
