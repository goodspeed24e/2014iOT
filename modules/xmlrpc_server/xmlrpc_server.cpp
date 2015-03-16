#include <xmlrpc_server/xmlrpc_server.hpp>
#include <boost/tokenizer.hpp>
#include <boost/bind.hpp>
#include <xmlrpc_server/xmlrpc_utility.hpp>
#include <xmlrpc_server/xmlrpc_request_handler.hpp>
#include <iostream>

namespace FBi
{

/**
 * @brief Constructor for XMLRPC server.
 *
 * @param[in] server The HTTP server.
 * @param[in] dispatcher The HTTP request dispatcher.
 */
XMLRPC_Server::XMLRPC_Server(const boost::shared_ptr<IServer>& server, const boost::shared_ptr<IHttpRequestDispatcher>& dispatcher)
	: http_server_(server)
{
	// Register the XMLRPC request handler.
	boost::shared_ptr<IHttpRequestHandler> xmlrpc_request_handler(new XMLRPC_RequestHandler(method_call_map_));
	dispatcher->RegisterHttpRequestHandler("POST", "/RPC2", xmlrpc_request_handler);
	dispatcher->RegisterHttpRequestHandler("POST", "/RPC", xmlrpc_request_handler);
	dispatcher->RegisterHttpRequestHandler("POST", "/", xmlrpc_request_handler);
}

XMLRPC_Server::~XMLRPC_Server()
{
	this->StopServer();
}

/// Function to start the XMLRPC server.
void XMLRPC_Server::StartServer()
{
	http_server_->StartServer();
	add_introspective_methods();
}

/// Function to stop the XMLRPC server.
void XMLRPC_Server::StopServer()
{
	http_server_->StopServer();
}

/**
 * @brief Function to register the callback function for the max connection number fit the limitation.
 *
 * @param[in] callback The callback function to be registered.
 */
void XMLRPC_Server::RegisterMeetMaxConnectionCallback(const MeetMaxConnectionNumCallback& callback)
{
	http_server_->RegisterMeetMaxConnectionCallback(callback);
}

/**
 * @brief Function for register the methods of XMLRPC. It will throw exception when the method name has already existed or the callback function is invalid.
 *
 * @param[in] callback The XMLRPC callback function to register.
 * @param[in] out_signature The signature of output of XMLRPC method for introspection. It allows only one signature of output in the specification of XMLRPC.
 * @param[in] method_name The XMLRPC method name.
 * @param[in] in_signature The signature of input of XMLRPC method for introspection.
 * @param[in] method_help The help information of method for introspection.
 * @param[in] is_public The flag to control the information of method to be shown when introspect. true: The information will be shown.
 * @exception "The method name has already existed or can not find the callback."
 */
void XMLRPC_Server::AddMethod(const XMLRPC_Method_Callback& callback, const std::string& out_signature, const std::string& method_name, const std::string& in_signature, const std::string& method_help, bool is_public)
{
	if((method_call_map_.find(method_name) == method_call_map_.end()) && callback)
	{
		this->internal_add_method(callback, out_signature, method_name, in_signature, method_help, is_public);
	}
	else
	{
		// FixMe:
		// Throw exception here
		// The method name has already existed.
		throw "The method name has already existed or can not find the callback.";
	}
}

/**
 * @brief Function to unregister the XMLRPC method.
 *
 * @param[in] method_name The registered method name when use AddMethod function.
 */
void XMLRPC_Server::RemoveMethod(const std::string& method_name)
{
	// This method should be used carefully in the multi-threaded environment.
	// Because we don't use any lock here.
	MethodCallMapIter iter = method_call_map_.find(method_name);

	if(iter != method_call_map_.end())
	{
		method_call_map_.erase(iter);
	}
}

/**
 * @brief Tokenizer for the output signature (The separator are dot and space).
 *
 * @param[in] signature_str The string of signature.
 * @param[out] signature_vector The output vector of each signature.
 */
void XMLRPC_Server::signature_tokenizer(const std::string& signature_str, std::vector<std::string>& signature_vector)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer_t;
	boost::char_separator<char> sep(", ");
	tokenizer_t tokens(signature_str, sep);

	for(tokenizer_t::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		signature_vector.push_back(*tok_iter);
}

/**
 * @brief Private function for register the introspection methods of XMLRPC.
 *
 * @param[in] callback The XMLRPC callback function to register.
 * @param[in] out_signature The signature of output of XMLRPC method for introspection. It allows only one signature of output in the specification of XMLRPC.
 * @param[in] method_name The XMLRPC method name.
 * @param[in] in_signature The signature of input of XMLRPC method for introspection.
 * @param[in] method_help The help information of method for introspection.
 * @param[in] is_public The flag to control the information of method to be shown when introspect. true: The information will be shown.
 */
void XMLRPC_Server::internal_add_method(const XMLRPC_Method_Callback& callback, const std::string& out_signature, const std::string& method_name, const std::string& in_signature, const std::string& method_help, bool is_public)
{
	method_call_map_[method_name].callback = callback;
	method_call_map_[method_name].output_signature = out_signature;
	signature_tokenizer(in_signature, method_call_map_[method_name].input_signature);
	method_call_map_[method_name].method_help = method_help;
	method_call_map_[method_name].is_public = is_public;
}

/// Function to register the introspection methods (system.listMethods, system.methodSignature and system.methodHelp).
void XMLRPC_Server::add_introspective_methods()
{
	internal_add_method(boost::bind(&XMLRPC_Server::system_list_methods, this, _1, _2), "array", "system.listMethods", "", "This method returns a list of the methods the server has, by name.", true);
	internal_add_method(boost::bind(&XMLRPC_Server::system_method_signature, this, _1, _2), "array", "system.methodSignature", "string", "This method returns a description of the argument format a particular method expects.", true);
	internal_add_method(boost::bind(&XMLRPC_Server::system_method_help, this, _1, _2), "string", "system.methodHelp", "string", "This method returns a text description of a particular method.", true);
}

/**
 * @brief Callback function of system.listMethods.
 *
 * @param[in] vector The XMLRPC parameters.
 * @param[out] resp The return values of this callback.
 */
void XMLRPC_Server::system_list_methods(const XMLRPC_Value_List& /*vector*/, MethodResponse& resp)
{
	MethodCallMapConstIter iter = method_call_map_.begin();
	XMLRPCArray array;
	for(; iter != method_call_map_.end(); iter++)
	{
		if(iter->second.is_public)
		{
			array.push_back(iter->first);
		}
	}

	resp.AddArray(array);
}

/**
 * @brief Callback function of system.methodSignature.
 *
 * @param[in] vector The XMLRPC parameters.
 * @param[out] resp The return values of this callback.
 */
void XMLRPC_Server::system_method_signature(const XMLRPC_Value_List& vector, MethodResponse& resp)
{
	std::string method_name = (std::string)vector.at(0);
	MethodCallMapConstIter iter = method_call_map_.find(method_name);
	if((iter != method_call_map_.end()) && (iter->second.is_public))
	{
		XMLRPCArray array;

		// Add output signature to array
		array.push_back(iter->second.output_signature);

		// Add intput signature to array
		for(size_t i = 0; i < iter->second.input_signature.size(); ++i)
			array.push_back(iter->second.input_signature[i]);

		resp.AddArray(array);
	}
	else
	{
		resp.SetFault(-32602, XMLRPC_Utility::GetXMLRPCErrorString(-32602));
	}

}

/**
 * @brief Callback function of system.methodHelp.
 *
 * @param[in] vector The XMLRPC parameters.
 * @param[out] resp The return values of this callback.
 */
void XMLRPC_Server::system_method_help(const XMLRPC_Value_List& vector, MethodResponse& resp)
{
	std::string method_name = (std::string)vector.at(0);
	MethodCallMapConstIter iter = method_call_map_.find(method_name);
	if((iter != method_call_map_.end()) && (iter->second.is_public))
	{
		resp.AddString(iter->second.method_help);
	}
	else
	{
		resp.SetFault(-32602, XMLRPC_Utility::GetXMLRPCErrorString(-32602));
	}
}

} // namespace FBi
