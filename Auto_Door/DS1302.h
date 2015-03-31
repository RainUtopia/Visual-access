#ifndef __DS1302_H_
#define __DS1302_H_

#define DS1302_SCLK   P17                   //DS1302ʱ�ӿ�P1.0
#define DS1302_IO  	 P16                     //DS1302���ݿ�P1.1
#define DS1302_RST 	 P15                    //DS1302Ƭѡ��P1.2

extern	unsigned char code init[];
extern	signed char  now[];


void DS1302_Initial();		//��ʼ��DS1302
void DS1302_SetTime(unsigned char *p);	//���ó�ʼʱ��
void DS1302_GetTime(unsigned char *p);	//��ȡ��ǰʱ��

#endif