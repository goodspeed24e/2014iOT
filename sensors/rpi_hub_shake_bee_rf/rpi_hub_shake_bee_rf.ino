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

enum ELEMENT_TYPE {
  ELEMENT_TYPE_PIPE = 0x00,
  ELEMENT_TYPE_EVENT1,
  ELEMENT_TYPE_RET1,
  ELEMENT_TYPE_EVENT2,
  ELEMENT_TYPE_RET2,
  ELEMENT_TYPE_TIMESTAMP
};

enum EVENT_TYPE {
  EVENT_TYPE_UNKNOWN     = 0, // 2 << 0
  EVENT_TYPE_FLAME       = 1, // 2 << 1
  EVENT_TYPE_GAS         = 2, // 2 << 2
  EVENT_TYPE_CO          = 3, // 2 << 3
  EVENT_TYPE_WATER       = 4, // 2 << 4
  EVENT_TYPE_NOISE       = 5, // 2 << 5
  EVENT_TYPE_TEMPERATURE = 6, // 2 << 6
  EVENT_TYPE_RFID        = 7, // 2 << 7
  EVENT_TYPE_SHOCK       = 8, // 2 << 8
  EVENT_TYPE_INTRUSION   = 9, // 2 << 9
  EVENT_TYPE_MAGETIC     = 10,// 2 << 10
  EVENT_TYPE_PHOTOCELL   = 11,// 2 << 11
  EVENT_TYPE_REVERSE1 = 12,   // 2 << 12
  EVENT_TYPE_REVERSE2 = 13,   // 2 << 13
  EVENT_TYPE_REVERSE3 = 14,   // 2 << 14
  EVENT_TYPE_REVERSE4 = 15,   // 2 << 15
  EVENT_TYPE_MAX      = 16    // 2 << 16
};

enum ACTION_TYPE {
  ACTION_TYPE_DONTCARE = 0x00,
  ACTION_TYPE_BEE = 0x01,
  ACTION_TYPE_MSG = 0x02,
  ACTION_TYPE_URGENT = 0x04,
};

struct Package {
  char nodeID[12];
  int event1_type;
  int return1_value;
  int event2_type;
  int return2_value;
  uint32_t time_stamp;
};

// rf channel number
const int CHANNELID = 70;
//#define RF_SETUP 0x17

// Radio pipe addresses for the 2 nodes to communicate.
   const uint64_t pipes[2]  = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2]  = { 0xF0F0F0F0E2LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2]   = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F2LL, 0xF0F0F0F0D2LL };
// Pipe0 is F0F0F0F0D2 ( same as reading pipe )

// rf timer & its duration
const int TIMEOUT = 1000;
unsigned long last_time = 0;

// sensor
const int BUZZER_PIN = 7;
const int SHOCK_PIN = 3;

/// GLOBAL VARIABLES
// Set up nRF24L01 radio on SPI pin for CE, CSN
RF24 radio(9,10);
char receivePayload[32] = {0};
char outBuffer[32] = {0};

// sensor type
static EVENT_TYPE g_shockResult = EVENT_TYPE_UNKNOWN;

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
    radio.setChannel(CHANNELID);
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
              idx++;
            }

            // debug message
            if (0)
            {
                Serial.print( "nodeID: ");        Serial.println(token[0]);
                Serial.print( "event1_type: ");   Serial.println(token[1]);
                Serial.print( "return1_value: "); Serial.println(token[2]);
                Serial.print( "event2_type: ");   Serial.println(token[3]);
                Serial.print( "return2_value: "); Serial.println(token[4]);
                Serial.print( "time_stamp: ");    Serial.println(token[5]);
            }
            
            if (atoi(token[ELEMENT_TYPE_EVENT1]) == (int)EVENT_TYPE_SHOCK)
            {
              g_shockResult = EVENT_TYPE_UNKNOWN;  
              Serial.println("RESET!!! -- SHAKE");
            }
         }
         delay(16);
    } // end of while 
    
    // reset output buffer
    memset(outBuffer, 0, sizeof(outBuffer));;

    Package pkg;
    sprintf(pkg.nodeID,"%X",pipes[0]); 
    pkg.event1_type   = (int)g_shockResult;
    pkg.return1_value = (int)g_shockResult;
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

      // Send outBuffer to hub(rpi)
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
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(SHOCK_PIN,INPUT);
}

void shock_sensor()
{
  int val = digitalRead(SHOCK_PIN);
  if(val == LOW)
  {
    Serial.println("[ALARM] - SHOCK!!!");
    digitalWrite(BUZZER_PIN,HIGH);   delay(100);
    digitalWrite(BUZZER_PIN,LOW);    delay(100);
    
    g_shockResult = EVENT_TYPE_SHOCK;
  }
}

void setup(void)
{
  Serial.begin(9600);
  
  // seteup sensor & rf24
  setup_sensor();
  setup_RF_2_RPI();
  
  last_time = millis();
}
  
void loop(void)
{
  // query sensor value
  shock_sensor();
  
  // send results to rpi
  if (millis() - last_time > TIMEOUT)
  {
    rf2rpi();
    last_time = millis();
  }
}
