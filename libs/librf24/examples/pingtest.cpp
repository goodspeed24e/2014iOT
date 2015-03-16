/**
* Example RF Radio Ping Pair
*
* This is an example of how to use the RF24 class.  Write this sketch to two different nodes,
* connect the role_pin to ground on one.  The ping node sends the current time to the pong node,
* which responds by sending the value back.  The ping node can then see how long the whole cycle
* took.
*/

#include <cstdlib>
#include <iostream>
#include "../RF24.h"

/*
    RPI connection concept

    rf24        rasp
    3.3v        3.3v (不能使用5v)
    GND         GND
    CE          GPIO 18 (右排往下第六个)
    CSN         GPIO 8 (右排往下倒数第二个)
    SCK         GPIO 11 (左排往下倒数第二个)  
    MOSI        GPIO 10 (左排往下倒数第四个)
    MISO        GPIO 9  (左排往下倒数第三个)
*/

// sensor type
enum SENSOR_TYPE {
  SENSOR_TYPE_UNKNOWN = 0x00,
  SENSOR_TYPE_SHOCK,
  SENSOR_TYPE_FLAME,
  SENSOR_TYPE_GAS,
  SENSOR_TYPE_INTRUSION,
  SENSOR_TYPE_MAX = 0x80
};

// hardware allocation
// spi设备、CSN速率、CSN引脚 GPIO 8
RF24 radio("/dev/spidev0.0", 8000000, 18); 

// set channel number
const int channel = 110;

// set data channel address
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// setup rf24
void setup(void) 
{

    radio.begin();

    // 开启动态有效信息长度
    radio.enableDynamicPayloads();

    // 设置重传次数以及每次重传的延迟
    //radio.setRetries(15,15);

    // 设置传输速率
    radio.setDataRate(RF24_1MBPS);

    // 设置功放级别，有四种级别：
    // RF24_PA_MIN=-18dBm
    // RF24_PA_LOW=-12dBm
    // RF24_PA_MED=-6dBM
    // RF24_PA_HIGH=0dBm
    radio.setPALevel(RF24_PA_HIGH);

    // 设置信道(0-127)
    radio.setChannel(channel);

    // 设置crc校验长度
    // 两种 8位RF24_CRC_8 和 16位RF24_CRC_16
    radio.setCRCLength(RF24_CRC_16);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);


    //
    // 开始监听
    //

    radio.startListening();

    // 打印配置信息
    radio.printDetails();
}

const static unsigned long GAS_RESET = 0x01;
const static unsigned long FLAME_RESET = 0x02;
const static unsigned long GAS_THRESHOLD = 600;
const static unsigned long FLAME_THRESHOLD = 300;
static bool updateGas = false;
static bool updateFlame = false;

const int counter_bias = 100;
static unsigned long cnt = counter_bias;
unsigned long triggerEvent()
{
   if (cnt > 120)
      cnt = counter_bias;
   else 
      cnt++;
 
   if (cnt == 110)
      return GAS_RESET;
   if (cnt == 120)
      return FLAME_RESET;

   return cnt;
}

void loop(void) 
{
    // 首先停止监听
    radio.stopListening();

    // 获取时间，并发送时间
    unsigned long time = __millis();

    bool ok = false;

    // query event
    //unsigned long ulVal = triggerEvent();
    //if (updateGas == false && ulVal == GAS_RESET)
    //{
    //   time = GAS_RESET;
    //   updateGas = true;
    //}
    //else if (updateFlame == false && ulVal == FLAME_RESET) 
    //{
    //   time = FLAME_RESET;
    //   updateFlame = true;
    //}
    //else if (updateGas && time != GAS_RESET)
    //{
    //   time = GAS_THRESHOLD;
    //   updateGas = false;
    //}
    //else if (updateFlame && time != FLAME_RESET)
    //{
    //   time = FLAME_THRESHOLD;
    //   updateFlame = false;
    //}
   
    printf("Now sending %lu...", time);

    // 是否发送成功
    ok = radio.write( &time, sizeof(unsigned long) );
    if (ok)
        printf("ok...");
    else
        printf("failed.\n\r");
    
    // 继续监听
    radio.startListening();

    // 等待对方返回数据，超时时间 250ms
    unsigned long started_waiting_at = __millis();

    bool timeout = false;
    while ( !radio.available() && !timeout ) 
    {

        //稍微延迟一下，等待radio.available()检测有效数据
        __msleep(5);
        if (__millis() - started_waiting_at > 200 )
   	     timeout = true;
    }

    // 是否超时
    if ( timeout ) 
    {
        printf("Failed, response timed out.\n\r");
    }
    else 
    {
        // 读取返回信息，并打印出来
        unsigned long got_msg;
        radio.read( &got_msg, sizeof(unsigned long) );

	if (got_msg > SENSOR_TYPE_MAX)
	{
		printf("Got response %lu, round-trip delay: %lu\n\r",got_msg,__millis()-got_msg);
	}
	else
	{
		switch (got_msg)
		{
 		case SENSOR_TYPE_UNKNOWN:
	 	    printf("UNKNOWN SENSOR TYPE ---%lu---\n\r", got_msg);
		break;
  		case SENSOR_TYPE_SHOCK:
  	  	    printf("SHOCK!!! ---%lu---\n\r", got_msg);
		break;
		case SENSOR_TYPE_FLAME:
  		    printf("FLAME!!! ---%lu---\n\r", got_msg);
		break;
		case SENSOR_TYPE_GAS:
  		    printf("GAS!!! ---%lu---\n\r", got_msg);
		break;
		case SENSOR_TYPE_INTRUSION:
        	    printf("INTRUSION!!! ---%lu---\n\r", got_msg);
		break;
		defualt:
		    printf("Got response %lu, round-trip delay: %lu\n\r",got_msg,__millis()-got_msg);
		}
	}
    }

    //延迟一会儿
    sleep(1);
}

int main(int argc, char** argv)
 {
    setup();

    while(1)
        loop();

    return 0;
}
