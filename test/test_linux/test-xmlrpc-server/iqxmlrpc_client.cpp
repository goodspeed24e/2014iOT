#include <iostream>
#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/http_client.h>

#include <libiqxmlrpc/https_client.h>

int main()
{
	using namespace iqxmlrpc;

	// 1: Create a HTTP client object
	Client<Http_client_connection> client(iqnet::Inet_addr("127.0.0.1", 60000));

	// 1. Create a HTTPs client object
	//iqnet::ssl::ctx = iqnet::ssl::Ctx::client_only();
	//Client<Https_client_connection> client(iqnet::Inet_addr("127.0.0.1", 60000), "/RPC2");

	// 2: Fill method parameters
	Param_list pl;
	pl.push_back(123);

	// 3: Call a remote method
	Response r = client.execute("example.TestInt", pl);

	// 4: Process method output
	std::cout << "The response is: " << r.value().get_int() << std::endl;
}
