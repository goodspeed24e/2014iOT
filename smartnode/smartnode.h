#ifndef _SMART_NODE_H_
#define _SMART_NODE_H_

#include <libtransport/rpc_session.hpp>
#include <libcommon/properties.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp> 

#include <iostream> 
#include <string>
#include <cstdlib>
#include <map>

#include "rf24.h"
#include "edge_sensor.h"
#include "rpc_remote_controller.h"
#include "httpSvr_out_layer.h"


#ifndef WIN32

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

class LinuxFile
{
private:
    int m_Handle;

public:
    LinuxFile(const char *pFile, int flags = O_RDWR)
    {
        m_Handle = open(pFile, flags);
    }

    ~LinuxFile()
    {
        if (m_Handle != -1)
            close(m_Handle);
    }

    size_t Write(const void *pBuffer, size_t size)
    {
        return write(m_Handle, pBuffer, size);
    }

    size_t Read(void *pBuffer, size_t size)
    {
        return read(m_Handle, pBuffer, size);
    }

    size_t Write(const char *pText)
    {
        return Write(pText, strlen(pText));
    }

    size_t Write(int number)
    {
        char szNum[32];
        snprintf(szNum, sizeof(szNum), "%d", number);
        return Write(szNum);
    }
};

class LinuxGPIOExporter
{
protected:
    int m_Number;

public:
    LinuxGPIOExporter(int number)
        : m_Number(number)
    {
        LinuxFile("/sys/class/gpio/export", O_WRONLY).Write(number);
    }

    ~LinuxGPIOExporter()
    {
        LinuxFile("/sys/class/gpio/unexport", 
                O_WRONLY).Write(m_Number);
    }
};

class LinuxGPIO : public LinuxGPIOExporter
{
public:
    LinuxGPIO(int number)
        : LinuxGPIOExporter(number)
    {
    }

    void SetValue(bool value)
    {
        char szFN[128];
        snprintf(szFN, sizeof(szFN), 
            "/sys/class/gpio/gpio%d/value", m_Number);
        LinuxFile(szFN).Write(value ? "1" : "0");
    }

    void SetDirection(bool isOutput)
    {
        char szFN[128];
        snprintf(szFN, sizeof(szFN), 
            "/sys/class/gpio/gpio%d/direction", m_Number);
        LinuxFile(szFN).Write(isOutput ? "out" : "in");
    }
};
#endif // WIN32


class GPIOClass;
class SmartNode
{
public:
	SmartNode(const char*, const int&);
	virtual ~SmartNode();

	std::string	GetRemoteIP()		const	{return m_ip;}
	int			GetRemotePort()		const	{return m_port;}
	RPCRemoteController* GetRPC()	const	{return m_rpcRC;}
	
	void	SetSensor(const std::string&);
	Sensor	GetSensor(const std::string&);
	
	void	installGPio(const std::string&, const std::string&);
	bool	TriggerGPio(const int&);
	void	uninstallGPio();

private:
	void start(FBi::Properties& sTour);
	void stop();
	void run(const boost::system::error_code&);
	//bool update_sensor(FBi::Properties&);

	SENSOR_TYPE	query_sensor_type(const std::string&);
	ACTION_TYPE	query_sensor_action(const std::string&);

private:
	boost::mutex m_lock_engine;
	boost::asio::deadline_timer m_timer;
	FBi::sb_allocator m_allocator;

	RPCRemoteController* m_rpcRC;
	HttpSvr*			 m_httpSvr;

	boost::mutex m_lock_sensor;
	std::map<std::string, EdgeSensor> m_EdgeSensor;

	std::string m_ip;
	int m_port;

	// GPIO pin
	GPIOClass* gpioIpin;
	GPIOClass* gpioOpin;

#ifndef WIN32
	LinuxGPIO gpio27;
#endif
};
#endif // _SMART_NODE_H_
