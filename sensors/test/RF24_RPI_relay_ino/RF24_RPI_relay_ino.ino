#include <SPI.h>
#include <RF24.h>
#include "printf.h"

RF24 radio(8,9);

const uint64_t pipes[2] = { 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL };

void setup(void) {
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24 arduino receiver\n\r");
  radio.begin();
  radio.setChannel(111);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.setAutoAck( true ) ;
  radio.powerUp() ;
  radio.startListening();
  radio.printDetails();
}
 
void loop(void)
{
  // if there is data ready
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
    unsigned long got_message;
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &got_message, sizeof(unsigned long) );
    }
    printf("Got message %lu\n\r", got_message);
    delay(100);
  }
}
