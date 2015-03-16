#ifndef INSTEK_XMLRPC_FAULT_HPP
#define INSTEK_XMLRPC_FAULT_HPP

#include <xmlrpc_server/libxmlrpc_server_config.h>
#include <string>

namespace FBi
{

/// Fault class for XMLRPC server. So we can throw this class in the method.
class LIBXMLRPCSERVER_DECL XMLRPC_Fault
{
public:
	/**
	 * @brief Constructor of fault for the XMLRPC.
	 *
	 * @param[in] code The fault code of XMLRPC.
	 * @param[in] str The fault string of XMLRPC.
	 */     
	XMLRPC_Fault(int code, const std::string& str): fault_code(code), fault_string(str) {};

	/// The fault code of XMLRPC.
	int fault_code;
	/// The fault string of XMLRPC.
	std::string fault_string;
};

}

#endif
