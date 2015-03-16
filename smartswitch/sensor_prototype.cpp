#include "sensor_prototype.h"

///////////////////////
Stooge*   FlameDetector::clone()
{ 
	return new FlameDetector;
}

void FlameDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_FLAME;

	config(NULL);
}

void FlameDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n FlameDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool FlameDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n FlameDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n FlameDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n FlameDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}

	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   GasDetector::clone()
{ 
	return new GasDetector;
}

void GasDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_GAS;

	config(NULL);
}

void GasDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n GasDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool GasDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n GasDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n GasDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n GasDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}

	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   CODetector::clone()
{ 
	return new CODetector;
}

void CODetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_CO;

	config(NULL);
}

void CODetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n CODetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool CODetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n CODetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n CODetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n CODetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}

	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   WaterDetector::clone()
{ 
	return new WaterDetector;
}

void WaterDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_WATER;

	config(NULL);
}

void WaterDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n CODetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool WaterDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n CODetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n CODetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n CODetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}

	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   NoiseDetector::clone()
{ 
	return new NoiseDetector;
}

void NoiseDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_NOISE;

	config(NULL);
}

void NoiseDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n NoiseDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool NoiseDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n NoiseDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n NoiseDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n NoiseDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}

	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   TemperatureDetector::clone()
{ 
	return new TemperatureDetector;
}

void TemperatureDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_TEMPERATURE;

	config(NULL);
}

void TemperatureDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n TemperatureDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool TemperatureDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n TemperatureDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n TemperatureDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n TemperatureDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   RfidDetector::clone()
{ 
	return new RfidDetector;
}

void RfidDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_RFID;

	config(NULL);
}

void RfidDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n RfidDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool RfidDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n RfidDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n RfidDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n RfidDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   ShockDetector::clone()
{ 
	return new ShockDetector;
}

void ShockDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_SHOCK;

	config(NULL);
}

void ShockDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n ShockDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool ShockDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n ShockDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n ShockDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n ShockDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   IntrusionDetector::clone()
{ 
	return new IntrusionDetector;
}

void IntrusionDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_INTRUSION;

	config(NULL);
}

void IntrusionDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n IntrusionDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool IntrusionDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n IntrusionDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n IntrusionDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n IntrusionDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   MageticDetector::clone()
{ 
	return new MageticDetector;
}

void MageticDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_MAGETIC;

	config(NULL);
}

void MageticDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n MageticDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool MageticDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n MageticDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n MageticDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n MageticDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   PhotocellDetector::clone()
{ 
	return new PhotocellDetector;
}

void PhotocellDetector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_PHOTOCELL;

	config(NULL);
}

void PhotocellDetector::config(const ActionReponse* ptrA)
{
	std::cout << "\n PhotocellDetector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool PhotocellDetector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n PhotocellDetector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n PhotocellDetector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n PhotocellDetector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}
///////////////////////
///////////////////////
Stooge*   Detector::clone()
{ 
	return new Detector;
}

void Detector::reset()
{
	memset(&sensor_, 0, sizeof(Sensor));
	sensor_.sensor_type_ = SENSOR_TYPE_UNKNOWN;

	config(NULL);
}

void Detector::config(const ActionReponse* ptrA)
{
	std::cout << "\n Detector creation";
	boost::mutex::scoped_lock lock(mutex_);

	if (ptrA != NULL)
	{
		memcpy(&sensor_.action_reponse_, ptrA, sizeof(ActionReponse));
	}
	else
	{
		sensor_.action_reponse_.warning_ = 100;
		sensor_.action_reponse_.warning_type_ = ACTION_TYPE_BEE;
		sensor_.action_reponse_.alarm_ = 200;
		sensor_.action_reponse_.alarm_type_ = ACTION_TYPE_ALARM;
		sensor_.action_reponse_.urgent_ = 300;
		sensor_.action_reponse_.urgent_type_ = ACTION_TYPE_PHONE;
	}
}

bool Detector::update(Sensor& s, ACTION_TYPE& actionVal)
{
	boost::mutex::scoped_lock lock(mutex_);

	bool bUpdate = true;
	if (s.sensor_value_ > sensor_.action_reponse_.urgent_)
	{ 
		std::cout << "\n Detector update - URGENT";
		s.sensor_value_ = -sensor_.action_reponse_.urgent_;
		actionVal = s.action_reponse_.urgent_type_ =  sensor_.action_reponse_.urgent_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.alarm_)
	{
		std::cout << "\n Detector update - ALARM";
		s.sensor_value_ = -sensor_.action_reponse_.alarm_;
		actionVal = s.action_reponse_.alarm_type_ =  sensor_.action_reponse_.alarm_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else if (s.sensor_value_ > sensor_.action_reponse_.warning_)
	{
		std::cout << "\n Detector update - WARNING";
		s.sensor_value_ = -sensor_.action_reponse_.warning_;
		actionVal = s.action_reponse_.warning_type_ =  sensor_.action_reponse_.warning_type_;
		sensor_.status_ = SENSOR_STATUS_RESET;
	}
	else
	{
		sensor_.status_ = SENSOR_STATUS_ON;
		bUpdate = false;
	}
	return bUpdate;
}

///////////////////////

SensorFactory::SensorFactory(const int& channelID)
	: m_rf24(this, channelID)
{
	load();

	bool bRet = false;
	while(1)
	{
		if (!bRet) 
		{
			m_rf24.Setup();
			bRet = true;
		}
		m_rf24.Loop();
	}
}

SensorFactory::~SensorFactory()
{
    for (unsigned int i=0; i < m_sensors.size(); ++i)
        delete m_sensors[i];
}

Stooge* SensorFactory::s_prototype[] = {
	new Detector, new FlameDetector, new GasDetector, new CODetector \
  , new WaterDetector, new NoiseDetector, new TemperatureDetector \
  , new RfidDetector,  new ShockDetector, new IntrusionDetector \
  , new MageticDetector, new PhotocellDetector, new Detector \
  , new Detector, new Detector
};

Stooge* SensorFactory::make_stooge(const int& choice)
{
	return s_prototype[choice]->clone();
}

void SensorFactory::load( void )
{
	int len = sizeof(s_prototype)/sizeof(Stooge*);
	
	for (int i = 0; i < len; ++i) 
		m_sensors.push_back(SensorFactory::make_stooge(i));

	for (unsigned int i=0; i < m_sensors.size(); ++i)
		m_sensors[i]->reset();
}

bool SensorFactory::UpdateSensor(Sensor& s, ACTION_TYPE& v)
{
	switch (s.sensor_type_) 
	{
		case SENSOR_TYPE_FLAME: 
			return m_sensors[SENSOR_TYPE_FLAME]->update(s,v);
		case SENSOR_TYPE_GAS: 
			return m_sensors[SENSOR_TYPE_GAS]->update(s,v);
        case SENSOR_TYPE_CO: 
			return m_sensors[SENSOR_TYPE_CO]->update(s,v);
        case SENSOR_TYPE_WATER: 
			return m_sensors[SENSOR_TYPE_WATER]->update(s,v);
		case SENSOR_TYPE_NOISE: 
			return m_sensors[SENSOR_TYPE_NOISE]->update(s,v);
		case SENSOR_TYPE_TEMPERATURE: 
			return m_sensors[SENSOR_TYPE_TEMPERATURE]->update(s,v);
		case SENSOR_TYPE_RFID: 			
			return m_sensors[SENSOR_TYPE_RFID]->update(s,v);
		case SENSOR_TYPE_SHOCK: 			
			return m_sensors[SENSOR_TYPE_SHOCK]->update(s,v);
		case SENSOR_TYPE_INTRUSION: 
			return m_sensors[SENSOR_TYPE_INTRUSION]->update(s,v);
		case SENSOR_TYPE_MAGETIC:
			return m_sensors[SENSOR_TYPE_MAGETIC]->update(s,v);
		case SENSOR_TYPE_PHOTOCELL: 
			return m_sensors[SENSOR_TYPE_PHOTOCELL]->update(s,v);
		default:
				break;
	}
	return false;
}

FBi::Properties SensorFactory::QuerySensorStatus()
{
	int len = sizeof(SENSOR_NAME)/sizeof(*SENSOR_NAME);
	for (int i = 0; i < len; i++)
	{
		if (m_sensors[i]->sensor_.status_ != SENSOR_STATUS_OFF)
		{
			m_sensor_props["sensor"][SENSOR_NAME[i]]["channel"]		= m_sensors[i]->sensor_.channel_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["pipe"]		= m_sensors[i]->sensor_.pipe_id_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["value"]		= m_sensors[i]->sensor_.sensor_value_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["timestamp"]	= (int)m_sensors[i]->sensor_.time_stamp_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["status"]		= m_sensors[i]->sensor_.status_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["warningType"]	= m_sensors[i]->sensor_.action_reponse_.warning_type_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["warningValue"]	= m_sensors[i]->sensor_.action_reponse_.warning_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["alarmType"]		= m_sensors[i]->sensor_.action_reponse_.alarm_type_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["alarmValue"]	= m_sensors[i]->sensor_.action_reponse_.alarm_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["urgentType"]	= m_sensors[i]->sensor_.action_reponse_.urgent_type_;
			m_sensor_props["sensor"][SENSOR_NAME[i]]["action"]["urgentValue"]	= m_sensors[i]->sensor_.action_reponse_.urgent_;
		}
	}

	if (0)
	{
		m_sensor_props.Save("c://sensor_props_switchswitch.xml");
	}

	return m_sensor_props;
}

FBi::Properties SensorFactory::SetSensorStatus(FBi::Properties& props)
{
	if (0)
	{
		props.Save("c://sensor_props_switch_new.xml");
	}

	FBi::Properties::Container::iterator sensorIter = props["sensor"].GetContainerPtr()->begin();
	for(; sensorIter != props["sensor"].GetContainerPtr()->end(); ++sensorIter)
	{
		try
		{
			std::string sensorType = sensorIter->first;
			m_sensor_props["sensor"][sensorType] = sensorIter->second.value;
		}
		catch(...)
		{
			std::cout << "\n SensorFactory::SetSensorStatus ERROR!!!";
		}

		if (0)
		{
			m_sensor_props.Save("c://sensor_props_update.xml");
		}
	}
	
	return m_sensor_props;
}