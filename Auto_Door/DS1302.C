#include <stc\stc15f2k60s2.h>
#include "delay.h"
#include "DS1302.h"

															//秒    分    时    日    月  星期    年
unsigned char code init[] = {0x00, 0x00, 0x12, 0x20, 0x08, 0x03, 0x14};
signed char  now[7];


/**************************************
从DS1302读1字节数据
**************************************/
unsigned char DS1302_ReadByte()
{
    unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
		DS1302_SCLK = 0;                   //时钟线拉低
		delay_us(2);                //延时等待
        dat >>= 1;	                //数据右移一位
		if (DS1302_IO) dat |= 0x80;        //读取数据
		DS1302_SCLK = 1;                   //时钟线拉高
		delay_us(2);                //延时等待
	}

    return dat;
}

/**************************************
向DS1302写1字节数据
**************************************/
void DS1302_WriteByte(unsigned char dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        DS1302_SCLK = 0;                   //时钟线拉低
        delay_us(2);                //延时等待
        dat >>= 1;                  //移出数据
        DS1302_IO = CY;                    //送出到端口
        DS1302_SCLK = 1;                   //时钟线拉高
        delay_us(2);                //延时等待
    }
}

/**************************************
读DS1302某地址的的数据
**************************************/
unsigned char DS1302_ReadData(unsigned char addr)
{
    unsigned char dat;

    DS1302_RST = 0;
    delay_us(2);
    DS1302_SCLK = 0;
    delay_us(2);
    DS1302_RST = 1;
    delay_us(2);
    DS1302_WriteByte(addr);         //写地址
    dat = DS1302_ReadByte();        //读数据
    DS1302_SCLK = 1;
    DS1302_RST = 0;

    return dat;
}

/**************************************
往DS1302的某个地址写入数据
**************************************/
void DS1302_WriteData(unsigned char addr, unsigned char dat)
{
    DS1302_RST = 0;
    delay_us(2);
    DS1302_SCLK = 0;
    delay_us(2);
    DS1302_RST = 1;
    delay_us(2);
    DS1302_WriteByte(addr);         //写地址
    DS1302_WriteByte(dat);          //写数据
    DS1302_SCLK = 1;
    DS1302_RST = 0;
}

/**************************************
写入初始时间
**************************************/
void DS1302_SetTime(unsigned char *p)
{
    unsigned char addr = 0x80;
    unsigned char n = 7;

    DS1302_WriteData(0x8e, 0x00);   //允许写操作
    while (n--)
    {
        DS1302_WriteData(addr, *p++);
        addr += 2;
    }
    DS1302_WriteData(0x8e, 0x80);   //写保护
}

/**************************************
读取当前时间
**************************************/
void DS1302_GetTime(unsigned char *p)
{
    unsigned char addr = 0x81;
    unsigned char n = 7;

    while (n--)
    {
        *p++ = DS1302_ReadData(addr);
        addr += 2;
    }
}

/**************************************
初始化DS1302
**************************************/
void DS1302_Initial()
{
    DS1302_RST = 0;
    DS1302_SCLK = 0;
    DS1302_WriteData(0x8e, 0x00);   //允许写操作
    DS1302_WriteData(0x80, 0x00);   //时钟启动
    DS1302_WriteData(0x90, 0xa6);   //一个二极管＋4K电阻充电
    DS1302_WriteData(0x8e, 0x80);   //写保护
}
