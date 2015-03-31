#ifndef __OV7670_H
#define __OV7670_H

sbit	VSYNC=P4^6;
sbit	HREF=P4^5;
sbit	RESET=P4^4;
sbit	PWDN=P4^3;


void	OV7670_Init();
void OV7670_off();
void OV7670_on();

#endif
