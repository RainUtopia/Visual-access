/********************************************************************
串行12864显示
Fancy	QQ:652774317
***********************************************************************/
#include <stc\stc15f2k60s2.h>
#include "delay.h"
#include "12864.h"


//按照液晶的串口通信协议，发送数据
void sendbyte(unsigned char zdata)
{
	unsigned int i;
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			SID = 1;
		}
		else 
		{
			SID = 0;
		}
		SCLK = 0;
		SCLK = 1;
	}
}


//写串口指令
void write_com(unsigned char cmdcode)
{
	CS = 1;
	sendbyte(0xf8);
	sendbyte(cmdcode & 0xf0);
	sendbyte((cmdcode << 4) & 0xf0);
	delay_us(200);
}

//写串口数据
void write_data(unsigned char Dispdata)
{
	CS = 1;
	sendbyte(0xfa);
	sendbyte(Dispdata & 0xf0);
	sendbyte((Dispdata << 4) & 0xf0);
	delay_us(200);
}
//清屏函数
void clr_Scr(void)
{
	write_com(0x01);
	delay_ms(2);
}
//lcd初始化
void lcdinit()
{  
	P47=1;
	write_com(0x30);
	delay_us(200);
	write_com(0x0c);
	delay_us(200);
	clr_Scr();
}

//设置起始地址函数,x表示行0-3,y表示列0-7
void lcd_set_xy( unsigned char x, unsigned char y )
{	
    unsigned char address;
	switch(x)
	{
    case 0: address = 0x80 + y; break; 
		case 1: address = 0x90 + y; break; 
   	case 2: address = 0x88 + y; break;
		case 3: address = 0x98 + y; break; 
		default:address = 0x80 + y; break;
	}
  write_com(address);
}
// 字符串显示函数
void lcd_disp_string(unsigned char *str) //写入字符串
{

   while(*str)
		{
     write_data(*str);
		 str++;
		}
}
//数字显示函数十六进制以下均可
void lcd_disp_number(unsigned char s)
{	
	if(s<=9)	write_data(s+0x30);
	else	write_data(s+0x37);
}
//显示消息框
void	lcd_msg(unsigned char *s)
{
	clr_Scr();
	lcd_set_xy(1,1);
	lcd_disp_string(s);
}
//星期显示
void	lcd_disp_week(unsigned char s)
{
	unsigned char temp[14]={0XD2,0XBB,	0XB6,0XFE,	0XC8,0XFD,	0XCB,0XC4,	0XCE,0XE5,	0XC1,0XF9,	0XC8,0XD5};
	write_data(temp[(s-1)*2]);
	write_data(temp[(s-1)*2+1]);
}