#include "smartswitch.h"

SmartSwitch::SmartSwitch(const int& nPort, const int& nChannel)
	: m_sensorRf(nChannel)
	//, m_rpcSvr(&m_sensorRf, nPort)
{
}

SmartSwitch::~SmartSwitch()
{
}
