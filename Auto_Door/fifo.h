#ifndef _FIFO_H_
#define _FIFO_H_


//===========FIFO PIN============
sbit	FIFO_RCK=P4^2;
sbit	FIFO_WR=P4^1;
sbit	FIFO_OE=P0^2;
sbit	FIFO_RRST=P0^4;
sbit	FIFO_WRST=P0^3;

#define DATA_PORT P2
//===================================

void FIFO_Reset_Read_Addr();
void FIFO_Reset_Write_Addr();
void	FIFO_Init();
void FIFO_sav(void);
#endif