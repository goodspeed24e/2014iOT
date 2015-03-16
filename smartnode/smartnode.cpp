#include "smartnode.h"
#include "rpc_remote_controller.h"
#include "GPIOClass.h"

static FBi::ServiceEngine rpc_client_engine_(boost::thread::hardware_concurrency());

SmartNode::SmartNode(const char* ip, const int& port)
	: m_timer(rpc_client_engine_.GetIOService())
	, gpioIpin(NULL), gpioOpin(NULL)
#ifndef WIN32
	, gpio27(27)
#endif
{
	// start service engine
	rpc_client_engine_.StartService();

	// save ip & the port number
	m_ip.append(ip);
	m_port = port;

	// initialize sensors
	int len = sizeof(SENSOR_NAME)/sizeof(*SENSOR_NAME);
	for (int i=0; i<len; i++) 
	{
		EdgeSensor s;
		m_EdgeSensor.insert(std::pair<std::string,EdgeSensor>(SENSOR_NAME[i],s));
	}

	m_rpcRC		= new RPCRemoteController(this);
	m_httpSvr	= new HttpSvr(this);

	installGPio("4", "27");

#ifndef WIN32
	gpio27.SetDirection(true);
#endif

	FBi::Properties tour;
	start(tour);
}

SmartNode::~SmartNode()
{
	uninstallGPio();

	delete m_rpcRC;
	delete m_httpSvr;
}

void SmartNode::start(FBi::Properties& sTour)
{
	stop();
	boost::mutex::scoped_lock lock(m_lock_engine);

	m_timer.expires_from_now(boost::posix_time::milliseconds(16));
	m_timer.async_wait(FBi::make_custom_alloc_handler(m_allocator,
		boost::bind(&SmartNode::run, this, boost::asio::placeholders::error)));
}

void SmartNode::run(const boost::system::error_code& err)
{
	boost::mutex::scoped_lock lock(m_lock_engine);
	if (err == boost::asio::error::operation_aborted)
	{
		return;
	}
	
	FBi::Properties sensorProps;
	m_rpcRC->GetSensorStatus(sensorProps);
	if (0)
	{
		sensorProps.Save("c://sensor_props_smartnode.xml");
	}
	//this->update_sensor(sensorProps);
	
	m_timer.expires_from_now(boost::posix_time::milliseconds(8));
	m_timer.async_wait(FBi::make_custom_alloc_handler(m_allocator,
		boost::bind(&SmartNode::run, this, boost::asio::placeholders::error)));
}

void SmartNode::stop()
{
}

SENSOR_TYPE SmartNode::query_sensor_type(const std::string& t)
{
	int len = sizeof(SENSOR_NAME)/sizeof(*SENSOR_NAME);
	for (int i = 0; i < len; i++)
	{
		if (SENSOR_NAME[i] == t)
			return (SENSOR_TYPE)i;
	}
	return SENSOR_TYPE_UNKNOWN;
}

ACTION_TYPE SmartNode::query_sensor_action(const std::string& a)
{
	int len = sizeof(SENSOR_ACTION)/sizeof(*SENSOR_ACTION);
	for (int i = 0; i < len; i++)
	{
		if (SENSOR_ACTION[i] == a)
			return (ACTION_TYPE)i;
	}
	return ACTION_TYPE_DONTCARE;
}

/*
bool SmartNode::update_sensor(FBi::Properties& Props)
{
	std::map<std::string,EdgeSensor>::iterator it;

	FBi::Properties::Container::iterator sensorIter = Props["sensor"].GetContainerPtr()->begin();
	for(; sensorIter != Props["sensor"].GetContainerPtr()->end(); ++sensorIter)
	{
		try
		{
			std::string sensorType = sensorIter->first;
			it = m_EdgeSensor.find(sensorType);

			if (it != m_EdgeSensor.end())
			{
				Sensor s;
				s.channel_						= Props["sensor"][sensorType]["channel"];
				s.pipe_id_						= Props["sensor"][sensorType]["pipe"];
				s.sensor_value_					= Props["sensor"][sensorType]["value"];
				s.sensor_type_					= query_sensor_type(sensorType);
				s.time_stamp_					= (unsigned int)(int)Props["sensor"][sensorType]["timestamp"];
				s.status_						= (SENSOR_STATUS) (int)Props["sensor"][sensorType]["status"];
				s.action_reponse_.warning_type_ = (ACTION_TYPE) (int)Props["sensor"][sensorType]["action"]["warningType"];
				s.action_reponse_.warning_		= Props["sensor"][sensorType]["action"]["warningValue"];
				s.action_reponse_.alarm_type_	= (ACTION_TYPE) (int)Props["sensor"][sensorType]["action"]["alarmType"];
				s.action_reponse_.alarm_		= Props["sensor"][sensorType]["action"]["alarmValue"];
				s.action_reponse_.urgent_type_	= (ACTION_TYPE) (int)Props["sensor"][sensorType]["action"]["urgentType"];
				s.action_reponse_.urgent_		= Props["sensor"][sensorType]["action"]["urgentValue"];

				{
					boost::mutex::scoped_lock lock(m_lock_sensor);
					it->second.Set(s);
				}
			}
		}
		catch (...)
		{
			std::cout << "\n SensorFactory::SetSensorStatus ERROR!!!";
		}

		Props.Save("c://sensor_props_update.xml");
	}

	return true;
}
*/
void SmartNode::SetSensor(const std::string& msg)
{
	// 1. message parsing
	std::vector<std::string> tokens;
	std::stringstream ss(msg);
				
	std::string buf;
	while (ss >> buf) 
	{
		tokens.push_back(buf);
		std::cout <<"\n buffer: " << buf;
	}

	// 2. setup sensor properties
	Sensor s;
	{
		s.channel_ = 1;
		s.pipe_id_ = 1;
		s.sensor_value_ = 1;
		s.time_stamp_ = 0;
		s.sensor_type_ = query_sensor_type(tokens[0]);
		s.status_ = SENSOR_STATUS_RESET;
		s.action_reponse_.warning_type_ = query_sensor_action(tokens[2]);
		s.action_reponse_.warning_		= atoi(tokens[1].c_str());
		s.action_reponse_.alarm_type_	= query_sensor_action(tokens[2]);
		s.action_reponse_.alarm_		= atoi(tokens[1].c_str());
		s.action_reponse_.urgent_type_	= query_sensor_action(tokens[2]);
		s.action_reponse_.urgent_		= atoi(tokens[1].c_str());
	}

	// 3. update sensor
	std::map<std::string,EdgeSensor>::iterator it;
	it = m_EdgeSensor.find(tokens[0]);
	if (it != m_EdgeSensor.end())
	{
		{
			boost::mutex::scoped_lock lock(m_lock_sensor);
			it->second.Set(s);
		}

		FBi::Properties Props;
		Props["sensor"][tokens[0]]["status"] = (int)s.status_;
		Props["sensor"][tokens[0]]["action"]["warningType"]	= (int)s.action_reponse_.warning_type_;
		Props["sensor"][tokens[0]]["action"]["warningValue"]= (int)s.action_reponse_.warning_;
		Props["sensor"][tokens[0]]["action"]["alarmType"]	= (int)s.action_reponse_.alarm_type_;
		Props["sensor"][tokens[0]]["action"]["alarmValue"]	= (int)s.action_reponse_.alarm_;
		Props["sensor"][tokens[0]]["action"]["urgentType"]	= (int)s.action_reponse_.urgent_type_;
		Props["sensor"][tokens[0]]["action"]["urgentValue"]	= (int)s.action_reponse_.urgent_;

		m_rpcRC->SetSensorStatus(Props);
		m_rpcRC->SendString(tokens[0]);
		m_rpcRC->SendString(tokens[1]);
		m_rpcRC->SendString(tokens[2]);
	}
}

Sensor SmartNode::GetSensor(const std::string& name)
{
	std::map<std::string,EdgeSensor>::iterator it;
	it = m_EdgeSensor.find(name);
	if (it != m_EdgeSensor.end())
		return it->second.Get();
	else
		return Sensor();
}

void SmartNode::installGPio(const std::string& iPin, const std::string& oPin)
{
	string inputstate;
	gpioIpin = new GPIOClass(iPin);
	gpioOpin = new GPIOClass(oPin);

	gpioIpin->export_gpio();
	gpioOpin->export_gpio(); 
	std::cout << " GPIO pins exported" << std::endl;

	gpioIpin->setdir_gpio("in");
	gpioOpin->setdir_gpio("out");
	std::cout << " Set GPIO pin directions" << std::endl;
}

bool SmartNode::TriggerGPio(const int& val)
{
	bool on = static_cast<bool>(val);
	cout << "TriggerGPio: " << val << std::endl;

#ifndef WIN32
	gpio27.SetValue(on);
#endif

	return true;
}

void SmartNode::uninstallGPio()
{
	delete gpioIpin;
	delete gpioOpin;
}
