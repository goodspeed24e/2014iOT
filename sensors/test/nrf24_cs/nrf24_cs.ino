/**
 *
 * Pins:
 * Hardware SPI: UNO / LEONARDO
 * MISO -> D12 / ICSP 1
 * MOSI -> D11 / ICSP 4
 * SCK -> D13 / ICSP 3
 *
 */
 
#include <SPI.h> // required by Mirf library
#include <Mirf.h> // required by Mirf library
#include <nRF24L01.h> // required by Mirf library
#include <MirfHardwareSpiDriver.h> // required by Mirf library
 
 
byte* SERVER_ADDR = (byte*)"srv01"; // 5 bytes addr
byte* CLIENT_ADDR = (byte*)"cli01"; // 5 bytes addr
boolean clientMode = false; // start mode using serverMode
unsigned long TIMEOUT = 5000; // server response timeout for 2s
unsigned long sendTime; // time when client send data
unsigned long randNumber = 5000;
unsigned long counter = 0;
void setup(){
  Serial.begin(9600);
 
  Mirf.cePin = 8; // default using pin 8
  Mirf.csnPin = 7; // default using pin 7
  Mirf.channel = 3; // default value
  Mirf.payload = 5; // define data length
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
 
  /*
   * Mirf will always listen to RADDR.
   * And set TADDR to the RADDR of the other end.
   */
  Mirf.setRADDR(SERVER_ADDR); // 5 bytes addr
  Mirf.setTADDR(CLIENT_ADDR); // 5 bytes addr
  Mirf.config();
 
  Serial.print("\r\nConfigure as server.");
  Serial.println("Change to client by input 'C'.");
}
 
void loop(){
  byte buffer[5] = {0xFF,0x00,0x00,0x00,0xFF}; // 5 bytes data
 
  if(Serial.available()){ // read input first
    char c = toupper(Serial.read());
    if(c == 'C' && !clientMode){ // set to client mode
      Serial.print("Configure as client mode.");
      Serial.println("Change to server by input 'S'.");
      clientMode = true;
 
      Mirf.setRADDR(CLIENT_ADDR); // 5 bytes addr
      Mirf.setTADDR(SERVER_ADDR); // 5 bytes addr
    }
    else if(c == 'S' && clientMode){ // set to server mode
      Serial.print("Configure as server mode.");
      Serial.println("Change to client by input 'C'.");
      clientMode = false;
 
      Mirf.setRADDR(SERVER_ADDR); // 5 bytes addr
      Mirf.setTADDR(CLIENT_ADDR); // 5 bytes addr
    }
  }
 
  if(clientMode){
    buffer[1] = 0xEF; buffer[2] = 0xFF; // client heart beat: FFEFFFxxFF
    sendTime = millis();
    Mirf.send(buffer);
    while(Mirf.isSending()); // wait for sending
    Serial.println("Send client heart beat!");
    while(!Mirf.dataReady()){
      if((millis() - sendTime) > TIMEOUT){
        Serial.println("Server response timeout!");
        return;
      }
    }
    Mirf.getData(buffer);
    if(buffer[0] != 0xFF || buffer[4] != 0xFF) // drop error reading
      return;
    if(buffer[1] == 0xEF && buffer[2] == 0x00){ // server heart beat: FFEF00xxFF
      Serial.println("Got server heart beat!");
    }
    Serial.print("Channel: ");    Serial.print(Mirf.channel);
    Serial.print(" => Ping: ");   Serial.println((millis() - sendTime));
    randNumber = random(100);
    delay(TIMEOUT-randNumber); // send heart beat after 1s
  }
 
  else if(!Mirf.isSending() && Mirf.dataReady()){ // Server Mode and recieved
    Mirf.getData(buffer);
    if(buffer[0] != 0xFF || buffer[4] != 0xFF) // drop error reading
      return;
    // client heart beat: FFEFFFxxFF, server heart beat: FFEF00xxFF
    if(buffer[1] == 0xEF && buffer[2] == 0xFF){
      Serial.println("Got client heart beat!");
      buffer[2] = 0x00; // format: FFEF00xxFF
      Mirf.send(buffer);
      while(Mirf.isSending()); // wait for sending
      Serial.print("Send server heart beat! --- ");      
      Serial.println(counter);    

      if (counter>TIMEOUT)
        counter = 0;
      else
        ++counter;
    }
  }
}

