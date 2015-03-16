#include <stdio.h>
#include <string.h>
#include "edge_sensor.h"

EdgeSensor::EdgeSensor()
{
	memset(&m_sensor, 0, sizeof(Sensor));
}

EdgeSensor::~EdgeSensor()
{
}

Sensor& EdgeSensor::Get()
{
	return m_sensor;
}

void EdgeSensor::Set(const Sensor& s)
{
	m_sensor = s;
}
