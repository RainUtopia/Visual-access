#include 	<stc\stc15f2k60s2.h>
#include	"delay.h"
#include	"uart.h"
#include	"sccb.h"
#include	"fifo.h"
#include	"OV7670.h"
#include	"NRF24L01.H"
#include	"net_camera.h"
#include "12864.h"
#include "intrins.h"


//串口发送一幅图像
void Uart_Camera(void)
{
	unsigned char bmp;
	lcd_msg("串口传图..");
	//保存一幅图像到FIFO
	FIFO_sav();
	//图像帧头
	SendData(0x01);
	SendData(0xFE);
	for(Length=0;Length<120;Length++)
		{
			for(Width=0;Width<160;Width++)
				{
						FIFO_RCK=0;
						bmp=DATA_PORT;
						SendData(bmp);
						FIFO_RCK=1;
						FIFO_RCK=0;
						bmp=DATA_PORT;
						SendData(bmp);
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
			for(Width=0;Width<160;Width++)
				{
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
		}
		//图像结束标志
		SendData(0xFE);
		SendData(0x01);
		lcd_msg("发送成功..");
		delay_ms(500);
}

unsigned char Length,Width;
unsigned char count;			//用于计算发送缓冲区数据大小
unsigned char data txbuf[32];
//NRF24L01发送一幅图像
void Nrf24L01_Camera_Test(void)
{
	count=0;
	lcd_msg("无线传图40CH");
	//保存一幅图像到FIFO
	FIFO_sav();
	//图像帧头
	txbuf[0]=0x01;
	txbuf[1]=0xfe;
	while(NRF24L01_FLAG & NRF24L01_BUSY);
	NRF24L01_SendFrame(txbuf,2);
	for(Length=0;Length<120;Length++)
		{
			for(Width=0;Width<160;Width++)
				{
					FIFO_RCK=0;
					txbuf[count++]=DATA_PORT;
					if(count==32)
						{
							count=0;
							while(NRF24L01_FLAG & NRF24L01_BUSY);
							NRF24L01_SendFrame(txbuf,32);
							delay_ms(2);
						}
					FIFO_RCK=1;
					FIFO_RCK=0;
					txbuf[count++]=DATA_PORT;
					if(count==32)
						{
							count=0;
							while(NRF24L01_FLAG & NRF24L01_BUSY);
							NRF24L01_SendFrame(txbuf,32);
							delay_ms(2);
						}
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
			for(Width=0;Width<160;Width++)
				{
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
		}
		//图像结束标志
		txbuf[0]=0xfe;
		txbuf[1]=0x01;
		while(NRF24L01_FLAG & NRF24L01_BUSY);
		NRF24L01_SendFrame(txbuf,2);
		lcd_msg("发送成功..");
		delay_ms(500);
}

//NRF24L01发送一幅编码图像
void Nrf24L01_Camera(void)
{
	unsigned int point;

	lcd_msg("发送图片..");
	//保存一幅图像到FIFO
	FIFO_sav();
	txbuf[0]=0x01;
	txbuf[1]=0xfe;
	count=4;
	point=0;
	for(Length=0;Length<120;Length++)
		{
			for(Width=0;Width<160;Width++)
				{
					FIFO_RCK=0;
					txbuf[count++]=DATA_PORT;
					if(count==28)
						{
							count=4;
							txbuf[2]=(point>>8);
							txbuf[3]=point;
							point++;
							while(NRF24L01_FLAG & NRF24L01_BUSY);
							NRF24L01_SendFrame(txbuf,28);
//							delay_ms(1);
						}
					FIFO_RCK=1;
					FIFO_RCK=0;
					txbuf[count++]=DATA_PORT;
					if(count==28)
						{
							count=4;
							txbuf[2]=(point>>8);
							txbuf[3]=point;
							point++;
							while(NRF24L01_FLAG & NRF24L01_BUSY);
							NRF24L01_SendFrame(txbuf,28);
//							delay_ms(1);
						}
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
			for(Width=0;Width<160;Width++)
				{
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
						FIFO_RCK=0;
						FIFO_RCK=1;
				}
		}
		lcd_msg("发送成功..");
		delay_ms(500);
}