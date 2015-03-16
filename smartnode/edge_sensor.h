#ifndef _EDGE_SENSOR_H_
#define _EDGE_SENSOR_H_

#include <iostream> 
#include <string>
#include <cstdlib>
#include <boost/thread/mutex.hpp>

#include "rf24.h"

class EdgeSensor
{
public:
	EdgeSensor();
	~EdgeSensor();

	Sensor& Get();
	void Set(const Sensor&);

private:
	Sensor m_sensor;
};

#endif // _EDGE_SENSOR_H_
