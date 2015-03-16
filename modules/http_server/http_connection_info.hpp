#ifndef HTTP_CONNECTION_INFO_HPP
#define HTTP_CONNECTION_INFO_HPP

#include <http_server/libhttp_server_config.h>

namespace FBi
{

class LIBHTTPSERVER_DECL HttpConnectionInfo
{
public:
	HttpConnectionInfo() : remote_port(0) {}
public:
	unsigned short remote_port;
	std::string remote_address;
};

} // namespace FBi

#endif

