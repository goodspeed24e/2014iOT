/*
    本示例为接受端，接受无符号长整形数据并将最后一个数据返回给发送段
     所有引脚连接方法
     rf24         arduino
     3.3v       3.3v
     GND        GND
     CE         D9
     CSN        D10
     SCK        D13
     MOSI       D11
     MISO       D12

*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

/*
  ping-back 接受端
*/


//
// 硬件配置
//

// 设置 nRF24L01+ CE与CSN引脚
RF24 radio(9,10);

// 设置数据通道地址
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void) {
    Serial.begin(9600);
    printf_begin();
    printf("\n\rRF24/examples/pingpair/\n\r");
    printf("ROLE: Pong back\n\r");
    //
    // 设置rf模块
    //
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
    radio.setChannel(110);

    // 设置crc校验长度
    // 两种 8位RF24_CRC_8 和 16位RF24_CRC_16
    radio.setCRCLength(RF24_CRC_16);

    // 打开两个通道用于两个设备进行来回的通信

    // 打开本端的通道用来写消息
    radio.openWritingPipe(pipes[1]);

    // 打开对方的通道用来读消息
    radio.openReadingPipe(1,pipes[0]);

    //
    // 开始监听
    //
    radio.startListening();

    //
    // 打印配置信息
    //
    radio.printDetails();
}

void loop(void) {

    // 是否有有效数据可以读取
    if (radio.available()) {

        unsigned long got_time;

        // 有效信息是否接收完
        bool done = false;
        while(!done) {
            // 获取最后获得的有效信息
            done = radio.read( &got_time, sizeof(unsigned long) );

            // 打印出来
            printf("Got payload %lu...",got_time);

            // 延迟一小会儿，便于完整接受下一个有效信息
            delay(20);
        }

        // 首先停止接受，便于发送一个返回信息
        radio.stopListening();

        // 这里将接受的数值减去100, 那边收到的话好对比发送的信息和对方接受到的结果
        got_time -= 100;

        // 发送
        radio.write( &got_time, sizeof(unsigned long) );
        printf("Sent response.\n\r");

        // 重新回到监听模式
        radio.startListening();

    }
}
