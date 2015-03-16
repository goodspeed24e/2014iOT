//KEYES 科易 ARDUINO 震动开关模块

//震动模块和数字13 接口自带LED 搭建简单电路，制作震动闪光器。
//利用数字13 接口自带的LED，将震动传感器接入数字3接口，当震动传感器感测
//到有震动信号时，LED 闪烁发光。
//例程源代码：
int Led=13;//定义LED 接口
int Shock=3;//定义震动传感器接口
int val;//定义数字变量val
void setup()
{
  pinMode(Led,OUTPUT);//定义LED 为输出接口
  pinMode(Shock,INPUT);//定义震动传感器为输出接口
}
void loop()
{
  val=digitalRead(Shock);//将数字接口3的值读取赋给val
  if(val==HIGH)//当震动传感器检测有信号时，LED 闪烁
  {
  digitalWrite(Led,LOW);
  }
  else
  {
  digitalWrite(Led,HIGH);
  }
}


