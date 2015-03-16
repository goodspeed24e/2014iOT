#include <libtransport/method.hpp>
#include <set>
#include <boost/foreach.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::property_tree;

namespace FBi
{

//===========================XMLRPC_Msg_Ptree Class================================================

/**
 * @brief Constructor for XMLRPC message.
 *
 * @param[in] param_path The path to point the <param> tag in the property tree.
 * @param[in] params_path The path to point the <params> tag in the property tree.
 */
XMLRPC_Msg_Ptree::XMLRPC_Msg_Ptree(const std::string& param_path, const std::string& params_path)
	:param_path_(param_path),
	params_path_(params_path)
{

}

/**
 * @brief Function to put the data into the back of stream.
 *
 * @param[in] data The partial data.
 */
void XMLRPC_Msg_Ptree::Parse(const char* data)
{
	m_stream << data ;
}

/**
 * @brief Function to put the data into the back of stream.
 *
 * @param[in] data The partial data.
 * @param[in] size The size of partial data.
 */
void XMLRPC_Msg_Ptree::Parse(const char* data, size_t size)
{
	m_stream.write(data, size);
}

/**
 * @brief Function for translate the data in the stream to property tree.
 *
 * @exception boost::property_tree::xml_parser::xml_parser_error
 */
void XMLRPC_Msg_Ptree::ComposeTree()
{
	// We can call Parse to put the data. Then call ComposeTree to construct the property tree
	// Note: The read_xml function of boost property tree here will throw exception when errors occur.
	boost::property_tree::xml_parser::read_xml(m_stream, m_pt);

	m_stream.clear();
	m_stream.str("");
}

/**
 * @brief Function to get the XML string.
 *
 * @param[in] use_indent true: use indent in the XML, false: will not use indent in the XML.
 * @return The XML string of XMLRPC.
 * @exception boost::property_tree::xml_parser::xml_parser_error
 */
std::string XMLRPC_Msg_Ptree::GetXML(bool use_indent) const
{
	// Note: The write_xml function of boost property tree here will throw exception when errors occur.

#ifdef DEBUG_2
	// We always use indent when we want to debug.
	use_indent = true;
#endif

	std::ostringstream ss;
	if(!use_indent)
	{
		boost::property_tree::xml_parser::write_xml(ss, m_pt);
	}
	else
	{
		// Add well indent when output the XML. For performance, we do not use the indent in the XMLRPC.
		//boost::property_tree::xml_writer_settings<char> xml_settings('\t', 1);
		boost::property_tree::xml_writer_settings<char> xml_settings(' ', 4);
		boost::property_tree::xml_parser::write_xml(ss, m_pt, xml_settings);
	}
	return ss.str();
}

/// Function to clear the stream and property tree.
void XMLRPC_Msg_Ptree::Clear()
{
	m_stream.clear();
	m_stream.str("");
	m_pt.clear();
}

/**
 * @brief Function to get the length of data in the stream.
 *
 * @return The length of data in the stream.
 */
size_t XMLRPC_Msg_Ptree::GetStreamLength() const
{
	return m_stream.str().length();
}

/**
 * @brief Function to add string in the XMLRPC.
 *
 * @param[in] param The string need to be added.
 */
void XMLRPC_Msg_Ptree::AddString(const std::string& param)
{
	boost::property_tree::ptree tmpTree;
	tmpTree.add(boost::property_tree::path("value" PT_SEP_STR "string", PT_SEP_CH), param);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), tmpTree);
}

/**
 * @brief Function to add integer (i4) in the XMLRPC.
 *
 * @param[in] param The integer need to be added.
 */
void XMLRPC_Msg_Ptree::AddInteger(int param)
{
	boost::property_tree::ptree tmpTree;
	tmpTree.add(boost::property_tree::path("value" PT_SEP_STR "i4", PT_SEP_CH), param);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), tmpTree);
}

/**
 * @brief Function to add boolean in the XMLRPC.
 *
 * @param[in] param The boolean need to be added.
 */
void XMLRPC_Msg_Ptree::AddBool(bool param)
{
	boost::property_tree::ptree tmpTree;
	tmpTree.add(boost::property_tree::path("value" PT_SEP_STR "boolean", PT_SEP_CH), (int)param);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), tmpTree);	
}

/**
 * @brief Function to add double in the XMLRPC.
 *
 * @param[in] param The double need to be added.
 */
void XMLRPC_Msg_Ptree::AddDouble(double param)
{
	boost::property_tree::ptree tmpTree;
	tmpTree.add(boost::property_tree::path("value" PT_SEP_STR "double", PT_SEP_CH), param);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), tmpTree);
}

/**
 * @brief Function to add struct in the XMLRPC. (Note: The struct can not contain array now.)
 *
 * @param[in] container The struct (Properties) need to be added.
 */
void XMLRPC_Msg_Ptree::AddProperties(const Properties& container)
{
	boost::property_tree::ptree temp_tree;
	boost::property_tree::ptree temp_sub_tree;

	container.Export(temp_sub_tree);
	temp_tree.add_child(boost::property_tree::path("value", PT_SEP_CH), temp_sub_tree);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), temp_tree);

}

/**
 * @brief Function to add array in the XMLRPC.
 *
 * @param[in] array The array (XMLRPCArray) need to be added.
 */
void XMLRPC_Msg_Ptree::AddArray(const XMLRPCArray& array)
{
	boost::property_tree::ptree temp_tree;
	boost::property_tree::ptree temp_sub_tree;

	FBi::Properties::ArrayToPropertyTree(array, temp_sub_tree);
	temp_tree.add_child(boost::property_tree::path("value", PT_SEP_CH), temp_sub_tree);
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), temp_tree);
}

/**
 * @brief Function to add date and time in the XMLRPC.
 *
 * @param[in] param The date and time need to be added.
 */
void XMLRPC_Msg_Ptree::AddDateTime(const XMLRPCDateTime& time)
{
	boost::property_tree::ptree tmpTree;
	tmpTree.add(boost::property_tree::path("value" PT_SEP_STR "dateTime.iso8601", PT_SEP_CH), time.GetDateTimeString());
	m_pt.add_child(boost::property_tree::path(param_path_, PT_SEP_CH), tmpTree);
}

/**
 * @brief Function to add value in the XMLRPC.
 *
 * @param[in] value The value need to be added.
 * @return true: If we can add value. Otherwise, it is false.
 */
bool XMLRPC_Msg_Ptree::AddValue(const Properties::ValueType& value)
{
	if(value.IsString())
	{
		this->AddString(value);
		return true;
	}

	if(value.IsInteger())
	{
		this->AddInteger(value);
		return true;
	}

	if(value.IsProperties())
	{
		this->AddProperties(value);
		return true;
	}

	if(value.IsXMLRPCArray())
	{
		this->AddArray(value);
		return true;
	}

	if(value.IsBool())
	{
		this->AddBool(value);
		return true;
	}

	if(value.IsDouble())
	{
		this->AddDouble(value);
		return true;
	}

	if(value.IsXMLRPCDateTime())
	{
		this->AddDateTime(value);
		return true;
	}

	return false;
}

/**
 * @brief Function to get the value from the <value> tag of XMLRPC in the property tree.
 *
 * @param[in] iter The iterator point to the <value> tag.
 * @exception
 */
Properties::ValueType XMLRPC_Msg_Ptree::get_rpc_value(boost::property_tree::ptree::const_iterator& iter)
{
	// Note: The get function of boost property tree here will throw exception when errors occur.
	if (iter->first == "struct")
	{
		FBi::Properties s1;
		FBi::Properties::PropertyTreeToProperties(iter->second, s1);
		return s1;
	}
	if (iter->first == "string")
	{
		return iter->second.get<std::string>(boost::property_tree::path("", PT_SEP_CH));
	}
	if (iter->first == "i4")
	{
		return iter->second.get<int>(boost::property_tree::path("", PT_SEP_CH));
	}
	if (iter->first == "int")
	{
		return iter->second.get<int>(boost::property_tree::path("", PT_SEP_CH));
	}
	if (iter->first == "boolean")
	{
		return iter->second.get<bool>(boost::property_tree::path("", PT_SEP_CH));
	}
	if (iter->first == "double")
	{
		return iter->second.get<double>(boost::property_tree::path("", PT_SEP_CH));
	}
	if(iter->first == "array")
	{
		XMLRPCArray array;
		FBi::Properties::PropertyTreeToArray(iter->second, array);
		return array;
	}
	if (iter->first == "dateTime.iso8601")
	{
		std::string str = iter->second.get<std::string>(boost::property_tree::path("", PT_SEP_CH));
		XMLRPCDateTime date_time;
		date_time.SetDateTime(str);

		return date_time;
	}

	return Properties::ValueType();
}

/**
 * @brief Function to get the parameters from the method call or method response of XMLRPC.
 *
 * @param[out] vector The output vector which contains parameters.
 * @return true: If we find all parameters. Otherwise, it is false.
 */
bool XMLRPC_Msg_Ptree::GetParameters(std::vector<FBi::Properties::ValueType>& vector)
{
	// Clear vector
	vector.clear();

	try
	{
		boost::property_tree::ptree& params_tree = m_pt.get_child(boost::property_tree::path(params_path_, PT_SEP_CH));
		try
		{
			BOOST_FOREACH(ptree::value_type &v, params_tree)
			{
				ptree::const_iterator value_iter = v.second.get_child(boost::property_tree::path("value", PT_SEP_CH)).begin();

				vector.push_back(get_rpc_value(value_iter));
			}
		} catch (...)
		{
			// We can not find the tree of value of parameters or other errors occurs.
			return false;
		}
	}
	catch (boost::property_tree::ptree_bad_path)
	{
		// In this case, we can't find the tree of parameters. So there is no parameters here.
		// We return true and empty vector.
		return true;
	}
	catch(...)
	{
		return false;
	}

        return true;
}
//===========================MethodCall Class================================================

/// Constructor of MethodCall of XMLRPC.
MethodCall::MethodCall()
	:XMLRPC_Msg_Ptree("methodCall" PT_SEP_STR "params" PT_SEP_STR "param", "methodCall" PT_SEP_STR "params")
{
}

/**
 * @brief Function to get the method name from the method call.
 *
 * @return The name of method.
 */
std::string MethodCall::GetMethodName()
{
	return m_pt.get(boost::property_tree::path("methodCall" PT_SEP_STR "methodName", PT_SEP_CH), "");
}

/**
 * @brief Function to set the method name of the method call.
 *
 * @param[in] str_method The name of method.
 */
void MethodCall::SetMethodName(const std::string& str_method)
{
	m_pt.put(boost::property_tree::path("methodCall" PT_SEP_STR "methodName", PT_SEP_CH), str_method);
	m_pt.put(boost::property_tree::path("methodCall" PT_SEP_STR "params", PT_SEP_CH), "");
}
//===========================MethodResponse Class================================================

/// Constructor of MethodResponse of XMLRPC.
MethodResponse::MethodResponse()
	:XMLRPC_Msg_Ptree("methodResponse" PT_SEP_STR "params" PT_SEP_STR "param", "methodResponse" PT_SEP_STR "params")
{
	m_pt.put(boost::property_tree::path("methodResponse" PT_SEP_STR "params", PT_SEP_CH), "");
}

/**
 * @brief Function to set the fault code and fault string in the XMLRPC.
 *
 * @param[in] fault_code The fault code of XMLRPC.
 * @param[in] fault_str The fault string of XMLRPC.
 */
void MethodResponse::SetFault(int fault_code, const std::string& fault_str)
{
	this->Clear();
	m_pt.put(boost::property_tree::path("methodResponse" PT_SEP_STR "fault" PT_SEP_STR "value" PT_SEP_STR "struct" PT_SEP_STR "member" PT_SEP_STR "name", PT_SEP_CH), "faultCode");
	m_pt.put(boost::property_tree::path("methodResponse" PT_SEP_STR "fault" PT_SEP_STR "value" PT_SEP_STR "struct" PT_SEP_STR "member" PT_SEP_STR "value" PT_SEP_STR "i4", PT_SEP_CH), fault_code);

	boost::property_tree::ptree tmpTree;
	tmpTree.put(boost::property_tree::path("name", PT_SEP_CH), "faultString");
	tmpTree.put(boost::property_tree::path("value" PT_SEP_STR "string", PT_SEP_CH), fault_str);
	m_pt.add_child(boost::property_tree::path("methodResponse" PT_SEP_STR "fault" PT_SEP_STR "value" PT_SEP_STR "struct" PT_SEP_STR "member", PT_SEP_CH), tmpTree);
}

/**
 * @brief Function to determine the method response is regular response or fault.
 *
 * @return true: If it is regular response. false: If it is fault response.
 */
bool MethodResponse::IsOK()
{
	try
	{
		m_pt.get_child(boost::property_tree::path("methodResponse" PT_SEP_STR "fault", PT_SEP_CH));
	}
	catch(...)
	{
		return true;
	}
	return false;
}

/**
 * @brief Function to get the fault string from the method response. 
 *
 * @return Fault string.
 */
string MethodResponse::GetErrMsg()
{
	try{
		BOOST_FOREACH(const boost::property_tree::ptree::value_type& v,  m_pt.get_child(boost::property_tree::path("methodResponse" PT_SEP_STR "fault" PT_SEP_STR "value" PT_SEP_STR "struct", PT_SEP_CH)))
		{
			std::string msg = v.second.get(boost::property_tree::path("value" PT_SEP_STR "string", PT_SEP_CH), string());
			if (!msg.empty())
				return msg;
		}
	}
	catch(...)
	{}

	return "";
}

/**
 * @brief Function to get the fault code from the method response.
 *
 * @return Fault code.
 */
int MethodResponse::GetErrValue()
{
	try {

		BOOST_FOREACH(const boost::property_tree::ptree::value_type& v,  m_pt.get_child(boost::property_tree::path("methodResponse" PT_SEP_STR "fault" PT_SEP_STR "value" PT_SEP_STR "struct", PT_SEP_CH)))
		{
			int val = v.second.get(boost::property_tree::path("value" PT_SEP_STR "i4", PT_SEP_CH), 0);
			if (val)
				return val;
		}
	}
	catch(...)
	{}

	return 0;
}

/**
 * @brief Function to get the value of the first parameter in the method response.
 *
 * @return The value of the first parameter (Properties).
 */
Properties::ValueType MethodResponse::GetResult()
{
	try
	{
		ptree::const_iterator v = m_pt.get_child(boost::property_tree::path("methodResponse" PT_SEP_STR "params" PT_SEP_STR "param" PT_SEP_STR "value", PT_SEP_CH)).begin();
		return get_rpc_value(v);
	}
	catch(...)
	{}
	return Properties::ValueType();
}

} // namespace FBi

