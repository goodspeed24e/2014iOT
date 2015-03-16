#ifndef XMLRPC_COMMON_DEF_HPP
#define XMLRPC_COMMON_DEF_HPP

#include <xmlrpc_server/libxmlrpc_server_config.h>

#include <libtransport/method.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <http_server/http_connection_info.hpp>

namespace FBi
{

typedef std::vector<FBi::Properties::ValueType> XMLRPC_Value_List;
typedef boost::function<void (const XMLRPC_Value_List&, FBi::MethodResponse&, const HttpConnectionInfo&)> XMLRPC_Method_Callback;

/// Struct to store the information of XMLRPC method.
struct LIBXMLRPCSERVER_DECL XMLRPC_Method_Info
{
	XMLRPC_Method_Callback callback;
	std::string output_signature;
	std::vector<std::string> input_signature;
	std::string method_help;
	bool is_public;
};

typedef std::map<std::string, XMLRPC_Method_Info> MethodCallMap;
typedef std::map<std::string, XMLRPC_Method_Info>::iterator MethodCallMapIter;
typedef std::map<std::string, XMLRPC_Method_Info>::const_iterator MethodCallMapConstIter;

}

#endif

