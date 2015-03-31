/****************************************
NRF24L01库函数
STC15F2K61S2已测试OK
支持24L01+和BK2423
支持ACK和NOACK
支持发送完自动切换到接收模式
中断处理消息
制作人:Fancy 
QQ:652774317
******************************************/

#include	<STC/STC15F2K60S2.H>
#include	"NRF24L01.H"


unsigned char RF_CH=0x40;		//RF频道
unsigned char NRF24L01_FLAG=0;	//NRF24L01当前状态标志
																//DB7; 1接收模式 			0发射模式
																//DB6; 1有任务   			0空闲
																//DB5; 1发送成功 			0发送失败(配合DB6检测是否发送成功)
																//DB4; 1NRF正常连接 	0 检测不到连接
																//DB3; 1接收到数据包 	0 缓冲区无数据
																//DB2; 1 ACK模式      0 NOACK模式
																//DB1; 1发送完自动进入接收模式 0关闭自动切换模式
unsigned char DYNPD_LEN=0;			//用于存放动态数据包长度
unsigned char TX_ADDRESS[NRF24L01_ADR_LEN]= {0x12,0x34,0x56,0x78,0x90};	//本地地址
unsigned char RX_ADDRESS[NRF24L01_ADR_LEN]= {0x12,0x34,0x56,0x78,0x90};	//接收地址
unsigned char	rx_buf[32]={0};
bit	camera=0;
bit	come=0;
bit	wait=0;
bit	overlook=0;

//向从机发送一个字节数据并返回接收数据
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

//写NRF24L01寄存器，返回状态值
unsigned char NRF24L01_Write_Reg(unsigned char reg, unsigned char value)
{
	 unsigned char BackDate;
   SPI_SS=0;
   SPI_WriteRead(reg);
   BackDate=SPI_WriteRead(value);
   SPI_SS=1;
   return(BackDate);
}
//读NRF24L01寄存器，返回寄存器值
unsigned char NRF24L01_Read_Reg(unsigned char reg)
{
	 unsigned char BackDate;
   SPI_SS=0;
   SPI_WriteRead(reg);
   BackDate=SPI_WriteRead(0x00);
   SPI_SS=1;
   return(BackDate);
}
//读取缓冲区数据
void NRF24L01_Read_Buf(unsigned char reg, unsigned char *pBuf,unsigned char bytes)
{
	unsigned char byte_ctr;
	SPI_SS = 0;
	SPI_WriteRead(reg);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
	pBuf[byte_ctr] = SPI_WriteRead(0x00);
	SPI_SS = 1;
}
//向缓冲区写入数据
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

//NRF24L01初始化函数
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
  //配置寄存器
	NRF24L01_FLAG |= NRF24L01_AUTO_ACK;		//是否开启应答模式
	NRF24L01_FLAG |= NRF24L01_AUTO_RX;		//发送完是否自动进入接收模式
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_SETUP_AW, NRF24L01_ADR_LEN-2);	//设置地址长度为 NRF24L01_ADR_LEN
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_CH,RF_CH);                  //设置RF通道为RFCH
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_SETUP, 0x27);               //设置TX发射参数,7db增益,250kbps,低噪声增益开启
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_EN_AA, 0x01);         					//使能通道0的自动应答
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_EN_RXADDR, 0x01);              //使能通道0的接收地址
  //RX模式配置
  NRF24L01_Write_Buf(NRF24L01_WRITE_REG | NRF24L01_RX_ADDR_P0,RX_ADDRESS,NRF24L01_ADR_LEN); //写RX节点地址
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FEATURE,0x05);									//使能动态数据包长度,AUTO_ACK模式
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_DYNPD,0X01);        						//选择通道0动态数据包长度
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_RX,NRF24L01_NOP);                //清除RX FIFO寄存器
  //TX模式配置
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG | NRF24L01_TX_ADDR ,TX_ADDRESS,NRF24L01_ADR_LEN);	//写TX节点地址
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_SETUP_RETR, 0x3A);           //设置自动重发间隔时间:750us + 86us;最大自动重发次数:5次
  NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX, NRF24L01_NOP);              //清除TX FIFO寄存器
  NRF24L01_RxMode();                                                  					//默认进入接收模式
  NRF24L01_CE=1;
	INT_CLKO |= 0x20;		//开启外部中断3
	EA=1;
}

//NRF24L01进入发送模式
void NRF24L01_TxMode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_CONFIG, 0x0e);
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,0x70);		//清除中断标志
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX,NRF24L01_NOP);  //清除TX FIFO寄存器
	NRF24L01_CE = 1; 
	NRF24L01_FLAG &= ~NRF24L01_MODE_RX;		//修改状态标识为发射模式
	NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY标志置0
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//清除有数据标志
}
//NRF24L01进入接收模式
void NRF24L01_RxMode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_CONFIG, 0x0f); 	//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,0x70);		//清除中断标志
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_RX,NRF24L01_NOP);  //清除RX FIFO寄存器
	NRF24L01_CE = 1; 
	NRF24L01_FLAG |= NRF24L01_MODE_RX;		//修改状态标识为接收模式
	NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY标志置0
	DYNPD_LEN=0;	//清0动态数据包长度
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//清除有数据标志
}

//NRF24L01 SPI通信检查
void NRF24L01_Check(void)
{
	unsigned char i=0;
	unsigned char Buff[5];
	NRF24L01_Read_Buf(NRF24L01_TX_ADDR,Buff,NRF24L01_ADR_LEN);
	i=NRF24L01_Read_Reg(NRF24L01_DYNPD);	//读取DYNPD监测是否正确写入
	if(i==0x01)		//这里设置的是0x01,要与初始化的值相对应
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

//NRF24L01发送数据
void NRF24L01_SendFrame(unsigned char *temp,unsigned char len)
{
	if(NRF24L01_FLAG & NRF24L01_MODE_RX)
	{
		NRF24L01_TxMode();
	}
	NRF24L01_FLAG |= NRF24L01_BUSY;	//BUSY标志置1
	NRF24L01_FLAG &= ~NRF24L01_TX_ACCESS;//清除发送成功标志
	NRF24L01_CE = 0;
	if(NRF24L01_FLAG & NRF24L01_AUTO_ACK)
		{
			NRF24L01_Write_Buf(NRF24L01_WR_TX_PLOAD,temp,len);	//写待发数据包,需要回应
		}
	else
	{
		NRF24L01_Write_Buf(W_TX_PAYLOAD_NOACK_CMD,temp,len);	//写待发数据包,无须回应
	}
	NRF24L01_CE = 1; 
}

//NRF24L01接收数据
void NRF24L01_RecvFrame(unsigned char *temp)
{
	DYNPD_LEN=NRF24L01_Read_Reg(R_RX_PL_WID_CMD);
	if(DYNPD_LEN>32)
	{
		NRF24L01_RxMode();	//重新初始化接收模式
	}
	else
	{
		NRF24L01_Read_Buf(NRF24L01_RD_RX_PLOAD,temp,DYNPD_LEN);	//读取缓冲区数据
	}
	NRF24L01_FLAG &= ~NRF24L01_RECIVE;//清除有数据标志
}
//更改RF_CH
void NRF24L01_Change_Ch(unsigned char ch)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_RF_CH,ch&0x7F);  //设置RF频道
	RF_CH=ch&0x7F;					//同步设置到系统参数
}

//NRF24L01中断处理
void NRF24L01_Handler(void) interrupt 11
{	
	unsigned char state,fifo_state;
	INT_CLKO &= 0xDF;		//清除外部中断
	state=NRF24L01_Read_Reg(NRF24L01_STATUS);		//读NRF24L01状态寄存器
	fifo_state=NRF24L01_Read_Reg(NRF24L01_FIFO_STATUS);		//读NRF24L01FIFO状态寄存器
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_STATUS,state);//清除中断
	//发送成功中断
	if(state & NRF24L01_STATUS_TX_DS)
	{
		P40=~P40;
		NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY标志置0
		NRF24L01_FLAG |= NRF24L01_TX_ACCESS;//发送成功标志置1
		if(NRF24L01_FLAG & NRF24L01_AUTO_RX)
		{
			NRF24L01_RxMode();	//重新初始化接收模式
		}
	}
	//达到最大重发次数中断
	if(state & NRF24L01_STATUS_MAX_RT)
	{
		P55=~P55;
		NRF24L01_Write_Reg(NRF24L01_WRITE_REG | NRF24L01_FLUSE_TX,NRF24L01_NOP);  //清除TX FIFO寄存器
		NRF24L01_FLAG &= ~NRF24L01_BUSY;	//BUSY标志置0
		NRF24L01_FLAG &= ~NRF24L01_TX_ACCESS;//发送成功标志置0
	}
	//接收到数据中断
	if(state & NRF24L01_STATUS_RX_DR)
	{
		P32=~P32;
		NRF24L01_FLAG |= NRF24L01_RECIVE;	//接收到数据标志位置1
		NRF24L01_RecvFrame(rx_buf);
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'1')	)		come=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'2')	)		wait=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'0')	)		overlook=1;
		if(	(DYNPD_LEN	==	1)	&&	(rx_buf[0]	==	'h')	)		camera=1;
	}
	//TX_FIFO满中断
	if(state & NRF24L01_STATUS_TX_FULL)
	{
		NRF24L01_TxMode();	//重新初始化发射模式
	}
	//RX_FIFO满
	if(fifo_state & NRF24L01_STATUS_RX_FULL)
	{
		NRF24L01_RxMode();	//重新初始化接收模式
	}
	INT_CLKO |= 0x20;		//开启外部中断3
}
