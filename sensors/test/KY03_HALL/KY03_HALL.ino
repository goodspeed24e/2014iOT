//Arduino 霍尔磁力传感器模块 KY-003
//霍尔磁力传感器模块和数字13 接口自带LED 搭建简单电路，制作磁力闪光器。
//利用数字13 接口自带的LED，将霍尔磁力传感器接入ARDUINO给力板数字3接口，当霍尔磁力传感器到有磁场信号时，LED 灯亮,反之则灯灭。
//附参考程序:

int Led=13;//定义LED 接口
int SENSOR=3;//定义霍尔磁力传感器接口
int val;//定义数字变量val
void setup()
{
  Serial.begin(9600);
  pinMode(Led,OUTPUT);//定义LED 为输出接口
  pinMode (SENSOR,INPUT);//定义霍尔磁力传感器为输出接口
}
void loop()
{
  val=digitalRead(SENSOR);//将数字接口3 的值读取赋给val
  if(val==HIGH)//当震动传感器检测有信号时，LED 亮
  {
    digitalWrite(Led, LOW);
  }
  else
  {
    digitalWrite(Led, HIGH);
  }
  Serial.println(val); 
}

