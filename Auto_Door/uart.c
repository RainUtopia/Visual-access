#include "uart.h"


bit Flag;
unsigned char GetC;

/*----------------------------
    UART1初始化
-----------------------------*/
void InitUART(void)
{
	SCON = 0x50;
	AUXR |= 0x40;
	AUXR &= 0xFE;
	TMOD &= 0x0F;
	TL1 = 0xBF;
	TH1 = 0xFF;	
	ET1 = 0;
	TI=1;
	TR1 = 1;
//  ES = 1;
//  EA = 1;
}


/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
    RI = 0;                 //清除RI位
    GetC = SBUF; 
		Flag=1;
    }


}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char dat)
{
    while (0==TI);               //等待前面的数据发送完成
    TI = 0;                 //清除TI位
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(unsigned char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

