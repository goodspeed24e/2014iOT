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

const int gasPin = 0;
const int flamePin = 5;
   
const int channelID = 110;
//const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
//const uint64_t pipes[6] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 0xF0F0F0F0F1, 0xF0F0F0F0F2 };

// const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
 const uint64_t pipes[2] = { 0xF0F0F0F0E2LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0E3LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F1LL, 0xF0F0F0F0D2LL };
// const uint64_t pipes[2] = { 0xF0F0F0F0F2LL, 0xF0F0F0F0D2LL };

/// GLOBAL VARIABLES
static unsigned long GAS_THRESHOLD = 600;
static unsigned long g_GasResult = 0;

static unsigned long FLAME_THRESHOLD = 200;
static unsigned long g_FlameResult = 0;

RF24 radio(9,10);

// FUNCTIONS 
void setup_RF2401_2_RPI(void)
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
            printf("Got payload %lu...\n\r", cur_time_stamp);
            
            delay(20);
        }
        
        {
          radio.stopListening();
       
          // update threshold
          if (cur_time_stamp == SENSOR_TYPE_GAS) {
                //GAS_THRESHOLD = cur_time_stamp; ;
                Serial.print("GAS UPDATED!!! ");
          }
          else if (cur_time_stamp == SENSOR_TYPE_FLAME) {
                //FLAME_THRESHOLD = cur_time_stamp;
                Serial.print("FLAME UPDATED!!! ");
          }
              
          if (g_FlameResult > FLAME_THRESHOLD)
          {
            unsigned long reponse = (unsigned long)SENSOR_TYPE_FLAME;
            radio.write( &reponse, sizeof(unsigned long) );
            g_FlameResult = 0;
          }
          else if (g_GasResult > GAS_THRESHOLD)
          {
            unsigned long reponse = (unsigned long)SENSOR_TYPE_GAS;
            radio.write( &reponse, sizeof(unsigned long) );
            g_GasResult = 0;
          }
          else
          {  
            radio.write( &cur_time_stamp, sizeof(unsigned long) );
          }
          
          radio.startListening();
        }
    }
}

void gas(void)
{
  unsigned long val  = analogRead(gasPin);
  if (val > GAS_THRESHOLD)
  {
    Serial.print("GAS Detected!!! ");
    Serial.println(val, DEC);
    g_GasResult = val;
  }
}

void flame(void)
{
  unsigned long val = analogRead(flamePin);
  if (val > FLAME_THRESHOLD)
  {
    Serial.print("Flame Detected!!! "); 
    Serial.println(val, DEC);
    g_FlameResult = val;
  } 
}

void setup(void)
{
  Serial.begin(9600); //Set serial baud rate to 9600 bps
  setup_RF2401_2_RPI();
}

void loop(void)
{
  flame();
  gas();
  rf2rpi();
}
