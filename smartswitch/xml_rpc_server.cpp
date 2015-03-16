#include "sensor_prototype.h"
#include "xml_rpc_server.h"

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

RPCSvr::RPCSvr(SensorFactory* srf, const int& nPort)
	: sensor_factory_(srf),
	rpc_port_(nPort)
{
	std::string address;

	FBi::SSLContextInfo ssl_context_info;
	ssl_context_info.private_key_file = "./test_certificates/server-key.pem";
	ssl_context_info.private_key_password = "testserver";
	ssl_context_info.certificate_file = "./test_certificates/server-cert.pem";
	ssl_context_info.certificate_authority_file = "./test_certificates/ca-chain.pem";
	ssl_context_info.ssl_dh_file = "./test_certificates/dHParam.pem";

	ssl_context_info.is_use_default_verify_paths = false;
	ssl_context_info.is_use_private_key_password_in_code = true;

	boost::shared_ptr<FBi::IHttpRequestDispatcher> http_request_dispathcer((FBi::IHttpRequestDispatcher*)(new FBi::HttpRequestDispatcher));
	boost::shared_ptr<FBi::IHttpRequestHandler> test_server_push_request_handler( (FBi::IHttpRequestHandler*)(new FBi::NotifyRequestHandler(&prepare_server_push_data, &server_push_exit_callback)) );
	http_request_dispathcer->RegisterHttpRequestHandler("GET", "/notify.cgi", test_server_push_request_handler);

	boost::shared_ptr<FBi::IServer> http_server;
	http_server.reset( (FBi::IServer*)(new FBi::HttpServer(http_request_dispathcer, address, nPort, ssl_context_info)) );

	server_ = new FBi::XMLRPC_Server(http_server, http_request_dispathcer);
	server_->AddMethod(boost::bind(&RPCSvr::TestInt, this, _1, _2), "i4","example.TestInt", "i4", "Test integer help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestBool, this, _1, _2), "boolean", "example.TestBool", "boolean", "Test boolean help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestString, this, _1, _2), "string", "example.TestString", "string", "Test string help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestDouble, this, _1, _2), "double", "example.TestDouble", "double", "Test double help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestStruct, this, _1, _2), "struct", "example.TestStruct", "struct", "Test struct help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestTwoParams, this, _1, _2), "struct", "example.TestTwoParams", "double,int", "Test two parameters help.");
	server_->AddMethod(boost::bind(&RPCSvr::UnPublicIntrospectionFunc, this, _1, _2), "boolean", "example.UnPublic", "", "Test private in introspection .", false);
	server_->AddMethod(boost::bind(&RPCSvr::TestArray, this, _1, _2), "array", "example.TestArray", "array", "Test array help.");
	server_->AddMethod(boost::bind(&RPCSvr::StopServer, this, _1, _2), "int", "example.StopServer", "", "Stop server help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestThrowFault, this, _1, _2), "int", "example.TestThrowFault", "", "Test throw fault help.");

	server_->AddMethod(boost::bind(&RPCSvr::TestEmptyStruct, this, _1, _2), "struct", "example.TestEmptyStruct", "", "Test returning empty struct help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestEmptydArray, this, _1, _2), "array", "example.TestEmptydArray", "", "Test returning empty array help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestComplicatedStruct, this, _1, _2), "struct", "example.TestComplicatedStruct", "struct", "Test complicated struct help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestComplicatedArray, this, _1, _2), "array", "example.TestComplicatedArray", "array", "Test complicated array help.");
	server_->AddMethod(boost::bind(&RPCSvr::TestAddValue, this, _1, _2), "string,boolean,int,double,struct,array", "example.TestAddValue", "", "Test method to add value.");
	server_->AddMethod(boost::bind(&RPCSvr::TestXMLRPCDateTime, this, _1, _2), "dateTime.iso8601", "example.TestXMLRPCDateTime", "dateTime.iso8601", "Test method to add date time.");
	server_->AddMethod(boost::bind(&RPCSvr::TestGetConnectionInfo, this, _1, _2, _3), "struct", "example.TestGetConnectionInfo", "", "Test method to get the information of connection.");

	// rpc function
	server_->AddMethod(boost::bind(&RPCSvr::GetSensorStatus, this, _1, _2), "struct", "GetSensorStatus", "int", "query status");
	server_->AddMethod(boost::bind(&RPCSvr::SetSensorStatus, this, _1, _2), "struct", "SetSensorStatus", "struct", "set status");
 
	server_->StartServer();
}

RPCSvr::~RPCSvr()
{
	delete server_;
}

void RPCSvr::callback1(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (int)vector[0]<< std::endl;
	resp.AddString("OK");
}

void RPCSvr::callback2(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (std::string)vector[0]<< std::endl;
	std::cout << "[1] = " << (double)vector[1]<< std::endl;
	resp.AddInteger(34);
	resp.AddBool(true);
}

void RPCSvr::TestInt(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (int)vector[0]<< std::endl;
	resp.AddInteger(23);
}

void RPCSvr::TestBool(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (bool)vector[0]<< std::endl;
	resp.AddBool(true);
}

void RPCSvr::TestString(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (std::string)vector[0]<< std::endl;
	resp.AddString("Linux is best......");

}

void RPCSvr::TestDouble(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (double)vector[0]<< std::endl;
	resp.AddDouble(3.1415926);
}

void RPCSvr::TestStruct(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	FBi::Properties setting = (FBi::Properties)vector[0];
	std::cout << "setting[string] = " << (std::string)setting["string"] << std::endl;
	std::cout << "setting[double] = " << (double)setting["double"] << std::endl;
	std::cout << "setting[integer] = " << (int)setting["integer"] << std::endl;
	std::cout << "setting[boolean] = " << (bool)setting["boolean"] << std::endl;
	std::cout << "setting[struct][Apple] = " << (std::string)setting["struct"]["Apple"] << std::endl;
	std::cout << "setting[struct][OMG] = " << (bool)setting["struct"]["OMG"] << std::endl;

	FBi::Properties setting2;
	setting2["Camera1"]["name"] = "Steve";
	setting2["Camera1"]["enabled"] = true;
	resp.AddProperties(setting2);
}

void RPCSvr::TestTwoParams(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	std::cout << "[0] = " << (double)vector[0]<< std::endl;
	std::cout << "[1] = " << (int)vector[1]<< std::endl;


	FBi::Properties setting2;
	setting2["RetVal"]["integer"] = 1.0;
	setting2["RetVal"]["string"] = "Second";
	resp.AddProperties(setting2);
}

void RPCSvr::UnPublicIntrospectionFunc(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	resp.AddBool(true);
}

void RPCSvr::TestArray(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;

	FBi::XMLRPCArray array = vector[0];

	if(array.size() < 6)
	{
		resp.SetFault(-32602, FBi::XMLRPC_Utility::GetXMLRPCErrorString(-32602));
		return;
	}

	std::cout << "array[0] = " << static_cast<int>(array[0]) << std::endl;
	std::cout << "array[1] = " << static_cast<bool>(array[1]) << std::endl;
	std::cout << "array[2] = " << static_cast<std::string>(array[2]) << std::endl;
	std::cout << "array[3] = " << static_cast<double>(array[3]) << std::endl;
	std::cout << "array[4] = " << std::endl;
	FBi::Properties setting = static_cast<FBi::Properties>(array[4]);
	std::cout << "[camera1] = " << static_cast<std::string>(setting["camera1"]) << std::endl;
	std::cout << "[camera2] = " << static_cast<std::string>(setting["camera2"]) << std::endl;

	std::cout << "array[5] = " << std::endl;
	FBi::XMLRPCArray array2 = array[5];
	std::cout << "[0] = " << static_cast<int>(array2[0]) << std::endl;
	std::cout << "[1] = " << static_cast<std::string>(array2[1]) << std::endl;

	FBi::XMLRPCArray array3;
	array3.push_back(24);
	array3.push_back(false);
	array3.push_back("^-^y");
	array3.push_back(344.53);

	FBi::Properties setting2;
	setting2["PP"]["out"] = "I don't know.";
	setting2["PP"]["in"] = 35;
	array3.push_back(setting2);

	FBi::XMLRPCArray array4;
	array4.push_back("DD");
	array4.push_back(29);
	array3.push_back(array4);

	resp.AddArray(array3);
}

void RPCSvr::StopServer(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	if(server_)
		server_->StopServer();
	resp.AddInteger(0);
}

void RPCSvr::TestThrowFault(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	throw FBi::XMLRPC_Fault(77, "My test throwing fault.");
	resp.AddInteger(88);
}

void RPCSvr::TestEmptyStruct(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	resp.AddProperties(FBi::Properties());
}

void RPCSvr::TestEmptydArray(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	resp.AddArray(FBi::XMLRPCArray());
}

void RPCSvr::TestComplicatedStruct(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	// Request
	FBi::Properties prop = vector[0];

	std::cout << static_cast<std::string>(prop["string"]) << std::endl;

	FBi::Properties prop2 = prop["struct"];

	std::cout << static_cast<std::string>(prop2["string"]) << std::endl;

	FBi::XMLRPCArray array = prop2["Array"];

	std::cout << static_cast<std::string>(array[0]) << std::endl;

	FBi::Properties prop3 = array[1];

	std::cout << static_cast<std::string>(prop3["string"]) << std::endl;

	FBi::XMLRPCArray array2 = array[2];

	std::cout << static_cast<std::string>(array2[0]) << std::endl;
	std::cout << static_cast<int>(array2[1]) << std::endl;

	// Response
	FBi::Properties resp_prop;
	FBi::Properties resp_prop2;
	FBi::Properties resp_prop3;
	FBi::XMLRPCArray resp_array;
	FBi::XMLRPCArray resp_array2;

	resp_prop3["string"] = "struct in array";
	resp_array2.push_back("array in array");

	resp_array.push_back(resp_prop3);
	resp_array.push_back(resp_array2);

	resp_prop2["array in struct"] = resp_array;
	resp_prop["struct in struct"] = resp_prop2;

	resp.AddProperties(resp_prop);
}

void RPCSvr::TestComplicatedArray(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	// Request
	FBi::XMLRPCArray array = vector[0];

	std::cout << static_cast<std::string>(array[0]) << std::endl;

	FBi::XMLRPCArray array2 = array[1];

	std::cout << static_cast<std::string>(array2[0]) << std::endl;

	FBi::Properties prop = array2[1];

	std::cout << static_cast<std::string>(prop["string"]) << std::endl;

	FBi::XMLRPCArray array3 = prop["Array"];

	std::cout << static_cast<std::string>(array3[0]) << std::endl;

	FBi::Properties prop2 = prop["Struct"];

	std::cout << static_cast<std::string>(prop2["string"]) << std::endl;

	// Response
	FBi::Properties resp_prop;
	FBi::Properties resp_prop2;
	FBi::XMLRPCArray resp_array;
	FBi::XMLRPCArray resp_array2;
	FBi::XMLRPCArray resp_array3;

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

void RPCSvr::TestAddValue(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	resp.AddValue("string");
	resp.AddValue(true);
	resp.AddValue(84);
	resp.AddValue(34.98);

	FBi::XMLRPCDateTime date_time;
	date_time.SetDateTime("1923-03-21T09:43:12");
	resp.AddValue(date_time);

	FBi::Properties prop;
	prop["test property"] = "success";
	resp.AddValue(prop);

	FBi::XMLRPCArray array;
	array.push_back("end");
	resp.AddValue(array);
}

void RPCSvr::TestXMLRPCDateTime(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp)
{
	//std::cout << "======" << __func__ << "======" << std::endl;
	FBi::XMLRPCDateTime date_time = vector[0];
	std::cout << date_time.GetDateTimeString() << std::endl;

	// Response
	FBi::XMLRPCDateTime resp_date_time;
	std::time_t result = std::time(NULL);
	struct tm* timeinfo = NULL;
	timeinfo = std::localtime(&result);

	resp_date_time.SetDateTime(*timeinfo);

	resp.AddDateTime(resp_date_time);
}

void RPCSvr::TestGetConnectionInfo(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp, const FBi::HttpConnectionInfo& info)
{
	//std::cout << "======" << __func__ << "======" << std::endl;

	FBi::Properties prop;
	std::cout << "Address = " << info.remote_address << ", Port = " << info.remote_port << std::endl;
	prop["Remote Address"] = info.remote_address;
	prop["Remote Port"] = info.remote_port;

	resp.AddProperties(prop);
}

void RPCSvr::GetSensorStatus(const FBi::XMLRPC_Value_List& calldata, FBi::MethodResponse& resp)
{
	int nUserId = calldata[0];

	std::string strUsername;
	std::string strPassword;

	resp.AddProperties(sensor_factory_->QuerySensorStatus());

	/*if(getUserInfoById(nUserId, strUsername, strPassword))
	{
		boost::mutex::scoped_lock lock(lock_);
		resp.AddProperties(layoutMgr_.GetConfigProperties());
	}*/
}

void RPCSvr::SetSensorStatus(const FBi::XMLRPC_Value_List& calldata, FBi::MethodResponse& resp)
{
	FBi::Properties Props = (FBi::Properties)calldata[0];

	resp.AddProperties(sensor_factory_->SetSensorStatus(Props));
}