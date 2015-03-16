//KEYES 科易 ARDUINO   水银倾斜开关模块 
//水银开关模块和数字13 接口自带LED 搭建简单电路，制作倾斜提示灯
//利用数字13 接口自带的LED，将水银倾斜开关传感器接入数字3接口，当水银倾斜开关传感器感测到有按键信号时，LED 亮,反之则灭.
//例程源代码：
int Led=13;//定义LED 接口
int buttonpin=3; //定义水银倾斜开关传感器接口
int val;//定义数字变量val
void setup()
{
  pinMode(Led,OUTPUT);//定义LED 为输出接口
  pinMode(buttonpin,INPUT);//定义水银倾斜开关传感器为输出接口
}
void loop()
{
  val=digitalRead(buttonpin);//将数字接口3的值读取赋给val
  if(val==HIGH)//当水银倾斜开关传感器检测有信号时，LED 闪烁
  {
    digitalWrite(Led,HIGH);
  }
  else
  {
    digitalWrite(Led,LOW);
  }
}

