//1) Replace every instance of "Client" with "EthernetClient"
//2) Replace every instance of "Server" with "EthernetServer"
//3) add this block near the beginning:
//CODE: SELECT ALL
   // make backwardly compatable
//   #if ARDUINO < 100
//       #define EthernetClient Client
//       #define EthernetServer Server
//   #endif
//Step three is optional if you never need to have it compile in the older IDE.

/************************************************/
#include <SPI.h>
/*
 * Web Server
 *
 * A simple web server that shows the value of the analog input pins.
 */
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] =  { 172, 16, 3, 225 };
// make backwardly compatable
#if ARDUINO < 100
  #define EthernetClient Client
  #define EthernetServer Server
#endif

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

EthernetServer server(80);
void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  EthernetClient client = server.available();
  if (client) 
  {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == 'n' && current_line_is_blank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
         
          // output the value of each analog input pin
          client.print("welcome to tinyos electronics");
          client.println("<br />");
          client.print("//*************************************");
          client.println("<br />");
          client.print("");
          client.println("<br />");
          client.print("//*************************************");
          client.println("<br />");
          for (int i = 0; i < 6; i++) {
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRead(i));
            client.println("<br />");
          }
          break;
        }
        if (c == 'n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != 'r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    client.stop();
  }
}
/************************************************/

