#include <RF24.h>
#include <SPI.h>
#include <ClusterNetwork.h>

RF24 radio(9,10);
ClusterNetwork cluster(radio);
const uint64_t pipe = 0xF0F0F0F0E1LL;
static int State = 1;
const uint8_t node_ID = 15;
uint8_t source_ID;        
uint8_t type;
uint8_t payload;

void setup(void)
{
  Serial.begin(9600);
  Serial.print("Node ID: ");
  Serial.print(node_ID);
  radio.begin();
  cluster.begin( /*channel*/ 90, /*pipe address*/ pipe, node_ID);
  cluster.updateCH( node_ID );
}

void loop(void)
{
  switch( State )
  {
    case 1:
        if( radio.available() ) State = 2;
    break;
    case 2:
        if( cluster.receiveMessage(source_ID,type,payload) ) State = 3;
        else State = 1;
    break;
    case 3:
        Serial.print("\nSource ID: ");
		Serial.print( source_ID ); // 3
        Serial.print("... Type: ");
        if (type == 1) Serial.print( "ADV" );  // 1
        Serial.print("... Payload: ");
		Serial.print( payload );   // 240
        State = 1;
    break;
  }
  Serial.print("\nState: ");Serial.print(State);
  delay(200);
}
