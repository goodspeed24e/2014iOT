#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "nodeconfig.h"
#include "printf.h"

// This is for git version tracking.  Safe to ignore
#ifdef VERSION_H
#include "version.h"
#else
#define __TAG__ "1.0.01-0001"
#endif

/**
 * global variables - network
 */
RF24 radio(8,7);  // the pin that the rf24
RF24Network network(radio);

const short my_channel = 100;
uint16_t this_node;

const unsigned long interval = 5000; // ms
unsigned long last_time_sent;

const short max_active_nodes = 64;
uint16_t active_nodes[max_active_nodes];
short num_active_nodes = 0;
short next_ping_node_index = 0;

// Prototypes for functions to send & receive(handle) messages
bool send_T(uint16_t to);
bool send_N(uint16_t to);
void receive_T(RF24NetworkHeader& header);
void receive_N(RF24NetworkHeader& header);

void add_node(uint16_t node);
void remove_node(uint16_t node);

void begin_network();
int execute_network();
void end_network();

/**
 * global variable - sensor
 */
const int ledPin = 2; // the pin that the LED 

void begin_sensor();
int execute_sensor(int incomingByte);
void end_sensor();

void setup(void)
{
  Serial.begin(57600);
  
  printf_begin();
  printf_P(PSTR("VERSION: " __TAG__ "\n\r"));
  
  begin_network();
  begin_sensor();
}

void loop(void)
{
  int result = 0;
  result = execute_network();
  result = execute_sensor(result);
}

///////////////////////////////////////////////////////////////////////////
/// Sensor ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/**
 * begin sensor
 */  
void begin_sensor()
{
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

/**
 * execute sensor
 */
int execute_sensor(int incomingByte)
{
  if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
  }
  if (incomingByte == 'L') {
     digitalWrite(ledPin, LOW);
  }
  return 1;
}

/**
 * end sensor
 */
void end_sensor()
{
}

///////////////////////////////////////////////////////////////////////////
/// Network ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/**
 * begin network
 */
void begin_network()
{
  this_node = nodeconfig_read();

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ my_channel, /*node address*/ this_node );
}

/**
 * execute network
 */
int execute_network()
{
  // Pump the network regularly
  network.update();

  while ( network.available() )
  {
    RF24NetworkHeader header;
    network.peek(header);

    switch (header.type)
    {
    case 'T':
      receive_T(header);
      break;
    case 'N':
      receive_N(header);
      break;
    default:
      printf_P(PSTR("*** WARNING *** Unknown message type %c\n\r"),header.type);
      network.read(header,0,0);
      break;
    };
  }

  unsigned long now = millis();
  if ( now - last_time_sent >= interval )
  {
    last_time_sent = now;

    uint16_t to = 00;
    if ( num_active_nodes )
    {
      to = active_nodes[next_ping_node_index++];
      if ( next_ping_node_index > num_active_nodes )
      {
		// reset node to header
		next_ping_node_index = 0;
		to = 00;
      }
    }

    bool ok;
    if ( this_node > 00 || to == 00 )
      ok = send_T(to);
    else
      ok = send_N(to);

    if (ok)
    {
      printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
    }
    else
    {
      printf_P(PSTR("%lu: APP Send failed\n\r"),millis());
      // workaround 
      last_time_sent -= 100;
    }
  }
  
  nodeconfig_listen();
  
  return 'H';
}

/**
 * EndNetwork
 */
void EndNetwork()
{
}

/**
 * Send a 'T' message, the current time
 */
bool send_T(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'T' /*Time*/);
  
  // The 'T' message that we send is just a ulong, containing the time
  unsigned long message = millis();
  printf_P(PSTR("\n\r %lu: APP Sending %lu to 0%o...\n\r"),millis(),message,to);
  return network.write(header,&message,sizeof(unsigned long));
}

/**
 * Send an 'N' message, the active node list
 */
bool send_N(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'N' /*Time*/);
  printf_P(PSTR("\n\r %lu: APP Sending active nodes to 0%o...\n\r"),millis(),to);
  return network.write(header,active_nodes,sizeof(active_nodes));
}

/**
 * Receive a 'T' message
 *
 * Add the node to the list of active nodes
 */
void receive_T(RF24NetworkHeader& header)
{
  // The 'T' message is just a ulong, containing the time
  unsigned long message;
  network.read(header,&message,sizeof(unsigned long));
  printf_P(PSTR("%lu: APP Received %lu from 0%o\n\r"),millis(),message,header.from_node);

  // If this message is from ourselves or the base, don't bother adding it to the active nodes.
  if ( header.from_node != this_node || header.from_node > 00 )
    add_node(header.from_node);
}

/**
 * Receive an 'N' message, the active node list
 */
void receive_N(RF24NetworkHeader& header)
{
  static uint16_t incoming_nodes[max_active_nodes];
  network.read(header,&incoming_nodes,sizeof(incoming_nodes));
  printf_P(PSTR("%lu: APP Received nodes from 0%o\n\r"),millis(),header.from_node);

  int i = 0;
  while ( i < max_active_nodes && incoming_nodes[i] > 00 )
    add_node(incoming_nodes[i++]);
}

/**
 * Add a particular node to the current list of active nodes
 */
void add_node(uint16_t node)
{
  // Do we already know about this node?
  short i = num_active_nodes;
  while (i--)
  {
    if ( active_nodes[i] == node )
      break;
  }
  // If not, add it to the table
  if ( i == -1 && num_active_nodes < max_active_nodes )
  {
    active_nodes[num_active_nodes++] = node; 
    printf_P(PSTR("%lu: APP Added 0%o to list of active nodes.\n\r"),millis(),node);
  }
}
