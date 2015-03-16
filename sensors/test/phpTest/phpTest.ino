const int ledPin = 13; // the pin that the LED 
int incomingByte;      // a variable to read incoming serial data into
  
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}
  
void loop() {
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(ledPin, LOW);
    }
  }
}

/*
<?php
function openSerial($command) {
    $openSerialOK = false;
    try {
        exec("mode com8: BAUD=9600 PARITY=n DATA=8 STOP=1 to=off dtr=off rts=off");
        $fp =fopen("com8", "w");
        //$fp = fopen('/dev/ttyUSB0','r+'); //use this for Linux
        $openSerialOK = true;
    } catch(Exception $e) {
        echo 'Message: ' .$e->getMessage();
    }
 
    if($openSerialOK) {
        fwrite($fp, $command); //write string to serial
        fclose($fp);
    }   
}
 
openSerial("Without this line, the first control will not work. I don't know way.");
 
if(isset($_POST['submit1'])) {
    openSerial("H");
}
 
if(isset($_POST['submit2'])) {
    openSerial("L");
}
?>
 
<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
   <input type="submit" name="submit1" value="1 on"><br>
   <input type="submit" name="submit2" value="1 off"><br>
</form>
*/
