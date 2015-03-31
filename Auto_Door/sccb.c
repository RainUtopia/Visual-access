
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
     unsigned char  i=0;  //并行数据转串行输出，串行数据输出的顺序为先高位再低位
       
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
      
                                   //第9位，Don’t Care
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
       //串行数据转并行输入，高位在前
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
 //3相写传输周期
       sccb_start();                             //启动SCCB
       sccb_sendbyte(0x42);                //OV7670设备从地址，写操作
       sccb_sendbyte(subaddr);            //设备内存地址
       sccb_sendbyte(value);                 //写数据字节
       sccb_end();                              //结束SCCB     
       delay_us(10);  
}
unsigned char sccb_receivedata(unsigned char subaddr)
{
 unsigned char temp;
      
       //2相写传输周期
       sccb_start();                             //启动SCCB
       sccb_sendbyte(0x42);               //OV7670设备从地址，写操作
       sccb_sendbyte(subaddr);            //设备内存地址
       sccb_end();                              //结束SCCB
      
       //2相读传输周期
       sccb_start();                             //启动SCCB
       sccb_sendbyte(0x43);                //OV7670设备从地址，读操作
       sccb_receivebyte(&temp);         //读字节
       sccb_end();                              //结束SCCB
       return temp;   
  
}

