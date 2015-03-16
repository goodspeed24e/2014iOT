#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// This is for git version tracking.  Safe to ignore
#ifdef VERSION_H
#include "version.h"
#else
#define __TAG__ "1.0.01-0001"
#endif

// CONST VARIABLES
struct Package {
  char nodeID[12];
  int event1_type;
  int return1_value;
  int event2_type;
  int return2_value;
  uint32_t time_stamp;
};

enum ELEMENT_TYPE {
  ELEMENT_TYPE_PIPE = 0x00,
  ELEMENT_TYPE_EVENT1,
  ELEMENT_TYPE_RET1,
  ELEMENT_TYPE_EVENT2,
  ELEMENT_TYPE_RET2,
  ELEMENT_TYPE_TIMESTAMP
};

enum EVENT_TYPE {
  EVENT_TYPE_UNKNOWN = 0x00,
  EVENT_TYPE_SHOCK,
  EVENT_TYPE_FLAME,
  EVENT_TYPE_GAS,
  EVENT_TYPE_INTRUSION,
  EVENT_TYPE_MAX = 0x80
};

enum ACTION_TYPE {
  ACTION_TYPE_DONTCARE = 0x00,
  ACTION_TYPE_BEE = 0x01,
  ACTION_TYPE_MSG = 0x02,
  ACTION_TYPE_URGENT = 0x04,
};

// Radio pipe addresses for the 2 nodes to communicate.
// const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F1LL, 0xF0F0F0F0D2LL };
   const uint64_t pipes[2] = { 0xF0F0F0F0F2LL, 0xF0F0F0F0D2LL };
// Pipe0 is F0F0F0F0D2 ( same as reading pipe )

// sensor
const int buzzerPin = 7;
const int shockPin = 3;

// rf
const int channelID = 76;
//#define RF_SETUP 0x17

/// GLOBAL VARIABLES
// Set up nRF24L01 radio on SPI pin for CE, CSN
RF24 radio(9,10);
char receivePayload[32];
char outBuffer[32];

// rf timer & duration
const  int timeout_timer = 1000;
unsigned long duration = 0;

// sensor type
static EVENT_TYPE gResult = EVENT_TYPE_UNKNOWN;

void setup_RF_2_RPI(void)
{
    printf_begin();
    printf_P(PSTR("VERSION: " __TAG__ "\n\r"));
    
    radio.begin();
    radio.enableDynamicPayloads();
    radio.setAutoAck(1);
    radio.setRetries(15,15);
    
    // Setup default radio settings  
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(channelID);
    radio.setCRCLength(RF24_CRC_16);
    radio.openWritingPipe(pipes[0]); 
    radio.openReadingPipe(1,pipes[1]);
    // Send only, ignore listening mode
    //radio.startListening();
    
    radio.printDetails(); 
}

void rf2rpi(void)
{
    unsigned long waiting_time = millis();
    
    bool updateStatus = false;
    bool done = false;
    while (radio.available() && !done ) 
    {
         uint8_t len = radio.getDynamicPayloadSize();
         done = radio.read( receivePayload, len); 
         printf(" inBuffer: %s\n\r", receivePayload);
       
         if (done)
         {
            int idx = 0;
            char *token[6];
            char *p = receivePayload;
            char *str;
            while ((str = strtok_r(p, ",", &p)) != NULL)
            {
              token[idx] = str;
              //Serial.println(token[idx]);
              idx++;
            }

            if (atoi(token[ELEMENT_TYPE_EVENT1]) == (int)EVENT_TYPE_SHOCK)
            {
              gResult = EVENT_TYPE_UNKNOWN;  
              Serial.println("RESET!!! -- SHAKE");
            }
         }
         delay(16);
    } // end of while 
    
    // reset output buffer
    memset(outBuffer, 0, sizeof(outBuffer));;

    Package pkg;
    sprintf(pkg.nodeID,"%X",pipes[0]); 
    pkg.event1_type   = (int)gResult;
    pkg.return1_value = (int)gResult;
    pkg.event2_type  = (int)EVENT_TYPE_UNKNOWN;
    pkg.return2_value = 0;
    pkg.time_stamp = millis();
    
    char temp[32];
    sprintf(pkg.nodeID,"%X",pipes[0]); 
    strcat(outBuffer,pkg.nodeID); strcat(outBuffer,",");
    sprintf(temp,"%02d",pkg.event1_type);  
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%03d",pkg.return1_value);
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%02d",pkg.event2_type);  
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%03d",pkg.return2_value);
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%lu",pkg.time_stamp);
    strcat(outBuffer,temp); 
    // End string with 0
    // strcat(outBuffer,0);
    printf("outBuffer: %s len: %d",outBuffer, strlen(outBuffer));
     
    {
      // Stop listening and write to radio 
      radio.stopListening();

      // Send to hub
      if ( radio.write( outBuffer, strlen(outBuffer)) ) 
      {
         printf("Send successful\n\r"); 
      }
      else 
      {
         printf("Send failed\n\r");
      }
    
      radio.startListening();
    } 
}

// FUNCTIONS 
void setup_sensor()
{
  pinMode(buzzerPin,OUTPUT);
  pinMode(shockPin,INPUT);
}

void shock()
{
  int val = digitalRead(shockPin);
  if(val == LOW)
  {
    Serial.println("SHOCK!!!");
    digitalWrite(buzzerPin,HIGH);   delay(100);
    digitalWrite(buzzerPin,LOW);    delay(100);
    
    gResult = EVENT_TYPE_SHOCK;
  }
}

void setup(void)
{
  Serial.begin(9600);
  setup_sensor();
  setup_RF_2_RPI();
  
  duration = millis();
}
  
void loop(void)
{
  //shock();
  
  if (millis() - duration > timeout_timer)
  {
    rf2rpi();
    duration = millis();
  }
}
