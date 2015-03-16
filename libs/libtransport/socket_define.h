#ifndef SOCKET_DEFINE_H_
#define SOCKET_DEFINE_H_

#include <libtransport/libtransport_config.h>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace FBi
{

// When we instantiate the template class RPCSession, Windows do not instantiate the template class that we pass into the RPCSession automatically.
// So we need to inheritance the ASIO SSL socket then the compiler will instantiate the ASIO SSL socket in advance.
class LIBTRANSPORT_DECL SSL_SOCK : public boost::asio::ssl::stream<boost::asio::ip::tcp::socket>
{
public:
	SSL_SOCK(boost::asio::io_service& io_service, boost::asio::ssl::context& ssl_context)
		: boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(io_service, ssl_context)
	{
	}
};

typedef boost::asio::ip::tcp::socket TCP_SOCK;

}

#endif