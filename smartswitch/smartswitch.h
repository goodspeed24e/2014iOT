#ifndef _SMART_SWITCH_H
#define _SMART_SWITCH_H

#include "xml_rpc_server.h"
#include "sensor_prototype.h"

class SmartSwitch
{
public:
	SmartSwitch(const int&, const int&);
	~SmartSwitch();

private:
	//RPCSvr m_rpcSvr;
	SensorFactory m_sensorRf;
};

#endif // _SMART_SWITCH_H
