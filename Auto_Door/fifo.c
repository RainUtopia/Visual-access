#include <stc\stc15f2k60s2.h>
#include "delay.h"
#include "fifo.h"
#include "OV7670.h"

void FIFO_Reset_Read_Addr()
{
 FIFO_RRST=0;
 FIFO_RCK=0; 
 FIFO_RCK=1;
 FIFO_RCK=0; 
 FIFO_RRST=1;
 FIFO_RCK=1; 
}
void FIFO_Reset_Write_Addr()
{
 FIFO_WRST=0;
 delay_us(10);
 FIFO_WRST=1;  
}

void	FIFO_Init()
{
	FIFO_OE=0;
	FIFO_WR=0;
	FIFO_Reset_Write_Addr();
	FIFO_Reset_Read_Addr();
}

void FIFO_sav(void)
{
	FIFO_Reset_Write_Addr();
	FIFO_Reset_Read_Addr();
	while(VSYNC);
	while(!VSYNC);
	FIFO_WR=1;
	while(VSYNC);
	while(!VSYNC);
	FIFO_WR=0;
}