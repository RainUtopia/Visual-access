#ifndef __net_camera_H__
#define __net_camera_H__

extern	unsigned char data txbuf[32];	//���ͻ�����
extern	unsigned char count;			//���ڼ��㷢�ͻ��������ݴ�С
extern	unsigned char Length,Width;		//ͼ�����

void Nrf24L01_Camera(void);	//NRF24L01����һ��ͼ��
void Uart_Camera(void);			//���ڷ���һ��ͼ��
void Nrf24L01_Camera_Test(void);

#endif