int sensorPin = 13;
int val = 0;
void setup() {                
 pinMode(sensorPin, OUTPUT);  //   定义13脚为数字输出接口
 Serial.begin(9600);
}

void loop() {
  digitalWrite(sensorPin, HIGH);   // 打开激光头
  delay(1000);              // 延时一秒
  digitalWrite(sensorPin, LOW);    // 关闭激光头
  delay(1000);              // 延时一秒
}

