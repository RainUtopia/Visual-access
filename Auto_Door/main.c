#include <stc\stc15f2k60s2.h>
#include "delay.h"
#include "12864.h"
#include "DS1302.h"
#include "menu.h"
#include "uart.h"
#include "sccb.h"
#include "fifo.h"
#include "OV7670.h"
#include	"NRF24L01.H"
#include	"net_camera.h"

void sys_ui(void);	//待机界面

void Main()
{
	lcdinit();
	lcd_msg("时钟初始化");
	delay_ms(300);
	DS1302_Initial();
	DS1302_SetTime(init);
	InitUART();
	lcd_msg("摄像头初始化");
	delay_ms(300);
	OV7670_Init();
	FIFO_Init();
	FIFO_sav();		//如果摄像头工作不正常,程序停留在这里
	lcd_msg("通信初始化");
	delay_ms(300);
	NRF24L01_Init();
	NRF24L01_Check();
	while(0 == (NRF24L01_FLAG & NRF24L01_CHECK));
	while(1)
	{
		sys_ui();
		key=keyscan();
		if(key==K1)	gui();
	if(come)	{lcd_msg("  请进! ");	come=0;	delay_ms(3000);}
	 if(wait)	{lcd_msg("  请稍等");	wait=0;	delay_ms(3000);}
	 if(overlook)	{lcd_msg("对方忙绿");	overlook=0;	delay_ms(3000);}
	}
}

void sys_ui(void)
{
	DS1302_GetTime(now);
	lcd_set_xy(0,0);
	lcd_disp_string("  郑州科技学院  ");
	lcd_set_xy(1,0);
	lcd_disp_string("20");
	lcd_disp_number(now[6]>>4);
	lcd_disp_number(now[6]%16);
	lcd_disp_string("/");
	lcd_disp_number(now[4]>>4);
	lcd_disp_number(now[4]%16);
	lcd_disp_string("/");
	lcd_disp_number(now[3]>>4);
	lcd_disp_number(now[3]%16);
	lcd_disp_string("星期");
	lcd_disp_week(now[5]);
	lcd_set_xy(2,0);
	lcd_disp_string("    ");
	lcd_disp_number(now[2]>>4);
	lcd_disp_number(now[2]%16);
	lcd_disp_string(":");
	lcd_disp_number(now[1]>>4);
	lcd_disp_number(now[1]%16);
	lcd_disp_string(":");
	lcd_disp_number(now[0]>>4);
	lcd_disp_number(now[0]%16);
	lcd_disp_string("    ");
	lcd_set_xy(3,0);
	lcd_disp_string("请按OK键进入菜单");
}
	