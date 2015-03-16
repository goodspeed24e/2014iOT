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
enum SENSOR_TYPE {
  SENSOR_TYPE_UNKNOWN = 0x00,
  SENSOR_TYPE_SHOCK,
  SENSOR_TYPE_FLAME,
  SENSOR_TYPE_GAS,
  SENSOR_TYPE_INTRUSION,
  SENSOR_TYPE_MAX = 0x80
};

const int buzzerPin = 7;
const int shockPin = 3;

const int channelID = 76;
const int timeout_timer = 500;
#define RF_SETUP 0x17

// Radio pipe addresses for the 2 nodes to communicate.
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F2LL, 0xF0F0F0F0D2LL };
// Pipe0 is F0F0F0F0D2 ( same as reading pipe )

/// GLOBAL VARIABLES
// Set up nRF24L01 radio on SPI pin for CE, CSN
RF24 radio(9,10);
char receivePayload[32];
int counter = 0;

SENSOR_TYPE gResult = SENSOR_TYPE_UNKNOWN;

void setup_RF_2_RPI(void)
{
    printf_begin();
    printf_P(PSTR("VERSION: " __TAG__ "\n\r"));
    
    radio.begin();
    radio.enableDynamicPayloads();
    //radio.setAutoAck(1);
    //radio.setRetries(15,15);
    
    // Setup default radio settings  
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(channelID);
    radio.setCRCLength(RF24_CRC_16);
    radio.openWritingPipe(pipes[0]); 
    radio.openReadingPipe(1,pipes[1]);
    // Send only, ignore listening mode
    radio.startListening();
    
    radio.printDetails(); 
}

void rf2rpi(void)
{
    unsigned long waiting_time = millis();
    
    bool timeout = false;
    while (radio.available() && !timeout ) 
    {
         uint8_t len = radio.getDynamicPayloadSize();
         bool done = radio.read( receivePayload, len); 
         printf("Got payload %s...\n\r", receivePayload);
        
         // Compare receive payload with outBuffer        
         //if ( ! strcmp(outBuffer, receivePayload) ) 
         //{
         //    unsigned long rtt = millis() - send_time;
         //    printf("time diff %lu...\n\r", rtt);   
         //}       
    
         // Check for timeout and exit the while loop
         if ( millis() - waiting_time > timeout_timer )
         {
            Serial.println("Timeout!!!");
            printf("Timeout!!!\n\r");
            timeout = true;
         }
         else
         {
            gResult = SENSOR_TYPE_UNKNOWN;
         }        

         delay(20);
    } // end of while 
    
    // Get readings from sensors
    int Data1, Data2 ,Data3;
    Data1 = counter;
    Data2 = (int)gResult;
    Data3 = 0;
    if ( counter++ > 999 ) counter = 0;

    // Use the last 2 pipes address as nodeID
    char nodeID[12];
    sprintf(nodeID,"%X",pipes[0]); 
    
    char temp[16];
    char outBuffer[31]=""; // Clear the outBuffer before every loop   
    strcat(outBuffer,nodeID); strcat(outBuffer,",");
    sprintf(temp,"%03d",Data1);  
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%04d",Data2);
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%04d",Data3);
    strcat(outBuffer,temp);  strcat(outBuffer,",");
    sprintf(temp,"%lu",millis());
    strcat(outBuffer,temp); 
    // End string with 0
    // strcat(outBuffer,0);
    printf("outBuffer: %s len: %d --- ",outBuffer, strlen(outBuffer));
  
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
    
    gResult = SENSOR_TYPE_SHOCK;
  }
}

unsigned long timer = 0;
unsigned long duration = 500;

void setup(void)
{
  Serial.begin(9600);
  setup_sensor();
  setup_RF_2_RPI();
  
  timer = millis();
}
  
void loop(void)
{
  shock();
  
  if (millis() - timer > duration)
  {
    rf2rpi();
    timer = millis();
  }
}
