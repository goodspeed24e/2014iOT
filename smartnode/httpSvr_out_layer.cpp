#include "httpSvr_out_layer.h"
#include "smartnode.h"

const std::string MessageTag[]={"[T]:", "[S]:", "[R]:"};
const char *HttpSvr::server_ip = "127.0.0.1";
const int HttpSvr::port = 8888;

HttpSvr::HttpSvr(SmartNode* node)
	: sock(0), recvd(0), snd(0), structlength(0),
	m_sensor_node(node)
{
	memset(recvBuf, 0, sizeof(recvBuf));
	memset(sendBuf, 0, sizeof(sendBuf));
	memset(message, 0, sizeof(message));
	strcpy(message,"I have received: \n");

	FBi::Properties tour;
	this->start(tour);
}

HttpSvr::~HttpSvr()
{
}

void HttpSvr::start(FBi::Properties& props)
{
#ifndef WIN32
	memset((char *)&server,0,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(server_ip);
	server.sin_port = htons(port);

	memset((char *)&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(port);

	if ((sock = socket (AF_INET,SOCK_DGRAM,0)) < 0 ) 
	{
		printf("socket create error!\n");
		return;
	}

	structlength = sizeof(server);
	if (::bind(sock,(struct sockaddr *) &server, structlength) < 0) 
	{
		printf("socket bind error!\n");
		perror("bind");
		return;
	}
#endif

	// std::cout << "Main Thread: " << boost::this_thread::get_id() << std::endl; 
	// lazy initialization - enable zoom in/out thread
	if (m_http_thread == NULL)
	{
		m_http_thread.reset(new boost::thread(
			boost::bind(&HttpSvr::loop, this)));
	}
}

void HttpSvr::loop()
{
	bool getphpCmd = 0;
	try {
		while(1)
		{
#ifndef WIN32
			structlength = sizeof(client); 

			recvd = recvfrom(sock,recvBuf,sizeof(recvBuf),0,(struct sockaddr *) & client, (socklen_t*)&structlength);
			if(recvd < 0)
			{
				perror("recvfrom"); 
				exit(EXIT_FAILURE);
			}
			else
			{
				printf("\n 1. received: %s",recvBuf);

				memset(message, 0, strlen(message));
				memcpy(message, recvBuf, strlen(recvBuf));

				getphpCmd = true;
			}
		
			if (getphpCmd)
			{
				std::string strMsg = message;

				// send command to sensor
				printf("\n 2. do command: %s", strMsg.c_str());
				bool bRet = this->parseMsg(strMsg);

				getphpCmd = false;
			}
			
			// update all status to php 
			{
				memset(sendBuf,0,strlen(sendBuf));
				memcpy(sendBuf,message,strlen(message));

				snd = sendto(sock,sendBuf,strlen(message),0,(struct sockaddr *) &client,structlength);
				if(snd < 0)
				{
					perror("sendto");
					exit(1);
				}

				printf("\n 3. send OK! \n");
			}
#endif
		} // end of while
	  } 
	  catch (boost::thread_interrupted&) 
	  { 
	  }
}

void HttpSvr::stop()
{
#ifndef WIN32
	close(sock);

	// turn off thread
	if (m_http_thread != NULL)
	{
		m_http_thread.get()->interrupt(); 
		m_http_thread.get()->join(); 
		m_http_thread.reset();
	}
#endif
}

void HttpSvr::set(const std::string& name)
{
}

void HttpSvr::get(const std::string& name)
{
}

bool HttpSvr::parseMsg(const std::string& message)
{
	std::size_t found = 0;
	std::string tmpMsg = message;

	// find the test message
	found = tmpMsg.find(MessageTag[0]);
	if (found!=std::string::npos)
	{
		tmpMsg.erase(found, MessageTag[0].length());

		int value = atoi(tmpMsg.c_str());
		std::cout << "\n TEST: "<< value << std::endl;
		
		m_sensor_node->TriggerGPio(value);
		return true;
	}

	// find the "sender" message
	found = tmpMsg.find(MessageTag[1]);
	if (found!=std::string::npos)
	{
		tmpMsg.erase(found, MessageTag[1].length());

		m_sensor_node->SetSensor(tmpMsg);
		return true;
	}

	// find the "receiver" message
	found = tmpMsg.find(MessageTag[2]);
	if (found!=std::string::npos)
	{
		std::cout << "first '[R]' found at: " << found;
		return true;
	}

	return false;
}
