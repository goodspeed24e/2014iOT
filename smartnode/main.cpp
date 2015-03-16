#include <iostream>
#include <string>
#include <cstdlib>

#include "smartnode.h"

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		std::cout << "Usage: " << argv[0] << "<StopServer>(0: false, 1: true)" << std::endl;
		return -1;
	}

	std::cout << "Server IP: "		<< argv[2] << std::endl;
	std::cout << "Remote Port: "	<< argv[3] << std::endl;

	/*
	int is_stop_server = atoi(argv[1]);
	if(is_stop_server)
	{
		FBi::MethodCallPtr stop_server_call(new FBi::MethodCall);
		stop_server_call->SetMethodName("example.StopServer");
		resp = m_rpc_session->Call(stop_server_call);
	}
	*/

	SmartNode node(argv[2], atoi(argv[3]));

	printf("type 'q' to quit.\n");
	char quit_char;
	do
	{
		scanf("%c",&quit_char);
	} while (quit_char != 'q');

	return 0;
}
