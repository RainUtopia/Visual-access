#ifndef __net_camera_H__
#define __net_camera_H__

extern	unsigned char data txbuf[32];	//发送缓冲区
extern	unsigned char count;			//用于计算发送缓冲区数据大小
extern	unsigned char Length,Width;		//图像长与宽

void Nrf24L01_Camera(void);	//NRF24L01发送一幅图像
void Uart_Camera(void);			//串口发送一幅图像
void Nrf24L01_Camera_Test(void);

#endif