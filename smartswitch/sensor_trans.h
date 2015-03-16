#ifndef _SENSOR_TRANS_H
#define _SENSOR_TRANS_H

#include <cstdlib>
#include <iostream>
#include <stdint.h>

#ifndef WIN32
#include <librf24/RF24.h>
#endif

#include "rf24.h"

class SensorFactory;
class RF24Wrapper
{
public:
	RF24Wrapper(SensorFactory*, const int&);
	~RF24Wrapper();

	void Setup();
	void Loop();

private:
	SensorFactory *m_sensorFactory;

#ifndef WIN32
	// CE and CSN pins On header using GPIO numbering (not pin numbers)
	RF24 radio; // Setup for GPIO 25 CSN
#endif

	int m_ChannelID;
};

#endif // _SENSOR_TRANS_H 
