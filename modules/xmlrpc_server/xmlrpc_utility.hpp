#ifndef FBi_XMLRPC_UTILITY_HPP
#define FBi_XMLRPC_UTILITY_HPP

#include <xmlrpc_server/libxmlrpc_server_config.h>

namespace FBi
{

/// The utility class for XMLRPC.
class LIBXMLRPCSERVER_DECL XMLRPC_Utility
{
public:

	/**
	 * @brief Function to get the default error string of XMLRPC.
	 *
	 * @param[in] error_code The default error code of XMLRPC.
	 * @return The default error string correspond with the input error code.
	 */
	static std::string GetXMLRPCErrorString(int error_code)
	{
		switch(error_code)
		{
		case -32700:
			return "Parse error. not well formed. ";
		case -32701:
			return "Parse error. unsupported encoding. ";
		case -32702:
			return "Parse error. invalid character for encoding. ";
		case -32600:
			return "Server error. invalid xml-rpc. not conforming to spec. ";
		case -32601:
			return "Server error. requested method not found. ";
		case -32602:
			return "Server error. invalid method parameters. ";
		case -32603:
			return "Server error. internal xml-rpc error. ";
		case -32500:
			return "Application error. ";
		case -32400:
			return "System error. ";
		case -32300:
			return "Transport error. ";
		default:
			return "";
		}
	}
};

}

#endif

