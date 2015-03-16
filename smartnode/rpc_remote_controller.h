#ifndef _RPC_REMOTE_CONTROLLER_H_
#define _RPC_REMOTE_CONTROLLER_H_

#include <libtransport/rpc_session.hpp>
#include <libcommon/properties.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <string>
#include <cstdlib>

class SmartNode;
class RPCRemoteController
{
public:
	typedef FBi::RPCSession<FBi::SSL_SOCK> NowRPCSession;
	//typedef FBi::RPCSession<FBi::TCP_SOCK> NowRPCSession;

	RPCRemoteController(SmartNode*);
	virtual ~RPCRemoteController();

	// RPC function
	void GetSensorStatus(FBi::Properties&);
	bool SetSensorStatus(const FBi::Properties&);

	bool SendTest();
	bool SendInt(const int&);
	bool SendBool(const bool&); 
	bool SendString(const std::string&);
	bool SendDouble(const double&);
	bool SendStruct(const FBi::Properties&);
	bool SendTwoParams(const double&, const int&);
	bool SendArray(const FBi::XMLRPCArray&);
	bool SendTime(const FBi::XMLRPCDateTime&);
	bool SendUndefined();

private:
	NowRPCSession* m_rpc_session;
	SmartNode *m_sensor_node;		// weak reference
};

#endif // _RPC_REMOTE_CONTROLLER_H_