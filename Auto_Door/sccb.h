#ifndef _SCCB_H_H
#define _SCCB_H_H 

void sccb_start(void);
void sccb_end(void);
void sccb_sendbyte(unsigned char value);
void sccb_receivebyte(unsigned char *value);
void sccb_senddata(unsigned char subaddr,unsigned char value);
unsigned char sccb_receivedata(unsigned char subaddr);

#endif