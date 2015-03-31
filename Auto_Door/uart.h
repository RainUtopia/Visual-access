#ifndef __uart_H_
#define __uart_H_

#include	<STC/STC15F2K60S2.H>

extern	bit Flag;
extern	unsigned char GetC;
//24MHZ 9600
void InitUART(void);
void SendData(unsigned char dat);
void SendString(unsigned char *s);

#endif