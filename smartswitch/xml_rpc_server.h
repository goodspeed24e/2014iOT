#ifndef _XML_RPC_SERVER
#define _XML_RPC_SERVER

#include <iostream>
#include <cstdlib>
#include <string>

#include <boost/bind.hpp>
#include <xmlrpc_server/xmlrpc_server.hpp>
#include <xmlrpc_server/xmlrpc_utility.hpp>
#include <http_server/http_server.hpp>
#include <http_server/http_request_dispatcher.hpp>
#include <http_server/notify_request_handler.hpp>

class SensorFactory;
class RPCSvr
{
public:
	RPCSvr(SensorFactory*, const int&);
	~RPCSvr();

	// RPC function
	void GetSensorStatus(const FBi::XMLRPC_Value_List& calldata, FBi::MethodResponse& resp);
	void SetSensorStatus(const FBi::XMLRPC_Value_List& calldata, FBi::MethodResponse& resp);

	void callback1(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void callback2(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestInt(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestBool(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestString(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestDouble(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestStruct(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestTwoParams(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void UnPublicIntrospectionFunc(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestArray(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void StopServer(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestThrowFault(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestEmptyStruct(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestEmptydArray(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestComplicatedStruct(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestComplicatedArray(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestAddValue(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp);
	void TestXMLRPCDateTime(const FBi::XMLRPC_Value_List& vector, FBi::MethodResponse& resp);
	void TestGetConnectionInfo(const FBi::XMLRPC_Value_List& /*vector*/, FBi::MethodResponse& resp, const FBi::HttpConnectionInfo& info);

private:
	FBi::XMLRPC_Server* server_;
	int rpc_port_;

	SensorFactory* sensor_factory_;	// weak reference
};

#endif // _XML_RPC_SERVER