//超声波测距
//Echo 接 arduino.5;Trig 接 arduino.4
//VCC +,END - GND 无需外接电源即可测试

const int TrigPin = 4;
const int EchoPin = 5;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop()
{
  // establish variables for duration of the ping,
  // and the distance result in centimeters:

  long duration, cm;
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

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(1000);
}

long microsecondsToCentimeters(long microseconds)

{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
