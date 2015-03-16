#include <RF24.h>
#include <SPI.h>
#include <ClusterNetwork.h>

RF24 radio(9,10);
ClusterNetwork cluster(radio);
const uint64_t pipe = 0xF0F0F0F0E1LL;
const uint8_t node_ID = 3;
const uint8_t CH_ID = 15;
const uint8_t payload = 240;

void setup(void)
{
  Serial.begin(9600);
  Serial.print("Node ID: ");
  Serial.print(node_ID);
  radio.begin();
  cluster.begin( /*channel*/ 90, /*pipe address*/ pipe, node_ID);
  cluster.updateCH( CH_ID );
}

void loop(void)
{
  boolean ok = cluster.sendMessage( CH_ID, ADV, payload ); 
  delay(1000);  
}
