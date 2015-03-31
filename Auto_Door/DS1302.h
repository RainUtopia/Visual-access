#ifndef __DS1302_H_
#define __DS1302_H_

#define DS1302_SCLK   P17                   //DS1302时钟口P1.0
#define DS1302_IO  	 P16                     //DS1302数据口P1.1
#define DS1302_RST 	 P15                    //DS1302片选口P1.2

extern	unsigned char code init[];
extern	signed char  now[];


void DS1302_Initial();		//初始化DS1302
void DS1302_SetTime(unsigned char *p);	//设置初始时间
void DS1302_GetTime(unsigned char *p);	//读取当前时间

#endif