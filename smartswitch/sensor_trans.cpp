#include <math.h>
#include "sensor_trans.h"
#include "sensor_prototype.h"

// Radio pipe addresses for the 2 nodes to communicate.
// First pipe is for writing, 2nd, 3rd, 4th, 5th & 6th is for reading...
const uint64_t pipes[6] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 0xF0F0F0F0F1LL, 0xF0F0F0F0F2LL };

RF24Wrapper::RF24Wrapper(SensorFactory *sf, const int& ch)
	: m_sensorFactory(sf), m_ChannelID(ch)
#ifndef WIN32
	, radio("/dev/spidev0.0",8000000,18)  // Setup for GPIO 25 CSN
#endif
{
}

RF24Wrapper::~RF24Wrapper()
{
}

void RF24Wrapper::Setup(void)
{
#ifndef WIN32
	// Refer to RF24.h or nRF24L01 DS for settings
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setAutoAck(1);
	radio.setRetries(15,15);
	radio.setDataRate(RF24_1MBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setChannel(m_ChannelID);
	radio.setCRCLength(RF24_CRC_16);

	// Open 6 pipes for readings ( 5 plus pipe0, also can be used for reading )
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);
	radio.openReadingPipe(2,pipes[2]);
	radio.openReadingPipe(3,pipes[3]);
	radio.openReadingPipe(4,pipes[4]);
	radio.openReadingPipe(5,pipes[5]);

	//
	// Start listening
	//
	radio.startListening();

	//
	// Dump the configuration of the rf unit for debugging
	//
	radio.printDetails();
	printf("\n\rOutput below : \n\r");
	usleep(1000);
#endif
}

void RF24Wrapper::Loop(void)
{
#ifndef WIN32
	char sendPayload[32] = {0};
	char receivePayload[32] = {0};
	uint8_t pipe = 1;
	
	// Start listening
	radio.startListening();

	while ( radio.available(&pipe) ) 
	{
		// get plyload length
		uint8_t len = radio.getDynamicPayloadSize();
		
		// read data
		radio.read( receivePayload, len );

		// display this payload
		printf("\n Recv: size=%i payload=%s pipe=%i", len, receivePayload, pipe);

		// update status, sensitivity, action operation
		if (pipe != 7) 
		{
			int idx = 0;
			char *token[ELEMENT_TYPE_MAX];
        		char *p = receivePayload;
        		char *str;
        		while ((str = strtok_r(p, ",", &p)) != NULL)
        		{
                		token[idx] = str;
                		idx++;
        		}

			Sensor sensor1;
			printf("\r\n1");
			sensor1.channel_ = m_ChannelID;
			printf("\r\n11");
			sensor1.pipe_id_ = atoi(token[ELEMENT_TYPE_PIPE]);
			printf("\r\n111");
			sensor1.sensor_type_ = (SENSOR_TYPE)atoi(token[ELEMENT_TYPE_EVENT1]);
			printf("\r\n1111");
			sensor1.sensor_value_ = atoi(token[ELEMENT_TYPE_RET1]);
			printf("\r\n111111");
			sensor1.time_stamp_ = atoi(token[ELEMENT_TYPE_TIMESTAMP]);
			printf("\r\n1111111");
			sensor1.status_ = (SENSOR_STATUS)atoi(token[ELEMENT_TYPE_STATUS]);

			Sensor sensor2;
			sensor2.channel_ = m_ChannelID;
			printf("\r\n2");
			sensor2.pipe_id_ = atoi(token[ELEMENT_TYPE_PIPE]);
			printf("\r\n22");
			sensor2.sensor_type_ = (SENSOR_TYPE)atoi(token[ELEMENT_TYPE_EVENT2]);
			printf("\r\n222");
			sensor2.sensor_value_ = atoi(token[ELEMENT_TYPE_RET2]);
 			printf("\r\n2222");
			sensor2.time_stamp_ = atoi(token[ELEMENT_TYPE_TIMESTAMP]);
 			printf("\r\n22222");
			sensor2.status_ = (SENSOR_STATUS)atoi(token[ELEMENT_TYPE_STATUS]);

			//ACTION_TYPE av = (ACTION_TYPE)atoi(token[ELEMENT_TYPE_ACTION]);
			//printf("\r\n222222");
/*
			// reset sensor threshold
			if (m_sensorFactory->UpdateSensor(sensor1, av) || m_sensorFactory->UpdateSensor(sensor2,av) )
			{
				// update sensitivity and action operation
				char buffer1[8];
				snprintf(buffer1, sizeof(buffer1), "%d", sensor1.sensor_value_);
				printf ("\n sensitivity1: %s",buffer1);
				token[ELEMENT_TYPE_RET1] = buffer1;

				char buffer2[8];
				snprintf(buffer2, sizeof(buffer2), "%d", sensor2.sensor_value_);
				printf ("\n sensitivity2: %s",buffer2);
				token[ELEMENT_TYPE_RET2] = buffer2;

				char buffer3[8];
				snprintf(buffer3, sizeof(buffer3), "%d", (int)av);
				printf ("\n action operation: %s",buffer3);
				token[ELEMENT_TYPE_ACTION] = buffer3;

				// debug message
        			if (1)
        			{
				int op = (int) (log(atoi(token[ELEMENT_TYPE_ACTION])) / log(2.0));
				printf("\n nodeID: %s, time_stamp: %s",		token[ELEMENT_TYPE_PIPE], token[ELEMENT_TYPE_TIMESTAMP]);
				printf("\n event1: %s, return1_value= %s",	token[ELEMENT_TYPE_EVENT1], token[ELEMENT_TYPE_RET1]);
				printf("\n event2: %s, return2_value= %s",	token[ELEMENT_TYPE_EVENT2], token[ELEMENT_TYPE_RET2]);
				printf("\n status: %s",		STATUS_MACHINE[atoi(token[ELEMENT_TYPE_STATUS])].c_str());
				printf("\n action: %s",		SENSOR_ACTION[op].c_str());			
				printf("\n Sensor1 Type: %s value= %s", SENSOR_NAME[atoi(token[ELEMENT_TYPE_EVENT1])].c_str(), token[ELEMENT_TYPE_RET1]);
				printf("\n Sensor2 Type: %s value= %s", SENSOR_NAME[atoi(token[ELEMENT_TYPE_EVENT2])].c_str(), token[ELEMENT_TYPE_RET2]);
        			}
			}
*/

 			printf("\r\n3");

			// combine token[] to sendPayload
			len = sprintf (sendPayload,"%s,%s,%s,%s,%s,%s,%s,%s", \
				token[ELEMENT_TYPE_PIPE],\
				token[ELEMENT_TYPE_EVENT1],\
				token[ELEMENT_TYPE_RET1],\
				token[ELEMENT_TYPE_EVENT2],\
				token[ELEMENT_TYPE_RET2],\
				token[ELEMENT_TYPE_STATUS],\
				token[ELEMENT_TYPE_ACTION],\
				token[ELEMENT_TYPE_TIMESTAMP]\
			 );

			printf("\r\n33");
		}
		
		// Send back payload to sender
		radio.stopListening();

		// if pipe is 7, do not send it back
		if ( pipe != 7 )
		{
			radio.write(sendPayload,len);
			sendPayload[len]=0;
			printf("\n Send: size=%i payload=%s pipe:%i\n\r",len, sendPayload, pipe);
		}
		else 
		{
			printf("\n\r");
        	}

		pipe++;

		// reset pipe to 0
		if ( pipe > 6 ) pipe = 0;

		usleep(20);
	}
	usleep(20);

#endif
}
