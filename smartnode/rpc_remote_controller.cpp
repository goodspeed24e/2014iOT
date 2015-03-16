#include <libtransport/rpc_session.hpp>
#include <libcommon/properties.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <string>
#include <cstdlib>

#include "rpc_remote_controller.h"
#include "smartnode.h"

RPCRemoteController::RPCRemoteController(SmartNode* node)
	: m_sensor_node(node)
{
	m_rpc_session = new NowRPCSession(m_sensor_node->GetRemoteIP(), m_sensor_node->GetRemotePort(), true);

	if (0)
		SendTest();
}

RPCRemoteController::~RPCRemoteController()
{
	delete m_rpc_session;
}

void RPCRemoteController::GetSensorStatus(FBi::Properties& sensorProps)
{
	FBi::MethodCallPtr get_sensor_status(new FBi::MethodCall);
	get_sensor_status->SetMethodName("GetSensorStatus");
	get_sensor_status->AddInteger(300);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(get_sensor_status);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			//std::cout << "============= print response of getProps ==================== " << std::endl;
			//std::cout << resp->GetXML() << std::endl;
			//std::cout << "=================================================== " << std::endl;

			sensorProps = resp->GetResult();
		}
		else
		{
			printf("XMLRPC call (GetSensorStatus) error = %d, error msg = %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}
}

bool RPCRemoteController::SetSensorStatus(const FBi::Properties& sensorProps)
{
	FBi::MethodCallPtr set_sensor_status(new FBi::MethodCall);
	set_sensor_status->SetMethodName("SetSensorStatus");
	set_sensor_status->AddProperties(sensorProps);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(set_sensor_status);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			//std::cout << "============= print response of getProps ==================== " << std::endl;
			//std::cout << resp->GetXML() << std::endl;
			//std::cout << "=================================================== " << std::endl;

			FBi::Properties s2 = resp->GetResult();
			return true;
		}
		else
		{
			printf("XMLRPC call (SetSensorStatus) error = %d, error msg = %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}
	}
	else
	{
		printf("Response is NULL.\n");
	}
	return false;
}

bool RPCRemoteController::SendInt(const int& val)
{
	FBi::MethodCallPtr test_int_call(new FBi::MethodCall);
	test_int_call->SetMethodName("example.TestInt");
	test_int_call->AddInteger(val);
	m_rpc_session->SetTimeoutValue(3000);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_int_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%d\n", (int) resp->GetResult());
			return true;
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
	return false;
}

bool RPCRemoteController::SendBool(const bool& val)
{
	FBi::MethodCallPtr test_bool_call(new FBi::MethodCall);
	test_bool_call->SetMethodName("example.TestBool");
	test_bool_call->AddBool(val);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_bool_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%d\n", (bool) resp->GetResult());
			return true;
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
	return false;
}

bool RPCRemoteController::SendString(const std::string& val)
{
	FBi::MethodCallPtr test_str_call(new FBi::MethodCall);
	test_str_call->SetMethodName("example.TestString");
	test_str_call->AddString(val);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_str_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%s\n", ((std::string)resp->GetResult()).c_str());
			return true;
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
	return false;
}

bool RPCRemoteController::SendDouble(const double& val)
{
	FBi::MethodCallPtr test_double_call(new FBi::MethodCall);
	test_double_call->SetMethodName("example.TestDouble");
	test_double_call->AddDouble(val);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_double_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("%f\n", (double)(resp->GetResult()));
			return true;
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
	return false;
}

bool RPCRemoteController::SendStruct(const FBi::Properties& s1)
{
	FBi::MethodCallPtr test_struct_call(new FBi::MethodCall);
	test_struct_call->SetMethodName("example.TestStruct");
	test_struct_call->AddProperties(s1);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_struct_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			FBi::Properties s2 = resp->GetResult();
			printf("%s\n", static_cast<std::string>(s2["Camera1"]["name"]).c_str());
			printf("%d\n", static_cast<bool>(s2["Camera1"]["enabled"]));
			return true;
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
	return false;
}

bool RPCRemoteController::SendTwoParams(const double& val1, const int& val2)
{
	FBi::MethodCallPtr test_two_params_call(new FBi::MethodCall);
	test_two_params_call->SetMethodName("example.TestTwoParams");
	test_two_params_call->AddDouble(val1);
	test_two_params_call->AddInteger(val2);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_two_params_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			FBi::Properties s1 = resp->GetResult();

			printf("%f\n", static_cast<double>(s1["RetVal"]["integer"]));
			printf("%s\n", static_cast<std::string>(s1["RetVal"]["string"]).c_str());
			return true;
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
	return false;
}

bool RPCRemoteController::SendArray(const FBi::XMLRPCArray& array)
{
	FBi::MethodCallPtr test_array_call(new FBi::MethodCall);
	test_array_call->SetMethodName("example.TestArray");
	test_array_call->AddArray(array);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_array_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			FBi::XMLRPCArray array1 = resp->GetResult();
			if(array1.size() >= 6)
			{
				printf("%d\n", (int)array1[0]);
				printf("%d\n", (bool)array1[1]);
				printf("%s\n", static_cast<std::string>(array1[2]).c_str());
				printf("%f\n", (double)array1[3]);

				FBi::Properties s1 = array1[4];
				printf("%s\n", static_cast<std::string>(s1["PP"]["out"]).c_str());
				printf("%d\n", static_cast<int>(s1["PP"]["in"]));
				FBi::XMLRPCArray array2 = array1[5];
				printf("%s\n", static_cast<std::string>(array2[0]).c_str());
				printf("%d\n", static_cast<int>(array2[1]));

				return true;
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
	return false;
}

bool RPCRemoteController::SendTime(const FBi::XMLRPCDateTime& date_time)
{
	FBi::MethodCallPtr test_date_time_call(new FBi::MethodCall);
	test_date_time_call->SetMethodName("example.TestXMLRPCDateTime");
	test_date_time_call->AddDateTime(date_time);

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_date_time_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			FBi::XMLRPCDateTime date_time2 = resp->GetResult();
			printf("%s\n", date_time2.GetDateTimeString().c_str());
			return true;
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
	return false;
}

bool RPCRemoteController::SendUndefined()
{
	FBi::MethodCallPtr test_undefined_call(new FBi::MethodCall);
	test_undefined_call->SetMethodName("example.Undefined");

	FBi::MethodResponsePtr resp = m_rpc_session->Call(test_undefined_call);
	if (resp.get())
	{
		if(resp->IsOK())
		{
			printf("It is impossible.\n");
			return true;
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
	return false;
}

bool RPCRemoteController::SendTest()
{
	bool bRet = false;
	bRet =	SendInt(283);
	bRet =  SendBool(false);
	bRet =  SendString("C++");
	bRet =  SendDouble(3.14159);
	bRet =  SendTwoParams(911.112233, 5566);

	FBi::Properties s1;
	s1["string"] = "test String";
	s1["double"] = 0.375;
	s1["integer"] = 12345;
	s1["boolean"] = true;
	s1["struct"]["Apple"] = "steve jobs";
	s1["struct"]["OMG"] = false;
	bRet =  SendStruct(s1);

	FBi::XMLRPCArray array;
	array.push_back(95);
	array.push_back(true);
	array.push_back("Pig");
	array.push_back(-0.96);
	FBi::Properties s2;
	s2["camera1"] = "dog";
	s2["camera2"] = "cat";
	array.push_back(s2);

	FBi::XMLRPCArray array2;
	array2.push_back(1234);
	array2.push_back("one two three four");
	array.push_back(array2);
	bRet =  SendArray(array);

	FBi::XMLRPCDateTime date_time;
	date_time.SetDateTime("1988-07-07T12:34:21");
	bRet =  SendTime(date_time);

	bRet =  SendUndefined();
	return bRet;
}
