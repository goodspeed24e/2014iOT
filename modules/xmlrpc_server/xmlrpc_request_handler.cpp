#include <xmlrpc_server/xmlrpc_request_handler.hpp>
#include <boost/exception/all.hpp>
#include <exception>
#include <xmlrpc_server/xmlrpc_utility.hpp>
#include <xmlrpc_server/xmlrpc_fault.hpp>

namespace FBi
{

/**
 * @brief Constructor of http request handler for the XMLRPC.
 *
 * @param[in] method_call_map The mapping table of the method call in the XMLRPC.
 */
XMLRPC_RequestHandler::XMLRPC_RequestHandler(const MethodCallMap& method_call_map)
	: method_call_map_(method_call_map)
{
}

XMLRPC_RequestHandler::~XMLRPC_RequestHandler()
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
void XMLRPC_RequestHandler::HandlePartialBody(const HttpRequestMsg& request_msg, HttpResponseMsg& response_msg, size_t /*start_index*/, size_t /*size*/, bool is_end, const HttpConnectionInfo& connection_info, INetworkPushDataConnection* /*push_data_connection*/)
{
	if(is_end)
	{
		// For XMLRPC
		MethodCall method_call;

		try
		{
			method_call.Parse(request_msg.entity_body.c_str(), request_msg.entity_body.length());
			method_call.ComposeTree();

			// XMLRPC
			call_xmlrpc_method(method_call, response_msg, connection_info);
		}
		catch(boost::property_tree::xml_parser::xml_parser_error err)
		{
			// Can not parse XML!
			MethodResponse method_response;
			std::string err_str = XMLRPC_Utility::GetXMLRPCErrorString(-32700);
			err_str += err.what();
			method_response.SetFault(-32700, err_str);

			set_xmlrpc_http_response(method_response, response_msg, 200);
			return;
		}
	}
}

/**
 * @brief Function to search the mapping table and call the XMLRPC method.
 *
 * @param[in] method_call The XMLRPC method call.
 * @param[out] response_msg The HTTP response messasge.
 */
void XMLRPC_RequestHandler::call_xmlrpc_method(MethodCall& method_call, HttpResponseMsg& response_msg, const HttpConnectionInfo& connection_info)
{
	MethodCallMapConstIter iter = method_call_map_.find(method_call.GetMethodName());
	MethodResponse method_response;
	if(iter != method_call_map_.end())
	{
		XMLRPC_Value_List params_vector;
		method_call.GetParameters(params_vector);
		try
		{
			iter->second.callback(params_vector, method_response, connection_info);
			set_xmlrpc_http_response(method_response, response_msg, 200);
		}
		catch(FBi::XMLRPC_Fault& fault)
		{
			// XMLRPC method call return fault.
			method_response.SetFault(fault.fault_code, fault.fault_string);
			set_xmlrpc_http_response(method_response, response_msg, 200);
		}
		catch(boost::exception& e)
		{
			// Application error
			std::string err_str = XMLRPC_Utility::GetXMLRPCErrorString(-32500);
			err_str +=  boost::diagnostic_information(e);
			method_response.SetFault(-32500, err_str);
			set_xmlrpc_http_response(method_response, response_msg, 200);
		}
		catch(std::exception& e)
		{
			// Application error
			std::string err_str = XMLRPC_Utility::GetXMLRPCErrorString(-32500);
			err_str += e.what();
			method_response.SetFault(-32500, err_str);
			set_xmlrpc_http_response(method_response, response_msg, 200);
		}
		catch(...)
		{
			// Application error
			method_response.SetFault(-32500, XMLRPC_Utility::GetXMLRPCErrorString(-32500));
			set_xmlrpc_http_response(method_response, response_msg, 200);
			return;
		}
	}
	else
	{
		// Can not find method call
		method_response.SetFault(-32601, XMLRPC_Utility::GetXMLRPCErrorString(-32601));
		set_xmlrpc_http_response(method_response, response_msg, 200);
		return;
	}
}

/**
 * @brief Function to combine the response of XMLRPC method with the HTTP response message.
 *
 * @param[in] The response of the XMLRPC method.
 * @param[out] The output HTTP response message.
 * @param[in] The HTTP status code in the response message.
 */
void XMLRPC_RequestHandler::set_xmlrpc_http_response(MethodResponse& method_response, HttpResponseMsg& response_msg, int http_status_code)
{
	response_msg.headers["Content-Type"] = "text/xml";
	HttpMsgUtility::SetDefaultStatusCode(response_msg, http_status_code);
	response_msg.entity_body = method_response.GetXML();

	response_msg.headers["Server"] = "FBi-XMLRPC-Server";
}

}
