
#include "sccb.h"
#include "delay.h"
#include <stc\stc15f2k60s2.h>

#define SCCB_SDA	P00
#define SCCB_SCL	P01

#define CLOCK_LOW()         (SCCB_SCL = 0) 
#define CLOCK_HIGH()        (SCCB_SCL = 1)    
#define DATA_LOW()          (SCCB_SDA = 0)    
#define DATA_HIGH()         (SCCB_SDA = 1)      


void sccb_start(void)
{
    CLOCK_HIGH();
    DATA_HIGH();
    delay_us(1);
    DATA_LOW();
    delay_us(1);
    CLOCK_LOW();
    delay_us(1);
}
  
void sccb_end(void) 
{
     DATA_LOW();
     delay_us(1);
     CLOCK_HIGH();
     delay_us(1);
     DATA_HIGH();
     delay_us(1);  
}

void sccb_sendbyte( unsigned char value )
{
     unsigned char  i=0;  //��������ת����������������������˳��Ϊ�ȸ�λ�ٵ�λ
       
     for(i=0;i<8;i++)
     {
          if( value & 0x80 )
                DATA_HIGH();
          else
                DATA_LOW();
                    
           delay_us(1);
           CLOCK_HIGH();
           delay_us(1);
           CLOCK_LOW();
           delay_us(1);
           DATA_LOW();
           delay_us(1);
             
           value<<=1; 
       }
      
                                   //��9λ��Don��t Care
       DATA_HIGH(); 
       delay_us(1);
       CLOCK_HIGH(); 
       delay_us(1);
       CLOCK_LOW();
       delay_us(1);      
}


void sccb_receivebyte(unsigned char *value) 
{
       int i=0;
       int svalue=0;
       int pvalue = 0;
       //��������ת�������룬��λ��ǰ
       for(i=7;i>=0;i--)
       {
              CLOCK_HIGH();
              delay_us(1);
							svalue= SCCB_SDA;
              CLOCK_LOW();
              delay_us(1);
              pvalue |= svalue<<i;
       }
       DATA_HIGH();
       delay_us(1);
       CLOCK_HIGH();
       delay_us(1);
       CLOCK_LOW();
       delay_us(1); 
       *value = (unsigned char)(pvalue & 0xff);   
}

void sccb_senddata(unsigned char subaddr,unsigned char value) 
{
 //3��д��������
       sccb_start();                             //����SCCB
       sccb_sendbyte(0x42);                //OV7670�豸�ӵ�ַ��д����
       sccb_sendbyte(subaddr);            //�豸�ڴ��ַ
       sccb_sendbyte(value);                 //д�����ֽ�
       sccb_end();                              //����SCCB     
       delay_us(10);  
}
unsigned char sccb_receivedata(unsigned char subaddr)
{
 unsigned char temp;
      
       //2��д��������
       sccb_start();                             //����SCCB
       sccb_sendbyte(0x42);               //OV7670�豸�ӵ�ַ��д����
       sccb_sendbyte(subaddr);            //�豸�ڴ��ַ
       sccb_end();                              //����SCCB
      
       //2�����������
       sccb_start();                             //����SCCB
       sccb_sendbyte(0x43);                //OV7670�豸�ӵ�ַ��������
       sccb_receivebyte(&temp);         //���ֽ�
       sccb_end();                              //����SCCB
       return temp;   
  
}

