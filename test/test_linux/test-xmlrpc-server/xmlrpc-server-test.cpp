#include <string>
#include <boost/bind.hpp>
#include <xmlrpc_server/xmlrpc_server.hpp>
#include <xmlrpc_server/xmlrpc_utility.hpp>
#include <http_server/http_server.hpp>
#include <http_server/http_request_dispatcher.hpp>
#include <iostream>
#include <cstdlib>

#include <http_server/notify_request_handler.hpp>

void prepare_server_push_data(const std::unordered_map<std::string, std::string>& /*query_strings*/, bool /*not_first_call*/, std::map<std::string, std::string>& server_push_headers, std::string& data, bool& is_end)
{
#ifdef DEBUG
	printf("Query:\n");
	std::unordered_map<std::string, std::string>::const_iterator iter = query_strings.begin();
	for(; iter!= query_strings.end(); ++iter)
	{
		printf("( %s , %s )\n", iter->first.c_str(), iter->second.c_str());
	}

	printf("not_first_call = %d\n", not_first_call);
#endif

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	server_push_headers["Content-Type"] = "text/html";
	data = "<html><head><title>Test Server Push</title><body><h3>";
	data += asctime(timeinfo);
	data += "</h3></body></html>";

	is_end = false;

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

void server_push_exit_callback(const std::unordered_map<std::string, std::string>& /*query_strings*/)
{
}

class Test
{
private:
	instek::XMLRPC_Server* server_;
public:
	Test(instek::XMLRPC_Server* server):server_(server) {}
	void callback1(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (int)vector[0]<< std::endl;
		resp.AddString("OK");
	}

	void callback2(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (std::string)vector[0]<< std::endl;
		std::cout << "[1] = " << (double)vector[1]<< std::endl;
		resp.AddInteger(34);
		resp.AddBool(true);
	}

	void TestInt(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (int)vector[0]<< std::endl;
		resp.AddInteger(23);
	}

	void TestBool(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (bool)vector[0]<< std::endl;
		resp.AddBool(true);
	}

	void TestString(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (std::string)vector[0]<< std::endl;
		resp.AddString("Linux is best......");

	}

	void TestDouble(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (double)vector[0]<< std::endl;
		resp.AddDouble(3.1415926);
	}

	void TestStruct(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		instek::Properties setting = (instek::Properties)vector[0];
		std::cout << "setting[string] = " << (std::string)setting["string"] << std::endl;
		std::cout << "setting[double] = " << (double)setting["double"] << std::endl;
		std::cout << "setting[integer] = " << (int)setting["integer"] << std::endl;
		std::cout << "setting[boolean] = " << (bool)setting["boolean"] << std::endl;
		std::cout << "setting[struct][Apple] = " << (std::string)setting["struct"]["Apple"] << std::endl;
		std::cout << "setting[struct][OMG] = " << (bool)setting["struct"]["OMG"] << std::endl;

		instek::Properties setting2;
		setting2["Camera1"]["name"] = "Steve";
		setting2["Camera1"]["enabled"] = true;
		resp.AddProperties(setting2);
	}

	void TestTwoParams(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		std::cout << "[0] = " << (double)vector[0]<< std::endl;
		std::cout << "[1] = " << (int)vector[1]<< std::endl;


		instek::Properties setting2;
		setting2["RetVal"]["integer"] = 1.0;
		setting2["RetVal"]["string"] = "Second";
		resp.AddProperties(setting2);
	}

	void UnPublicIntrospectionFunc(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		resp.AddBool(true);
	}

	void TestArray(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;

		instek::XMLRPCArray array = vector[0];

		if(array.size() < 6)
		{
			resp.SetFault(-32602, instek::XMLRPC_Utility::GetXMLRPCErrorString(-32602));
			return;
		}


		std::cout << "array[0] = " << static_cast<int>(array[0]) << std::endl;
		std::cout << "array[1] = " << static_cast<bool>(array[1]) << std::endl;
		std::cout << "array[2] = " << static_cast<std::string>(array[2]) << std::endl;
		std::cout << "array[3] = " << static_cast<double>(array[3]) << std::endl;
		std::cout << "array[4] = " << std::endl;
		instek::Properties setting = static_cast<instek::Properties>(array[4]);
		std::cout << "[camera1] = " << static_cast<std::string>(setting["camera1"]) << std::endl;
		std::cout << "[camera2] = " << static_cast<std::string>(setting["camera2"]) << std::endl;

		std::cout << "array[5] = " << std::endl;
		instek::XMLRPCArray array2 = array[5];
		std::cout << "[0] = " << static_cast<int>(array2[0]) << std::endl;
		std::cout << "[1] = " << static_cast<std::string>(array2[1]) << std::endl;

		instek::XMLRPCArray array3;
		array3.push_back(24);
		array3.push_back(false);
		array3.push_back("^-^y");
		array3.push_back(344.53);

		instek::Properties setting2;
		setting2["PP"]["out"] = "I don't know.";
		setting2["PP"]["in"] = 35;
		array3.push_back(setting2);

		instek::XMLRPCArray array4;
		array4.push_back("DD");
		array4.push_back(29);
		array3.push_back(array4);

		resp.AddArray(array3);
	}

	void StopServer(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		if(server_)
			server_->StopServer();
		resp.AddInteger(0);
	}

	void TestThrowFault(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		throw instek::XMLRPC_Fault(77, "My test throwing fault.");
		resp.AddInteger(88);
	}

	void TestEmptyStruct(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		resp.AddProperties(instek::Properties());
	}

	void TestEmptydArray(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		resp.AddArray(instek::XMLRPCArray());
	}

	void TestComplicatedStruct(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		// Request
		instek::Properties prop = vector[0];

		std::cout << static_cast<std::string>(prop["string"]) << std::endl;

		instek::Properties prop2 = prop["struct"];

		std::cout << static_cast<std::string>(prop2["string"]) << std::endl;

		instek::XMLRPCArray array = prop2["Array"];

		std::cout << static_cast<std::string>(array[0]) << std::endl;

		instek::Properties prop3 = array[1];

		std::cout << static_cast<std::string>(prop3["string"]) << std::endl;

		instek::XMLRPCArray array2 = array[2];

		std::cout << static_cast<std::string>(array2[0]) << std::endl;
		std::cout << static_cast<int>(array2[1]) << std::endl;

		// Response
		instek::Properties resp_prop;
		instek::Properties resp_prop2;
		instek::Properties resp_prop3;
		instek::XMLRPCArray resp_array;
		instek::XMLRPCArray resp_array2;

		resp_prop3["string"] = "struct in array";
		resp_array2.push_back("array in array");

		resp_array.push_back(resp_prop3);
		resp_array.push_back(resp_array2);

		resp_prop2["array in struct"] = resp_array;
		resp_prop["struct in struct"] = resp_prop2;

		resp.AddProperties(resp_prop);
	}

	void TestComplicatedArray(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		// Request
		instek::XMLRPCArray array = vector[0];

		std::cout << static_cast<std::string>(array[0]) << std::endl;

		instek::XMLRPCArray array2 = array[1];

		std::cout << static_cast<std::string>(array2[0]) << std::endl;

		instek::Properties prop = array2[1];

		std::cout << static_cast<std::string>(prop["string"]) << std::endl;

		instek::XMLRPCArray array3 = prop["Array"];

		std::cout << static_cast<std::string>(array3[0]) << std::endl;

		instek::Properties prop2 = prop["Struct"];

		std::cout << static_cast<std::string>(prop2["string"]) << std::endl;

		// Response
		instek::Properties resp_prop;
		instek::Properties resp_prop2;
		instek::XMLRPCArray resp_array;
		instek::XMLRPCArray resp_array2;
		instek::XMLRPCArray resp_array3;

		resp_prop2["string"] = "struct in struct";
		resp_array3.push_back("array in struct");

		resp_prop["string"] = "struct in array";
		resp_prop["Struct"] = resp_prop2;
		resp_prop["Array"] = resp_array3;

		resp_array2.push_back("array in array");

		resp_array.push_back(resp_prop);
		resp_array.push_back(resp_array2);

		resp.AddArray(resp_array);
	}

	void TestAddValue(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		resp.AddValue("string");
		resp.AddValue(true);
		resp.AddValue(84);
		resp.AddValue(34.98);

		instek::XMLRPCDateTime date_time;
		date_time.SetDateTime("1923-03-21T09:43:12");
		resp.AddValue(date_time);

		instek::Properties prop;
		prop["test property"] = "success";
		resp.AddValue(prop);

		instek::XMLRPCArray array;
		array.push_back("end");
		resp.AddValue(array);
	}

	void TestXMLRPCDateTime(const instek::XMLRPC_Value_List& vector, instek::MethodResponse& resp)
	{
		std::cout << "======" << __func__ << "======" << std::endl;
		instek::XMLRPCDateTime date_time = vector[0];
		std::cout << date_time.GetDateTimeString() << std::endl;


		// Response
		instek::XMLRPCDateTime resp_date_time;
		std::time_t result = std::time(NULL);
		struct tm* timeinfo = NULL;
		timeinfo = std::localtime(&result);

		resp_date_time.SetDateTime(*timeinfo);

		resp.AddDateTime(resp_date_time);
	}

	void TestGetConnectionInfo(const instek::XMLRPC_Value_List& /*vector*/, instek::MethodResponse& resp, const instek::HttpConnectionInfo& info)
	{
		std::cout << "======" << __func__ << "======" << std::endl;

		instek::Properties prop;
		std::cout << "Address = " << info.remote_address << ", Port = " << info.remote_port << std::endl;
		prop["Remote Address"] = info.remote_address;
		prop["Remote Port"] = info.remote_port;

		resp.AddProperties(prop);
	}
};


int main(int /*argc*/, char** /*argv*/)
{
	instek::SSLContextInfo ssl_context_info;
#if 0
//	ssl_context_info.rsa_private_key_file = "./gen_ca_utils/instek_ca_output/server-key.pem";
	ssl_context_info.private_key_file = "./gen_ca_utils/instek_ca_output/server-key.pem";
//	ssl_context_info.private_key_file = "/etc/instek/key.pem";
	ssl_context_info.private_key_password = "testserver";
	ssl_context_info.certificate_file = "./gen_ca_utils/instek_ca_output/server-cert.pem";
//	ssl_context_info.certificate_file = "./gen_ca_utils/instek_ca_output/server-cert-chain.pem";
//	ssl_context_info.certificate_file = "/etc/instek/cacert.pem";
	ssl_context_info.certificate_authority_file = "./gen_ca_utils/instek_ca_output/ca-chain.pem";
	ssl_context_info.ssl_dh_file = "./gen_ca_utils/instek_ca_output/dHParam.pem";
#endif
	ssl_context_info.private_key_file = "./test_certificates/server-key.pem";
	ssl_context_info.private_key_password = "testserver";
	ssl_context_info.certificate_file = "./test_certificates/server-cert.pem";
	ssl_context_info.certificate_authority_file = "./test_certificates/ca-chain.pem";
	ssl_context_info.ssl_dh_file = "./test_certificates/dHParam.pem";

	ssl_context_info.is_use_default_verify_paths = false;
	ssl_context_info.is_use_private_key_password_in_code = true;

	std::string address;
	int port = 60000;

	boost::shared_ptr<instek::IHttpRequestDispatcher> http_request_dispathcer((instek::IHttpRequestDispatcher*)(new instek::HttpRequestDispatcher));
	boost::shared_ptr<instek::IHttpRequestHandler> test_server_push_request_handler( (instek::IHttpRequestHandler*)(new instek::NotifyRequestHandler(&prepare_server_push_data, &server_push_exit_callback)) );
	http_request_dispathcer->RegisterHttpRequestHandler("GET", "/notify.cgi", test_server_push_request_handler);
	boost::shared_ptr<instek::IServer> http_server;

	http_server.reset( (instek::IServer*)(new instek::HttpServer(http_request_dispathcer, address, port, ssl_context_info)) );

	instek::XMLRPC_Server* server = new instek::XMLRPC_Server(http_server, http_request_dispathcer);


	Test tester(server);

	server->AddMethod(boost::bind(&Test::TestInt, &tester, _1, _2), "i4","example.TestInt", "i4", "Test integer help.");
	server->AddMethod(boost::bind(&Test::TestBool, &tester, _1, _2), "boolean", "example.TestBool", "boolean", "Test boolean help.");
	server->AddMethod(boost::bind(&Test::TestString, &tester, _1, _2), "string", "example.TestString", "string", "Test string help.");
	server->AddMethod(boost::bind(&Test::TestDouble, &tester, _1, _2), "double", "example.TestDouble", "double", "Test double help.");
	server->AddMethod(boost::bind(&Test::TestStruct, &tester, _1, _2), "struct", "example.TestStruct", "struct", "Test struct help.");
	server->AddMethod(boost::bind(&Test::TestTwoParams, &tester, _1, _2), "struct", "example.TestTwoParams", "double,int", "Test two parameters help.");
	server->AddMethod(boost::bind(&Test::UnPublicIntrospectionFunc, &tester, _1, _2), "boolean", "example.UnPublic", "", "Test private in introspection .", false);
	server->AddMethod(boost::bind(&Test::TestArray, &tester, _1, _2), "array", "example.TestArray", "array", "Test array help.");
	server->AddMethod(boost::bind(&Test::StopServer, &tester, _1, _2), "int", "example.StopServer", "", "Stop server help.");
	server->AddMethod(boost::bind(&Test::TestThrowFault, &tester, _1, _2), "int", "example.TestThrowFault", "", "Test throw fault help.");

	server->AddMethod(boost::bind(&Test::TestEmptyStruct, &tester, _1, _2), "struct", "example.TestEmptyStruct", "", "Test returning empty struct help.");
	server->AddMethod(boost::bind(&Test::TestEmptydArray, &tester, _1, _2), "array", "example.TestEmptydArray", "", "Test returning empty array help.");
	server->AddMethod(boost::bind(&Test::TestComplicatedStruct, &tester, _1, _2), "struct", "example.TestComplicatedStruct", "struct", "Test complicated struct help.");
	server->AddMethod(boost::bind(&Test::TestComplicatedArray, &tester, _1, _2), "array", "example.TestComplicatedArray", "array", "Test complicated array help.");
	server->AddMethod(boost::bind(&Test::TestAddValue, &tester, _1, _2), "string,boolean,int,double,struct,array", "example.TestAddValue", "", "Test method to add value.");
	server->AddMethod(boost::bind(&Test::TestXMLRPCDateTime, &tester, _1, _2), "dateTime.iso8601", "example.TestXMLRPCDateTime", "dateTime.iso8601", "Test method to add date time.");

	server->AddMethod(boost::bind(&Test::TestGetConnectionInfo, &tester, _1, _2, _3), "struct", "example.TestGetConnectionInfo", "", "Test method to get the information of connection.");

	server->StartServer();

	printf("type 'q' to quit.\n");
	char quit_char;
	do
	{
		scanf("%c",&quit_char);
	} while (quit_char != 'q');

	delete server;

	return 0;
}
