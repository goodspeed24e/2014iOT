
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

static int intrusion_pin = 4;

const unsigned long GasOffset = 10000;
const unsigned long FlameOffset = 20000;
const unsigned long IntrusionOffset = 30000;
unsigned long g_IntrusionResult = 0;

RF24 radio(9,10);
void setup_RF2401_2_RPI(void)
{
    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    const int channelID = 110;
    
    printf_begin();
    printf("\n\rRF24/examples/pingpair/\n\r");
    printf("ROLE: Pong back\n\r");
 
    radio.begin();

    radio.enableDynamicPayloads();
    //radio.setRetries(15,15);
    radio.setDataRate(RF24_1MBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setChannel(channelID);
    radio.setCRCLength(RF24_CRC_16);
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
    radio.printDetails();
}

void RF2RPI(void)
{
    if (radio.available()) 
    {
        unsigned long got_time;

        bool done = false;
        while(!done) 
        {
            done = radio.read( &got_time, sizeof(unsigned long) );
            printf("Got payload %lu...\n\r",got_time);

            delay(20);
        }
        
        radio.stopListening();
     
        if (g_IntrusionResult == IntrusionOffset)
        {
          g_IntrusionResult += 1;
          printf("Sent Instrusion -- "); Serial.println(g_IntrusionResult,DEC); //Print the value to serial port
          radio.write( &g_IntrusionResult, sizeof(unsigned long) );
          g_IntrusionResult = 0;
        }
        else
        {  
          radio.write( &got_time, sizeof(unsigned long) );
          //printf("Sent response.\n\r");
        }
        radio.startListening();
    }
}

void SleepTime(void)
{
   delay(100); // idea 0.5 sec
}

void setup_intrusion()
{
  pinMode(intrusion_pin, INPUT);  
}

void Intrusion()
{
  int val = digitalRead(intrusion_pin); 
  if (val == 1)
  {
    Serial.print("Flame Detected!!! "); 
    Serial.println(val,DEC); //Print the value to serial port
    g_IntrusionResult = IntrusionOffset;
  }  
}

void setup(void)
{
  Serial.begin(9600); //Set serial baud rate to 9600 bps
  //setup_intrusion();
  setup_RF2401_2_RPI();
}

void loop(void)
{
  //Intrusion();
  RF2RPI();
  SleepTime();
}

