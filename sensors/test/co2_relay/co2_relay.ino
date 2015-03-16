//#include <SPI.h>
//#include "nRF24L01.h"
//#include "RF24.h"
//#include "printf.h"

// This is for git version tracking.  Safe to ignore
#ifdef VERSION_H
#include "version.h"
#else
#define __TAG__ "1.0.01-0001"
#endif

// CONST VARIABLES
enum SENSOR_TYPE {
  SENSOR_TYPE_UNKNOWN = 0x00,
  SENSOR_TYPE_SHOCK,
  SENSOR_TYPE_FLAME,
  SENSOR_TYPE_GAS,
  SENSOR_TYPE_INTRUSION,
  SENSOR_TYPE_WATER,
  SENSOR_TYPE_MAX = 0x80
};

const int ledPin = 13;
const int buzzerPin = 7;
const int waterPin = A5;
   
//const int channelID = 110;
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
//const uint64_t pipes[6] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 0xF0F0F0F0F1, 0xF0F0F0F0F2 };

// const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F2LL, 0xF0F0F0F0D2LL };

/// GLOBAL VARIABLES
//SENSOR_TYPE gResult = SENSOR_TYPE_UNKNOWN;

//RF24 radio(9,10);

// FUNCTIONS 
void setup_sensor()
{
  pinMode(buzzerPin,OUTPUT);
  pinMode(waterPin,INPUT);
}

/*
void setup_RF_2_RPI(void)
{
    printf_begin();
    printf_P(PSTR("VERSION: " __TAG__ "\n\r"));

    radio.begin();
    radio.enableDynamicPayloads();
    //radio.setAutoAck(1);
    //radio.setRetries(15,15);
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setChannel(channelID);
    radio.setCRCLength(RF24_CRC_16);
    //radio.openWritingPipe(pipes[1]);
    //radio.openReadingPipe(1,pipes[0]);
    radio.openWritingPipe(pipes[0]); 
    radio.openReadingPipe(1,pipes[1]); 
    
    radio.startListening();
    radio.printDetails();
}

void rf2rpi(void)
{
    if (radio.available()) 
    {
        unsigned long cur_time_stamp = 0;

        bool done = false;
        while(!done) 
        {
            done = radio.read( &cur_time_stamp, sizeof(unsigned long) );
            printf("Got payload %lu...\n\r",cur_time_stamp);

            delay(20);
        }
        
        {
          radio.stopListening();
     
          if (gResult == SENSOR_TYPE_SHOCK)
          {
            unsigned long reponse = (unsigned long)gResult;
            Serial.println(reponse, DEC);
            
            radio.write( &reponse, sizeof(unsigned long) );
            gResult = SENSOR_TYPE_UNKNOWN;
          }
          else
          {  
            radio.write( &cur_time_stamp, sizeof(unsigned long) );
          }
          
          radio.startListening();
        }      
    }
}
*/
void co()
{
  int val = analogRead(waterPin);
  if(val > 180)
  {
    Serial.println(1111, DEC);
    digitalWrite(buzzerPin,HIGH);   delay(1000);
    digitalWrite(buzzerPin,LOW);    delay(1000);
    
    //gResult = SENSOR_TYPE_WATER;
  }
  
  digitalWrite(ledPin, HIGH);  
  delay(val);          
  digitalWrite(ledPin, LOW);   
  delay(val);  
  Serial.println(val, DEC);
}

void setup()
{
  Serial.begin(9600);
  setup_sensor();
  //setup_RF_2_RPI();
}

void loop()
{
  co();
  //rf2rpi();
}
