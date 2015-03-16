#include <iostream>
#include <cstdio>
#include <libtransport/rpc_session.hpp>
#include <libcommon/properties.hpp>

#define SESSION_ID_KEY_NAME "session_id"
#define QUERY_STRING_KEY_NAME "query_str"

void notify_session_callback(const char* data, size_t size)
{
	std::cout << std::endl << "\n======= Get data ======== size = " << size << std::endl;
	std::string temp_data(data, size);
	std::cout << temp_data << std::endl;

	if(size == 0)
		return;

	std::cout << "\n---------- parse response ----------" << std::endl;
	instek::MethodResponsePtr resp(new instek::MethodResponse);
	resp->Parse(data, size);
	resp->ComposeTree();

	if(!resp)
	{
		std::cout << "The response of server push is NULL" << std::endl;
		return;
	}

	if(!resp->IsOK())
	{
		std::cout << "error.............code: " << resp->GetErrValue() << " , msg: " << resp->GetErrMsg() << std::endl;
		return;
	}

	instek::Properties prop = static_cast<instek::Properties>(resp->GetResult());

	if(prop.Empty())
	{
		std::cout << "This property is empty." << std::endl;
		return;
	}

	// Get the camera name and status.
	instek::Properties::Container* cam_container = prop["cam"].GetContainerPtr();
	instek::Properties::Container::iterator cam_itor = cam_container->begin();
	std::vector<int> channels;
	for (; cam_itor != cam_container->end(); ++cam_itor)
	{
		std::cout << "ID = " << cam_itor->first;

		instek::Properties::Container* id_container = cam_itor->second.value.GetContainerPtr();

		instek::Properties::Container::iterator itor = id_container->find("name");
		if (itor != id_container->end())
		{
			std::cout << ", Name = " << static_cast<std::string>(itor->second.value);
		}

		itor = id_container->find("status");
		if (itor != id_container->end())
		{
			std::cout << ", Status = " << static_cast<std::string>(itor->second.value);
		}

		std::cout << std::endl;
	}
}

int main()
{
	instek::RPCSession<instek::SSL_SOCK> rpc_session("172.16.3.33", 60009, true, false);
	instek::NotifySession session("172.16.3.33", 60009, &notify_session_callback);

	// Login the server.
        instek::MethodCallPtr login_call(new instek::MethodCall);
        login_call->SetMethodName("server.login");
        login_call->AddString("admin");
        login_call->AddString("id");
        login_call->AddString("client");

        //rpc_session_.SetTimeoutValue(3000);

        instek::MethodResponsePtr resp = rpc_session.Call(login_call);
	std::string session_id;

	if(!resp)
	{
		std::cout << "The response of login call is NULL" << std::endl;
		return -1;
	}

	if(!resp->IsOK())
	{
		std::cout << "error.............code: " << resp->GetErrValue() << " , msg: " << resp->GetErrMsg() << std::endl;
		return -1;
	}

	// Get session ID.
	session_id = static_cast<std::string>(resp->GetResult());
	std::cout << "Session ID = " << session_id << std::endl;

	// Prepare the notify session and start it.
	std::string path = "/notify.cgi?" SESSION_ID_KEY_NAME "=";
	// %20 or + is one space in the HTTP query string.
	path += (session_id + "&" QUERY_STRING_KEY_NAME "=cam:*:name%20cam:*:status");
	session.Start(path);

	// Wait to quit.
	printf("\ntype 'q' to quit.\n\n");
	char quit_char;
	do
	{
		scanf("%c", &quit_char);
	} while (quit_char != 'q');

	session.Stop();

	return 0;
}
