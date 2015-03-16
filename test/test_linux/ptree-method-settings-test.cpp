#include <iostream>
#include <string>
#include <vector>

#include <libcommon/properties.hpp>
#include <libtransport/method.hpp>

using boost::property_tree::ptree;
using namespace std;


int main(int argc, char **argv)
{
	instek::MethodResponse response;

	// Test fault
	std::cout << "\nFault===================================" << std::endl;
	response.SetFault(4, "Too many parameters.");
	std::cout << response.GetXML() << std::endl;

	std::cout << "faultCode:" << response.GetErrValue() << std::endl;
	std::cout << "faultString:" << response.GetErrMsg() << std::endl;
	std::cout << "isOK:" << response.IsOK() << std::endl;

	// Test parse and compose tree of fault
	std::cout << "\nCompose fault===================================" << std::endl;
	std::cout << response.GetXML() << std::endl;
	std::string temp_xml = response.GetXML();
	response.Clear();
	response.Parse(temp_xml.c_str());
	response.ComposeTree();
	std::cout << response.GetXML() << std::endl;

	// Test copy of fault
/*	std::cout << "\nCopy fault===================================" << std::endl;
	instek::MethodResponse response2;
	response2 = response;
	std::cout << response2.GetXML() << std::endl;*/

	// Test regular response
	std::cout << "\nRegular response===================================" << std::endl;
	response.Clear();
	instek::Properties settings;
	instek::Properties settings2;
	instek::Properties settings3;

	settings2["name"] = "dog";
	settings2["angle"] = 30.32;
	settings["camera1"] = settings2;
	
	settings3["enable"] = false;
	settings3["num"] = 53;
	settings["camera2"] = settings3;

	response.AddString("test string");
	response.AddInteger(937);
	response.AddBool(true);
	response.AddDouble(3.1415926);
	response.AddProperties(settings);
	std::cout << response.GetXML() << std::endl;
	std::cout << "isOK:" << response.IsOK() << std::endl;

	std::vector<instek::Properties::ValueType> vector;
	if(response.GetParameters(vector))
	{
		std::string temp_str1 = vector[0];
		int temp_int1 = vector[1];
		bool temp_bool1 = (int)vector[2];
		double temp_double1 = vector[3];
		instek::Properties temp_settings1 = vector[4];

		std::cout << "temp_str1:" << temp_str1 << std::endl;
		std::cout << "temp_int1:" << temp_int1 << std::endl;
		std::cout << "temp_bool1:" << temp_bool1 << std::endl;
		std::cout << "temp_double1:" << temp_double1 << std::endl;
		temp_str1 = static_cast<std::string>(temp_settings1["camera1"]["name"]);
		temp_double1 = temp_settings1["camera1"]["angle"];
		temp_bool1 = (int)temp_settings1["camera2"]["enable"];
		temp_int1 = temp_settings1["camera2"]["num"];
		std::cout << "temp_settings1[camera1][name]:" << temp_str1 << std::endl;
		std::cout << "temp_settings1[camera1][angle]:" << temp_double1 << std::endl;
		std::cout << "temp_settings1[camera2][enable]:" << temp_bool1 << std::endl;
		std::cout << "temp_settings1[camera2][num]:" << temp_int1 << std::endl;
	}
	else
	{
		std::cout << "getParameters error" << std::endl;
	}

	// Test parse and compose tree of response
	std::cout << "\nCompose response===================================" << std::endl;
	std::cout << response.GetXML() << std::endl;
	temp_xml = response.GetXML();
	response.Clear();
	response.Parse(temp_xml.c_str());
	response.ComposeTree();
	std::cout << response.GetXML() << std::endl;



	// Test regular request
	std::cout << "\nRegular request===================================" << std::endl;
	instek::MethodCall method;
	method.SetMethodName("examples.getStateName");
	std::cout << "MethodName:" << method.GetMethodName() << std::endl;
	method.SetMethodName("examples.Rename");
	std::cout << "MethodName:" << method.GetMethodName() << std::endl;
	instek::MethodCall method2;
	std::cout << "MethodName2:" << method2.GetMethodName() << std::endl;
	std::cout << "Method2 XML:" << method2.GetXML() << std::endl;
	
	method.AddString("test string");
	method.AddInteger(937);
	method.AddBool(true);
	method.AddDouble(3.1415926);
	method.AddProperties(settings);
	std::cout << method.GetXML() << std::endl;

	vector.clear();
	if(method.GetParameters(vector))
	{
		std::string temp_str1 = vector[0];
		int temp_int1 = vector[1];
		bool temp_bool1 = (int)vector[2];
		double temp_double1 = vector[3];
		instek::Properties temp_settings1 = vector[4];

		std::cout << "temp_str1:" << temp_str1 << std::endl;
		std::cout << "temp_int1:" << temp_int1 << std::endl;
		std::cout << "temp_bool1:" << temp_bool1 << std::endl;
		std::cout << "temp_double1:" << temp_double1 << std::endl;
		temp_str1 = static_cast<std::string>(temp_settings1["camera1"]["name"]);
		temp_double1 = temp_settings1["camera1"]["angle"];
		temp_bool1 = (int)temp_settings1["camera2"]["enable"];
		temp_int1 = temp_settings1["camera2"]["num"];
		std::cout << "temp_settings1[camera1][name]:" << temp_str1 << std::endl;
		std::cout << "temp_settings1[camera1][angle]:" << temp_double1 << std::endl;
		std::cout << "temp_settings1[camera2][enable]:" << temp_bool1 << std::endl;
		std::cout << "temp_settings1[camera2][num]:" << temp_int1 << std::endl;
	}
	else
	{
		std::cout << "getParameters error" << std::endl;
	}

	// Test parse and compose tree of request
	std::cout << "\nCompose request===================================" << std::endl;
	std::cout << method.GetXML() << std::endl;
	temp_xml = method.GetXML();
	method.Clear();
	method.Parse(temp_xml.c_str());
	method.ComposeTree();
	std::cout << method.GetXML() << std::endl;

	instek::MethodCall method4;
	method4.Parse("<methodCall><methodName>test.noparam</methodName><params></params></methodCall>");
	method4.ComposeTree();
	std::cout << "\nMethod4 XML:\n" << method4.GetXML() << std::endl;


	// Test copy of request
/*	std::cout << "\nCopy method===================================" << std::endl;
	method2.Clear();
	method2 = method;
	std::cout << method2.GetXML() << std::endl;*/

	// Test no parameters of request
	std::cout << "\nNo parameters of request===================================" << std::endl;
	instek::MethodCall method3;
	method3.SetMethodName("test.noparam");
	std::cout << "MethodName3:" << method3.GetMethodName() << std::endl;
	std::cout << "Method3 Only method name XML:\n" << method3.GetXML() << std::endl;
//	method3.SetVoidParameter();
	std::cout << "Method3 void parameter XML:\n" << method3.GetXML() << std::endl;
	
	vector.clear();
	if(method3.GetParameters(vector))
	{
		std::cout << "size of vector:" << vector.size() << std::endl;
	}
	else
	{
		std::cout << "getParameters error" << std::endl;
	}




	// Test no parameters of response
	std::cout << "\nNo parameters of response===================================" << std::endl;
	instek::MethodResponse response3;
	std::cout << "response3 empty XML:\n" << response3.GetXML() << std::endl;

	vector.clear();
	if(response3.GetParameters(vector))
	{
		std::cout << "size of vector:" << vector.size() << std::endl;
	}
	else
	{
		std::cout << "getParameters error" << std::endl;
	}

//	response3.SetVoidParameter();
	std::cout << "response3 void parameter XML:\n" << response3.GetXML() << std::endl;

	vector.clear();
	if(response3.GetParameters(vector))
	{
		std::cout << "size of vector:" << vector.size() << std::endl;
	}
	else
	{
		std::cout << "getParameters error" << std::endl;
	}

	return 0;
}
