#include <libtransport/rpc_session.hpp>
#include <iostream>
#include <libcommon/properties.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <cstdlib>

using namespace std;
using namespace instek;

typedef instek::RPCSession<instek::SSL_SOCK> NowRPCSession;
//typedef instek::RPCSession<instek::TCP_SOCK> NowRPCSession;

int main(int argc, char** argv)
{

	if(argc != 2)
	{
		std::cout << "Usage: " << argv[0] << "<StopServer>(0: false, 1: true)" << std::endl;
		return -1;
	}

	int is_stop_server = atoi(argv[1]);
	std::string address = "127.0.0.1";
	int port = 60000;

	NowRPCSession* test_session = new NowRPCSession(address, port, true);

	// ================================
	instek::MethodCallPtr test_int_call(new instek::MethodCall);
	test_int_call->SetMethodName("example.TestInt");
	test_int_call->AddInteger(283);
	test_session->SetTimeoutValue(3000);
	instek::MethodResponsePtr resp = test_session->Call(test_int_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%d\n", (int) resp->GetResult());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_bool_call(new instek::MethodCall);
	test_bool_call->SetMethodName("example.TestBool");
	test_bool_call->AddBool(false);

	resp = test_session->Call(test_bool_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%d\n", (bool) resp->GetResult());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_str_call(new instek::MethodCall);
	test_str_call->SetMethodName("example.TestString");
	test_str_call->AddString("C++");

	resp = test_session->Call(test_str_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%s\n", ((std::string)resp->GetResult()).c_str());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_double_call(new instek::MethodCall);
	test_double_call->SetMethodName("example.TestDouble");
	test_double_call->AddDouble(0.93);

	resp = test_session->Call(test_double_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%f\n", (double)(resp->GetResult()));
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_struct_call(new instek::MethodCall);
	test_struct_call->SetMethodName("example.TestStruct");
	instek::Properties s1;
	s1["string"] = "test String";
	s1["double"] = 0.375;
	s1["integer"] = 12345;
	s1["boolean"] = true;
	s1["struct"]["Apple"] = "steve jobs";
	s1["struct"]["OMG"] = false;
	test_struct_call->AddProperties(s1);

	resp = test_session->Call(test_struct_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			s1 = resp->GetResult();
			printf("%s\n", static_cast<std::string>(s1["Camera1"]["name"]).c_str());
			printf("%d\n", static_cast<bool>(s1["Camera1"]["enabled"]));
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_two_params_call(new instek::MethodCall);
	test_two_params_call->SetMethodName("example.TestTwoParams");
	test_two_params_call->AddDouble(54.96);
	test_two_params_call->AddInteger(14);

	resp = test_session->Call(test_two_params_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			s1 = resp->GetResult();

			printf("%f\n", static_cast<double>(s1["RetVal"]["integer"]));
			printf("%s\n", static_cast<std::string>(s1["RetVal"]["string"]).c_str());

		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_array_call(new instek::MethodCall);
	test_array_call->SetMethodName("example.TestArray");
	instek::XMLRPCArray array;
	array.push_back(95);
	array.push_back(true);
	array.push_back("Pig");
	array.push_back(-0.96);
	Properties s2;
	s2["camera1"] = "dog";
	s2["camera2"] = "cat";
	array.push_back(s2);
	instek::XMLRPCArray array2;

	array2.push_back(1234);
	array2.push_back("one two three four");
	array.push_back(array2);

	test_array_call->AddArray(array);

	resp = test_session->Call(test_array_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			instek::XMLRPCArray array1 = resp->GetResult();
			if(array1.size() >= 6)
			{
				printf("%d\n", (int)array1[0]);
				printf("%d\n", (bool)array1[1]);
				printf("%s\n", static_cast<std::string>(array1[2]).c_str());
				printf("%f\n", (double)array1[3]);
				s1 = array1[4];
				printf("%s\n", static_cast<std::string>(s1["PP"]["out"]).c_str());
				printf("%d\n", static_cast<int>(s1["PP"]["in"]));
				instek::XMLRPCArray array2 = array1[5];
				printf("%s\n", static_cast<std::string>(array2[0]).c_str());
				printf("%d\n", static_cast<int>(array2[1]));
			}
			else
			{
				printf("Error: The size of array of response do not match the client expects.\n");
			}

		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_date_time_call(new instek::MethodCall);
	instek::XMLRPCDateTime date_time;
	date_time.SetDateTime("1988-07-07T12:34:21");

	test_date_time_call->SetMethodName("example.TestXMLRPCDateTime");
	test_date_time_call->AddDateTime(date_time);
	resp = test_session->Call(test_date_time_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{

			instek::XMLRPCDateTime date_time2 = resp->GetResult();
			printf("%s\n", date_time2.GetDateTimeString().c_str());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
	instek::MethodCallPtr test_undefined_call(new instek::MethodCall);
	test_undefined_call->SetMethodName("example.Undefined");

	resp = test_session->Call(test_undefined_call);

	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("It is impossible.\n");
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}

	// ================================
//	sleep(1);

	// ================================
	if(is_stop_server)
	{
		instek::MethodCallPtr stop_server_call(new instek::MethodCall);
		stop_server_call->SetMethodName("example.StopServer");
		resp = test_session->Call(stop_server_call);
	}

	return 0;
}
