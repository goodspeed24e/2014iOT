#include <iostream>

#include <ulxmlrpcpp/ulxmlrpcpp.h>

#include <ulxmlrpcpp/ulxr_http_protocol.h>
#include <ulxmlrpcpp/ulxr_tcpip_connection.h>
#include <ulxmlrpcpp/ulxr_requester.h>
#include <ulxmlrpcpp/ulxr_call.h>
#include <ulxmlrpcpp/ulxr_response.h>
#include <ulxmlrpcpp/ulxr_value.h>

int main()
{

	ulxr::TcpIpConnection conn(false, "localhost", 60000);
	ulxr::HttpProtocol prot(&conn);
	ulxr::Requester client(&prot);

	ulxr::MethodCall testcall ("example.TestInt");
	testcall.addParam(ulxr::Integer(123));

	ulxr::MethodResponse resp = client.call(testcall, "");
//	std::cout << "call result: \n" << resp.getXml(0) << std::endl;
	std::cout << "call result: " << static_cast<ulxr::Integer>(resp.getResult()).getInteger() << std::endl;
	return 0;
}
