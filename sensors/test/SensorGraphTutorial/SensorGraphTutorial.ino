/*
  Sends sensor data to Arduino
  (needs SensorGraph and Amarino app installed and running on Android)
*/
 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;
const int sensor = 5;
const int onboardLed = 13;
int val;

const int TrigPin = 4;
const int EchoPin = 5;
// establish variables for duration of the ping,
// and the distance result in centimeters:
long duration, cm;

void setup()  
{
  // use the baud rate your bluetooth module is configured to 
  // not all baud rates are working well, i.e. ATMEGA168 works best with 57600
  Serial.begin(9600); 
 
  // we initialize analog pin 5 as an input pin
  pinMode(sensor, INPUT);
  pinMode(onboardLed, OUTPUT);
  digitalWrite(onboardLed, HIGH);
}

void loop()
{
  meetAndroid.receive(); // you need to keep this in your loop() to receive events
  
  // read input pin and send result to Android
  //int val = analogRead(sensor);
  //if (val > 400)
  //{
  //  digitalWrite(onboardLed, LOW);
  //  delay(val);
  //  digitalWrite(onboardLed, HIGH);
  //  Serial.println(val);
    
  //  meetAndroid.send(val);
  //} 
  // add a little delay otherwise the phone is pretty busy
  //delay(100);
  

  // The HC-SR04 is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(TrigPin, OUTPUT);
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(TrigPin, LOW);

  // The same pin is used to read the signal from the HC-SR04: a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(EchoPin, INPUT);
  duration = pulseIn(EchoPin, HIGH);
  
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  if (cm > 100)
  {
    digitalWrite(onboardLed, LOW);
    delay(cm);
    digitalWrite(onboardLed, HIGH);
    
    meetAndroid.send(cm);
  } 
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  // add a little delay otherwise the phone is pretty busy
  delay(100);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
