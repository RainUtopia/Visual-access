/****************************************
NRF24L01�⺯��
STC15F2K61S2�Ѳ���OK
֧��24L01+��BK2423
֧��ACK��NOACK
֧�ַ������Զ��л�������ģʽ
�жϴ�����Ϣ
������:Fancy 
QQ:652774317
******************************************/

#include	<STC/STC15F2K60S2.H>
#include	"NRF24L01.H"


unsigned char RF_CH=0x40;		//RFƵ��
unsigned char NRF24L01_FLAG=0;	//NRF24L01��ǰ״̬��־
																//DB7; 1����ģʽ 			0����ģʽ
																//DB6; 1������   			0����
																//DB5; 1���ͳɹ� 			0����ʧ��(���DB6����Ƿ��ͳɹ�)
																//DB4; 1NRF�������� 	0 ��ⲻ������
																//DB3; 1���յ����ݰ� 	0 ������������
																//DB2; 1 ACKģʽ      0 NOACKģʽ
																//DB1; 1�������Զ��������ģʽ 0�ر��Զ��л�ģʽ
unsigned char DYNPD_LEN=0;			//���ڴ�Ŷ�̬���ݰ�����
unsigned char TX_ADDRESS[NRF24L01_ADR_LEN]= {0x12,0x34,0x56,0x78,0x90};	//���ص�ַ
unsigned char RX_ADDRESS[NRF24L01_ADR_LEN]= {0x12,0x34,0x56,0x78,0x90};	//���յ�ַ
unsigned char	rx_buf[32]={0};
bit	camera=0;
bit	come=0;
bit	wait=0;
bit	overlook=0;

//��ӻ�����һ���ֽ����ݲ����ؽ�������
/**************************************************/
bdata unsigned char st=0;
sbit st_1=st^0;
sbit st_2=st^1;
sbit st_3=st^2;
sbit st_4=st^3;
sbit st_5=st^4;
sbit st_6=st^5;
sbit st_7=st^6;
sbit st_8=st^7;
bdata unsigned char st1=0;
sbit st_11=st1^0;
sbit st_12=st1^1;
sbit st_13=st1^2;
sbit st_14=st1^3;
sbit st_15=st1^4;
sbit st_16=st1^5;
sbit st_17=st1^6;
sbit st_18=st1^7;
unsigned	char SPI_WriteRead(unsigned char byte)
{
    st=byte;

    SPI_MOSI=st_8;
    SPI_SCLK = 1;
    st_18=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_7;
    SPI_SCLK = 1;
    st_17=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_6;
    SPI_SCLK = 1;
    st_16=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_5;
    SPI_SCLK = 1;
    st_15=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_4;
    SPI_SCLK = 1;
    st_14=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_3;
    SPI_SCLK = 1;
    st_13=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_2;
    SPI_SCLK = 1;
    st_12=SPI_MISO;
    SPI_SCLK = 0;

    SPI_MOSI=st_1;
    SPI_SCLK = 1;
    st_11=SPI_MISO;
    SPI_SCLK = 0;
    return(st1);           		  // return read byte
}

//дNRF24L01�Ĵ���������״ֵ̬
unsigned char NRF24L01_Write_Reg(unsigned char reg, unsigned char value)
{
	 unsigned char BackDate;
   SPI_SS=0;
   SPI_WriteRead(reg);
   BackDate=SPI_WriteRead(value);
   SPI_SS=1;
   return(BackDate);
}
//��NRF24L01�Ĵ��������ؼĴ���ֵ
unsigned char NRF24L01_Read_Reg(unsigned char reg)
{
	 unsigned char BackDate;
   SPI_SS=0;
   SPI_WriteRead(reg);
   BackDate=SPI_WriteRead(0x00);
   SPI_SS=1;
   return(BackDate);
}
//��ȡ����������
void NRF24L01_Read_Buf(unsigned char reg, unsigned char *pBuf,unsigned char bytes)
{
	unsigned char byte_ctr;
	SPI_SS = 0;
	SPI_WriteRead(reg);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
	pBuf[byte_ctr] = SPI_WriteRead(0x00);
	SPI_SS = 1;
}
//�򻺳���д������
unsigned char NRF24L01_Write_Buf(unsigned char reg,unsigned char *pBuf,unsigned char bytes)
{
	unsigned char status,byte_ctr;
	SPI_SS = 0;
	status = SPI_WriteRead(reg);
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) 
	SPI_WriteRead(*pBuf++);
	SPI_SS = 1; 
	return(status);
}

//NRF24L01��ʼ������
void NRF24L01_Init(void)
{
	#if BK2423
	NRF24L01_CE=0;
	NRF24L01_Write_Reg(ACTIVATE_CMD, 0x73);
	NRF24L01_CE=1;
	#endif
	
	SPI_SS=1;			// Spi disable	
	SPI_SCLK=0;			// Spi clock line init high
	
	NRF24L01_CE=0;
  //���üĴ���
	NRF24L01_FLAG |= NRF24L01_AUTO_ACK;		//�Ƿ���Ӧ��ģʽ
	NRF24L01_FLAG |= NRF24L01_AUTO_RX;		//�������Ƿ��Զ��������ģʽ
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_SETUP_AW, NRF24L01_ADR_LEN-2);	//���õ�ַ����Ϊ NRF24L01_ADR_LEN
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_CH,RF_CH);                  //����RFͨ��ΪRFCH
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_SETUP, 0x27);               //����TX�������,7db����,250kbps,���������濪��
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_EN_AA, 0x01);         					//ʹ��ͨ��0���Զ�Ӧ��
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_EN_RXADDR, 0x01);              //ʹ��ͨ��0�Ľ��յ�ַ
  //RXģʽ����
  NRF24L01_Write_Buf(NRF24L01_WRITE_REG | NRF24L01_RX_ADDR_P0,RX_ADDRESS,NRF24L01_ADR_LEN); //дRX�ڵ��ַ
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FEATURE,0x05);									//ʹ�ܶ�̬���ݰ�����,AUTO_ACKģʽ
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_DYNPD,0X01);        						//ѡ��ͨ��0��̬���ݰ�����
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_RX,NRF24L01_NOP);                //���RX FIFO�Ĵ���
  //TXģʽ����
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG | NRF24L01_TX_ADDR ,TX_ADDRESS,NRF24L01_ADR_LEN);	//дTX�ڵ��ַ
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_SETUP_RETR, 0x3A);           //�����Զ��ط����ʱ��:750us + 86us;����Զ��ط�����:5��
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX, NRF24L01_NOP);              //���TX FIFO�Ĵ���
  NRF24L01_RxMode();                                                  					//Ĭ�Ͻ������ģʽ
  NRF24L01_CE=1;
	INT_CLKO |= 0x20;		//�����ⲿ�ж�3
	EA=1;
}

//NRF24L01���뷢��ģʽ
void NRF24L01_TxMode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_CONFIG, 0x0e);
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,0x70);		//����жϱ�־
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX,NRF24L01_NOP);  //���TX FIFO�Ĵ���
	NRF24L01_CE = 1; 
	NRF24L01_FLAG &= ~NRF24L01_MODE_RX;		//�޸�״̬��ʶΪ����ģʽ
	NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY��־��0
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//��������ݱ�־
}
//NRF24L01�������ģʽ
void NRF24L01_RxMode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_CONFIG, 0x0f); 	//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,0x70);		//����жϱ�־
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_RX,NRF24L01_NOP);  //���RX FIFO�Ĵ���
	NRF24L01_CE = 1; 
	NRF24L01_FLAG |= NRF24L01_MODE_RX;		//�޸�״̬��ʶΪ����ģʽ
	NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY��־��0
	DYNPD_LEN=0;	//��0��̬���ݰ�����
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//��������ݱ�־
}

//NRF24L01 SPIͨ�ż��
void NRF24L01_Check(void)
{
	unsigned char i=0;
	unsigned char Buff[5];
	NRF24L01_Read_Buf(NRF24L01_TX_ADDR,Buff,NRF24L01_ADR_LEN);
	i=NRF24L01_Read_Reg(NRF24L01_DYNPD);	//��ȡDYNPD����Ƿ���ȷд��
	if(i==0x01)		//�������õ���0x01,Ҫ���ʼ����ֵ���Ӧ
	{
  for(i = 0; i < NRF24L01_ADR_LEN; i++)
    {
			
        if(Buff[i] == TX_ADDRESS[i])
        {
         NRF24L01_FLAG |= NRF24L01_CHECK;
        }
				else 
				{
					NRF24L01_FLAG &= ~NRF24L01_CHECK;
					break;
				}
    }
	}
	else	NRF24L01_FLAG &= ~NRF24L01_CHECK;
}

//NRF24L01��������
void NRF24L01_SendFrame(unsigned char *temp,unsigned char len)
{
	if(NRF24L01_FLAG & NRF24L01_MODE_RX)
	{
		NRF24L01_TxMode();
	}
	NRF24L01_FLAG |= NRF24L01_BUSY;	//BUSY��־��1
	NRF24L01_FLAG &= ~NRF24L01_TX_ACCESS;//������ͳɹ���־
	NRF24L01_CE = 0;
	if(NRF24L01_FLAG & NRF24L01_AUTO_ACK)
		{
			NRF24L01_Write_Buf(NRF24L01_WR_TX_PLOAD,temp,len);	//д�������ݰ�,��Ҫ��Ӧ
		}
	else
	{
		NRF24L01_Write_Buf(W_TX_PAYLOAD_NOACK_CMD,temp,len);	//д�������ݰ�,�����Ӧ
	}
	NRF24L01_CE = 1; 
}

//NRF24L01��������
void NRF24L01_RecvFrame(unsigned char *temp)
{
	DYNPD_LEN=NRF24L01_Read_Reg(R_RX_PL_WID_CMD);
	if(DYNPD_LEN>32)
	{
		NRF24L01_RxMode();	//���³�ʼ������ģʽ
	}
	else
	{
		NRF24L01_Read_Buf(NRF24L01_RD_RX_PLOAD,temp,DYNPD_LEN);	//��ȡ����������
	}
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//��������ݱ�־
}
//����RF_CH
void NRF24L01_Change_Ch(unsigned char ch)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_CH,ch&0x7F);  //����RFƵ��
	RF_CH=ch&0x7F;					//ͬ�����õ�ϵͳ����
}

//NRF24L01�жϴ���
void NRF24L01_Handler(void) interrupt 11
{	
	unsigned char state,fifo_state;
	INT_CLKO &= 0xDF;		//����ⲿ�ж�
	state=NRF24L01_Read_Reg(NRF24L01_STATUS);		//��NRF24L01״̬�Ĵ���
	fifo_state=NRF24L01_Read_Reg(NRF24L01_FIFO_STATUS);		//��NRF24L01FIFO״̬�Ĵ���
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,state);//����ж�
	//���ͳɹ��ж�
	if(state & NRF24L01_STATUS_TX_DS)
	{
		P40=~P40;
		NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY��־��0
		NRF24L01_FLAG |= NRF24L01_TX_ACCESS;//���ͳɹ���־��1
		if(NRF24L01_FLAG & NRF24L01_AUTO_RX)
		{
			NRF24L01_RxMode();	//���³�ʼ������ģʽ
		}
	}
	//�ﵽ����ط������ж�
	if(state & NRF24L01_STATUS_MAX_RT)
	{
		P55=~P55;
		NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX,NRF24L01_NOP);  //���TX FIFO�Ĵ���
		NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY��־��0
		NRF24L01_FLAG &= ~NRF24L01_TX_ACCESS;//���ͳɹ���־��0
	}
	//���յ������ж�
	if(state & NRF24L01_STATUS_RX_DR)
	{
		P32=~P32;
		NRF24L01_FLAG |= NRF24L01_RECIVE;	//���յ����ݱ�־λ��1
		NRF24L01_RecvFrame(rx_buf);
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'1')	)		come=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'2')	)		wait=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'0')	)		overlook=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'h')	)		camera=1;
	}
	//TX_FIFO���ж�
	if(state & NRF24L01_STATUS_TX_FULL)
	{
		NRF24L01_TxMode();	//���³�ʼ������ģʽ
	}
	//RX_FIFO��
	if(fifo_state & NRF24L01_STATUS_RX_FULL)
	{
		NRF24L01_RxMode();	//���³�ʼ������ģʽ
	}
	INT_CLKO |= 0x20;		//�����ⲿ�ж�3
}
