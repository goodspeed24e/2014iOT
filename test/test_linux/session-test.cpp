#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <libtransport/rpc_session.hpp>
#include <iostream>
#include <libcommon/properties.hpp>
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace instek;

////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

//instek::RPCSession<instek::SSL_SOCK> rpc_session_("172.16.3.50", 60009, true, false);
//instek::RPCSession<instek::SSL_SOCK> rpc_session_("172.16.2.116", 60009, true, false);
//instek::RPCSession<instek::SSL_SOCK> rpc_session_("172.16.0.119", 60009, true, false);
instek::RPCSession<instek::SSL_SOCK> rpc_session_("172.16.3.148", 60009, true, false);
instek::MethodCallPtr notify_call;
void foo(const instek::MethodResponsePtr& resp)
{
	cout << "Async call back function: foo()" << endl;
	if (!resp.get() || !resp->IsOK())
	{
		cout << "error" << endl;
		cout << resp->GetXML() << endl;
		return;
	}


	string buf = resp->GetXML();

	cout << buf << endl;
	cout << "===============================" << endl;

	rpc_session_.Call(notify_call, (instek::RPCSessionFuncCallback) foo);
}

int main()
{	
	//"duck type" -like 
	Properties c1;
	c1["aa"] = 3;
	int value = c1["aa"];
	c1["aa"] = "duck type";
	std::string str = c1["aa"];

	std::cout << value << " " << str << std::endl;

	//Nested Container concept.
	std::cout << "STL only presents 'copy' behavior: " << std::endl;
	std::map<int, std::map<int, int> > nested_map;
	std::map<int, int> sub_map;
	nested_map[1] = sub_map;
	sub_map[2] = 3;
	std::cout << nested_map[1][2] << "\t--> we expect the value is 3 but 0" << std::endl;

	Properties nested_settings;
	Properties sub_settings;
	nested_settings["VideoStream"] = sub_settings;
	sub_settings["Resolution"] = "D1";
	std::cout << "VideoStream:FPS -- >" << (std::string) nested_settings["VideoStream"]["Resolution"] << std::endl;

	//auto-extension
	Properties c2;
	c2["Motion"]["Window1"]["Sensitivity"] = 15;
	std::cout << (int) c2["Motion"]["Window1"]["Sensitivity"] << std::endl;

	//serialize
	cout << "========= Serialization/UnSerialization ==========" << endl;
	Properties s1;
	s1["aa"] = 3;
	s1["bb"] = "abcde";
	s1["cc"]["test"] = 4;

	s1.Save("/tmp/a.cfg");


	Properties s2;
	s2.Load("/tmp/a.cfg");

	cout << "================= result ======== " << endl;
	cout << (int)s2["aa"] << endl;
	cout << (std::string) s2["bb"] << endl;
	cout << (int) s2["cc"]["test"] << endl;

	instek::MethodCallPtr login_call(new instek::MethodCall);
	login_call->SetMethodName("server.login");
	login_call->AddString("admin");
	login_call->AddString("id");
	login_call->AddString("client");

	//rpc_session_.SetTimeoutValue(3000);

	cout << login_call->GetXML() << endl;

	instek::MethodResponsePtr resp = rpc_session_.Call(login_call);

	if(resp)
	{
		if(resp->IsOK())
		{
			printf("Session ID = %s\n", ( (std::string) (resp->GetResult()) ).c_str());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}

		cout << "============= print session id ==================== " << endl;
		cout << resp->GetXML() << endl;
		cout << "=================================================== " << endl;
	}
	else
	{
		printf("The response of login call is NULL\n");
	}

	instek::MethodCallPtr get_prop_call(new instek::MethodCall);
	get_prop_call->SetMethodName("server.getProps");
	get_prop_call->AddString("cam:3,4,5:name");
	get_prop_call->AddBool(true);
	std::string strRe = resp->GetResult();
	get_prop_call->AddString(strRe); //session id
	resp = rpc_session_.Call(get_prop_call);

	std::string stru = get_prop_call->GetXML();
	cout << stru << endl;

	if(resp)
	{
		if(resp->IsOK())
		{
			instek::Properties temp_prop = resp->GetResult();
			printf("cam 3 = %s\n", ((std::string)temp_prop["cam"]["3"]["name"]).c_str());
			printf("cam 4 = %s\n", ((std::string)temp_prop["cam"]["4"]["name"]).c_str());
			printf("cam 5 = %s\n", ((std::string)temp_prop["cam"]["5"]["name"]).c_str());
		}
		else
		{
			printf("error.............code: %d msg: %s\n", resp->GetErrValue(), resp->GetErrMsg().c_str());
		}


		cout << "============= print response of getProps ==================== " << endl;
		cout << resp->GetXML() << endl;
		cout << "=================================================== " << endl;
	}
	else
	{
		printf("The response of getProps call is NULL\n");
	}

	notify_call.reset(new instek::MethodCall);
	notify_call->SetMethodName("server.notify");
	notify_call->AddInteger(5);
	notify_call->AddString(strRe);

	rpc_session_.Call(notify_call, (instek::RPCSessionFuncCallback) foo);

	sleep(100);
	return 1;
}
