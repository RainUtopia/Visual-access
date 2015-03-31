#ifndef __12864_H_
#define __12864_H_

#define CS		P14    //片选信号
#define SID 	P13		//数据信号
#define SCLK	P12		//时钟信号 
#define BK		P47		//背光

void sendbyte(unsigned char zdata);					//按照液晶的串口通信协议，发送数据
void write_com(unsigned char cmdcode);			//写串口指令
void write_data(unsigned char Dispdata);		//写串口数据
void clr_Scr(void);													//清屏函数
void lcdinit();															//lcd初始化
void lcd_set_xy( unsigned char x, unsigned char y );	//设置起始地址函数,x表示行0-3,y表示列0-7
void lcd_disp_string(unsigned char *str); //向X,Y坐标开始写入字符串
void lcd_disp_number(unsigned char s);			//数字显示函数
void	lcd_msg(unsigned char *s);						//显示消息框
void	lcd_disp_week(unsigned char s);				//星期显示
void find_person(void);											//找人子程序

#endif