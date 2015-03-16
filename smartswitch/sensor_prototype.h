#ifndef _SENSOR_PROTOTYPE_H
#define _SENSOR_PROTOTYPE_H

#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/thread/condition_variable.hpp>
#include <libcommon/properties.hpp>

#include <iostream>
#include <vector>

#include "rf24.h"
#include "sensor_trans.h"

class Stooge
{
public:
	virtual Stooge* clone() = 0;
	virtual void config(const ActionReponse*) = 0;
	virtual void reset() = 0;
	virtual bool update(Sensor&, ACTION_TYPE&) = 0;

public:
	boost::mutex mutex_;
	Sensor sensor_;
};

class SensorFactory 
{
public:
	SensorFactory(const int&);
	~SensorFactory();

	static Stooge* make_stooge(const int&);

	bool UpdateSensor(Sensor&, ACTION_TYPE&);
	FBi::Properties QuerySensorStatus();
	FBi::Properties SetSensorStatus(FBi::Properties&);

private:
	void load();

private:
	static Stooge* s_prototype[SENSOR_TYPE_MAX];
	std::vector<Stooge*> m_sensors;
	RF24Wrapper m_rf24;

	FBi::Properties m_sensor_props;
};

class FlameDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class GasDetector: public Stooge
{
public:
	Stooge* clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class CODetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class WaterDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class NoiseDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class TemperatureDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class RfidDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class ShockDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class IntrusionDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class MageticDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class PhotocellDetector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

class Detector: public Stooge
{
public:
	Stooge*   clone();
	void reset();
	void config(const ActionReponse*);
	bool update(Sensor&, ACTION_TYPE&);
};

#endif // _SENSOR_PROTOTYPE_H
