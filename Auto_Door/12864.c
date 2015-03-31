/********************************************************************
����12864��ʾ
Fancy	QQ:652774317
***********************************************************************/
#include <stc\stc15f2k60s2.h>
#include "delay.h"
#include "12864.h"


//����Һ���Ĵ���ͨ��Э�飬��������
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


//д����ָ��
void write_com(unsigned char cmdcode)
{
	CS = 1;
	sendbyte(0xf8);
	sendbyte(cmdcode & 0xf0);
	sendbyte((cmdcode << 4) & 0xf0);
	delay_us(200);
}

//д��������
void write_data(unsigned char Dispdata)
{
	CS = 1;
	sendbyte(0xfa);
	sendbyte(Dispdata & 0xf0);
	sendbyte((Dispdata << 4) & 0xf0);
	delay_us(200);
}
//��������
void clr_Scr(void)
{
	write_com(0x01);
	delay_ms(2);
}
//lcd��ʼ��
void lcdinit()
{  
	P47=1;
	write_com(0x30);
	delay_us(200);
	write_com(0x0c);
	delay_us(200);
	clr_Scr();
}

//������ʼ��ַ����,x��ʾ��0-3,y��ʾ��0-7
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
// �ַ�����ʾ����
void lcd_disp_string(unsigned char *str) //д���ַ���
{

   while(*str)
		{
     write_data(*str);
		 str++;
		}
}
//������ʾ����ʮ���������¾���
void lcd_disp_number(unsigned char s)
{	
	if(s<=9)	write_data(s+0x30);
	else	write_data(s+0x37);
}
//��ʾ��Ϣ��
void	lcd_msg(unsigned char *s)
{
	clr_Scr();
	lcd_set_xy(1,1);
	lcd_disp_string(s);
}
//������ʾ
void	lcd_disp_week(unsigned char s)
{
	unsigned char temp[14]={0XD2,0XBB,	0XB6,0XFE,	0XC8,0XFD,	0XCB,0XC4,	0XCE,0XE5,	0XC1,0XF9,	0XC8,0XD5};
	write_data(temp[(s-1)*2]);
	write_data(temp[(s-1)*2+1]);
}