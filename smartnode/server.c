#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int peekSensor()
{
	return 0;
}

void main()
{
	int sock;
	struct sockaddr_in server,client;
	int recvd, snd;
	int structlength;
	
	const char *server_ip = "127.0.0.1";
	const int port = 8888;

	char recvBuf[2000],sendBuf[2000]; 
	char message[]="I have received: \n";

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
		exit(1);
	}

	structlength = sizeof(server);
	if (bind(sock,(struct sockaddr *) &server,structlength) < 0) 
	{
		printf("socket bind error!\n");
		perror("bind");
		exit(1);
	}

	int phpCmd = 0;
	while(1)
	{
		structlength = sizeof(client); 
		printf("waiting.......\n");

		recvd = recvfrom(sock,recvBuf,sizeof(recvBuf),0,(struct sockaddr *) & client,&structlength);
		if(recvd < 0)
		{
			perror("recvfrom"); 
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("1. received: %s\n",recvBuf);

			memset(message, 0, strlen(message));
			memcpy(message, recvBuf, strlen(recvBuf));
			phpCmd = 1;
		}
	
		if (1 == phpCmd)
		{
			// send command to sensor
			printf("2. do command\n");

			phpCmd = 0;
		}
		
		// get status from sensors
		{
			printf("3. query sensor status.\n");

			int status = peekSensor();
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

			printf("4. send OK!\n");
		}
	} // end of while

	close(sock);
}
