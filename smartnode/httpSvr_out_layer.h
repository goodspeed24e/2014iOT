#ifndef _HTTPSVR_OUT_LAYOUT_H_
#define _HTTPSVR_OUT_LAYOUT_H_

#include <libtransport/rpc_session.hpp>
#include <libcommon/properties.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp> 

#include <iostream> 
#include <string>
#include <cstdlib>

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#endif

#include "rf24.h"
#include "edge_sensor.h"

class SmartNode;
class HttpSvr
{
public:
	HttpSvr(SmartNode*);
	virtual ~HttpSvr();

private:
	void start(FBi::Properties&);
	void stop();
	void loop();

	void set(const std::string&);
	void get(const std::string&);
	bool parseMsg(const std::string&);

private:
	int sock;
	int recvd;
	int snd;
	int structlength;

#ifndef WIN32
	struct sockaddr_in server;
	struct sockaddr_in client;
#endif

	static const char *server_ip;
	static const int port;

	char recvBuf[2048];
	char sendBuf[2048];
	char message[2048];

	boost::shared_ptr<boost::thread>   m_http_thread;
	SmartNode* m_sensor_node;	// weak reference
};

#endif // _HTTPSVR_OUT_LAYOUT_H_
