#ifndef FBi_SERVER_IF_HPP
#define FBi_SERVER_IF_HPP

#include <http_server/libhttp_server_config.h>
#include <boost/asio/io_service.hpp>
#include <boost/function.hpp>

namespace FBi
{

typedef boost::function< void() > MeetMaxConnectionNumCallback;

/// Abstract interfaces for server.
class LIBHTTPSERVER_DECL IServer
{
public:
	/// For the destrcutor of child can be called correctly.
	virtual ~IServer() {}

	/// Interface to start server
	virtual void StartServer() = 0;

	/// Interface to stop server
	virtual void StopServer() = 0;

	/**
	 * @brief Interface to register the callback function for the max connection number fit the limitation.
	 *
	 * @param[in] callback The callback function to be registered.
	 */
	virtual void RegisterMeetMaxConnectionCallback(const MeetMaxConnectionNumCallback& callback) = 0;
};

}

#endif

