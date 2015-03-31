#include "uart.h"


bit Flag;
unsigned char GetC;

/*----------------------------
    UART1��ʼ��
-----------------------------*/
void InitUART(void)
{
	SCON = 0x50;
	AUXR |= 0x40;
	AUXR &= 0xFE;
	TMOD &= 0x0F;
	TL1 = 0xBF;
	TH1 = 0xFF;	
	ET1 = 0;
	TI=1;
	TR1 = 1;
//  ES = 1;
//  EA = 1;
}


/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
    RI = 0;                 //���RIλ
    GetC = SBUF; 
		Flag=1;
    }


}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(unsigned char dat)
{
    while (0==TI);               //�ȴ�ǰ������ݷ������
    TI = 0;                 //���TIλ
    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(unsigned char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

