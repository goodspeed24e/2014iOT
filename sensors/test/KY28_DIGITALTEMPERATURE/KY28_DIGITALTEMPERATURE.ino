//KEYES 科易 ARDUINO 数字温度模块 
//数字温度模块和数字13 接口自带LED 搭建简单电路，制作温度提示灯
//利用数字13 接口自带的LED，将数字温度传感器接入数字3接口，当数字温度传感器感测到有按键信号时，LED 亮,反之则灭.
//例程源代码：
int Led=13;//定义LED 接口
int buttonpin=8; //定义数字温度传感器接口
int val;//定义数字变量val
void setup()
{
pinMode(Led,OUTPUT);//定义LED 为输出接口
pinMode(buttonpin,INPUT);//定义数字温度传感器为输出接口
Serial.begin(9600);
}
void loop()
{
  val=digitalRead(buttonpin);//将数字接口3的值读取赋给val
  if(val==HIGH)//当数字温度传感器检测有信号时，LED 闪烁
  {
  digitalWrite(Led,HIGH);
  }
  else
  {
  digitalWrite(Led,LOW);
  }
  delay(100);       
  Serial.println(val);
}

