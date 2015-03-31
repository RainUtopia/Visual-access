#include <stc\stc15f2k60s2.h>
#include "12864.h"
#include "DS1302.h"
#include "delay.h"
#include "menu.h"
#include	"net_camera.h"
#include	"NRF24L01.H"

unsigned char key;

unsigned char keyscan()
{
	unsigned char	temp=0;
	P3 = P3	|	0x78;
	delay_us(1);
	if(P3	!=	0x78)
	{
		delay_ms(10);
		if(P3	!=	0x78)
		{
			if(P33	== 0)				while(0x78!=(P3&0x78))	temp=K1;
			else	if(P34	== 0)	while(0x78!=(P3&0x78))	temp=K2;
			else	if(P35	== 0)	while(0x78!=(P3&0x78))	temp=K3;
			else	if(P36	== 0)	while(0x78!=(P3&0x78))	temp=K4;
		}
	}
	return	temp;
}

struct 	MenuItem code	Find[7];

void	NullSubs(void)	{}
//Ŀ¼�ṹ�嶨��
struct 	MenuItem                           //�ṹ������
{ 		unsigned char 	MenuCount;         //��ǰ��ڵ�����ÿ��˵�����ʾ����Ŀ��
    	unsigned char 	*DisplayString;    //ָ��˵�������ַ���
    	unsigned char 	(*Subs)();         //ָ��ǰ״̬Ӧ��ִ�еĹ��ܺ���
    	struct MenuItem 	*ChildrenMenus;//ָ��ǰ�˵����¼��˵�
    	struct MenuItem 	*ParentMenus;  //ָ��ǰ�˵����ϼ��˵�
}		Null;



unsigned char code	office[7]=	//�칫�ҳ�ԱƵ��
{64,65,66,67,68,69,70};

struct	MenuItem code 	MainMenu[3] =      //�ṹ�����˵�
{
		{3,"  ����  ",&NullSubs,&Find, &Null},	
		{3,"����ʱ��",&time_change,&Null, &Null},	
		{3,"��������",&date_change,&Null, &Null},	
//		{5,"���ڴ�ͼ",&Nrf24L01_Camera_Test,&Null, &Null},
//		{5,"���ߴ�ͼ",&Nrf24L01_Camera,&Null,&Null},	
};

struct	MenuItem  code	Find[7] = 
{
{7,"������",&person_ui,&Null,&MainMenu},
{7,"����Ƽ",&person_ui,&Null,&MainMenu},
{7,"����",&person_ui,&Null,&MainMenu},
{7,"ţ����",&person_ui,&Null,&MainMenu},
{7,"�Ͻ�",&person_ui,&Null,&MainMenu},
{7,"��ʤ��",&person_ui,&Null,&MainMenu},
{7,"������",&person_ui,&Null,&MainMenu},
};
void person_ui(void)
{
	unsigned char	rx_temp[32];

	lcd_msg("����");
	lcd_disp_string(MenuPoint[UserChoose].DisplayString);
	lcd_set_xy(2,3);
	lcd_disp_number(office[UserChoose]/100);
	lcd_disp_number(office[UserChoose]%100/10);
	lcd_disp_number(office[UserChoose]%10);
	lcd_disp_string("CH");
	NRF24L01_Change_Ch(office[UserChoose]);
	rx_temp[0]='h';
	NRF24L01_SendFrame(&rx_temp,1);
	delay_ms(500);
		if(camera)	{	Nrf24L01_Camera();	camera=0;}
		else	{	lcd_msg("����ʧ��");	lcd_set_xy(2,0);	lcd_disp_string("����������ʽ��ϵ");	delay_ms(3000);}

}
struct 	MenuItem (*MenuPoint) = MainMenu; 
signed char		DisplayStart  = 0; 			       //��ʾ�˵��Ŀ�ʼλ��   
signed char 	UserChoose    = 0; 				   //�û���ѡ��
signed char		DisplayStart_Old  = 0; 			 //����˵��Ŀ�ʼλ��  
signed char 	UserChoose_Old		  = 0;     //�����û��Ĳ˵����
signed char 	MaxItems      = 0;					//�˵�������
#define		LCD_MAX_DISP_LINES		4						//��Ļ�����ʾ��Ŀ��
//��ʾ����
void	ShowMenu(void)
{
		unsigned char 	DisplayPoint  = 0; 				   //��ʾ�˵��ı��
		unsigned char i = 0;
		clr_Scr();			//����
		MaxItems = MenuPoint[0].MenuCount;        //��ȡ��ǰ�˵�����Ŀ��
		DisplayPoint = DisplayStart;
		if(MaxItems >= LCD_MAX_DISP_LINES)
		{
			for (i = 0;i < LCD_MAX_DISP_LINES;i++)
			{
				  lcd_set_xy(i,1);
					if ((DisplayPoint) == (UserChoose))
						{
							lcd_disp_string("\XA1\XFA");	  //дС��ͷ
						}
					else
						{
							lcd_disp_string("  ");    //д�ո�
						}
						lcd_disp_string(MenuPoint[DisplayPoint].DisplayString);
					DisplayPoint += 1;	//�ַ���ƫ����+1
					if ((DisplayPoint) == (MaxItems))
						{
							DisplayPoint = 0;
						}
			}
		}
		else
		{
			for (i = 0;i < MaxItems;i++)
			{
				  lcd_set_xy(i,1);
					if ((DisplayPoint) == (UserChoose))
						{
							lcd_disp_string("\XA1\XFA");	  //дС��ͷ
						}
					else
						{
							lcd_disp_string("  ");    //д�ո�
						}
						lcd_disp_string(MenuPoint[DisplayPoint].DisplayString);
					DisplayPoint += 1;	//�ַ���ƫ����+1
					if ((DisplayPoint) == (MaxItems))
						{
							DisplayPoint = 0;
						}
		 }
	 }

}

//--------------------------------------------------
// [����˵��] *
// ----------------------------------- *
// [K1] KEY_ENTER(ȷ��) *
// [K2] KEY_DOWN(����) 	*
// [K3] KEY_UP(����) 		*
// [K4] KEY_ESC(����) 	*
//---------------------------------------------------/
void Key_Menu(unsigned char KeyNum)
{
	if (KeyNum != 0)
	{
		MaxItems = MenuPoint[0].MenuCount;        //��ȡ��ǰ�˵�����Ŀ��
		switch (KeyNum)
		{
		case K2:
			UserChoose --;	
			if (UserChoose < 0)
			{
				UserChoose = MaxItems-1;
			}
			break;
		case K4:
			if (MenuPoint[0].ParentMenus != &Null)
			{
				MenuPoint = MenuPoint[0].ParentMenus;
				//�ָ���֮ǰ��ѡ��˵�
				UserChoose = UserChoose_Old;
				DisplayStart = DisplayStart_Old;
			}
			break;
		case K3:
			UserChoose ++;
			if (UserChoose >= MaxItems)
			{
				UserChoose = 0;
			}
			break;
		case K1:
			if (MenuPoint[UserChoose].Subs != NullSubs)	//�������������Ϊ�գ����в�������
			{
				(*MenuPoint[UserChoose].Subs)();
			}
			if (MenuPoint[UserChoose].ChildrenMenus != &Null)	//�������������Ϊ�ղ����Ӳ˵���Ϊ�գ�����в˵��л�
			{
					MenuPoint = MenuPoint[UserChoose].ChildrenMenus;
					DisplayStart_Old = DisplayStart;
					UserChoose_Old = UserChoose;
					UserChoose = 0;
					DisplayStart = 0;
			}
			break;
			default:	break;
		}

		if ( (UserChoose > (DisplayStart+(LCD_MAX_DISP_LINES-1))) )
		{//�˵���ҳ
			DisplayStart = UserChoose;
		}
		if( ((UserChoose + MaxItems - DisplayStart) > (LCD_MAX_DISP_LINES-1)) && (DisplayStart > UserChoose) )
		{//�˵���ҳ
			DisplayStart = UserChoose;
		}
		ShowMenu();
	}
}
//�˵����
void gui(void)
{
	ShowMenu();
		while(1)
		{
		key=keyscan();
		if (MenuPoint[0].ParentMenus == &Null)
			if(key==K4)	break;		//��ǰΪ���˵��Ұ��·��ؼ�ʱ�˳��˵�
		
	if(come)	{lcd_msg("  ���! ");	come=0;	delay_ms(3000);	key=0xff;}
	if(wait)	{lcd_msg("  ���Ե�");	wait=0;	delay_ms(3000);	key=0xff;}
	if(overlook)	{lcd_msg("�Է�æ��");	overlook=0;	delay_ms(3000);	key=0xff;}
		Key_Menu(key);
	 
		}
}
void time_view(signed char point)
{
	clr_Scr();			//����
	lcd_set_xy(0,2);
	lcd_disp_string("ʱ������");
	lcd_set_xy(2,1);
	lcd_disp_number(now[2]/10);
	lcd_disp_number(now[2]%10);
	lcd_disp_string(": ");
	lcd_disp_number(now[1]/10);
	lcd_disp_number(now[1]%10);
	lcd_disp_string(": ");
	lcd_disp_number(now[0]/10);
	lcd_disp_number(now[0]%10);
	if(point==0)	lcd_set_xy(2,1);
	else	if(point==1)	lcd_set_xy(2,3);
}
//ʱ����ں���
void time_change(void)
{
	signed char point=0,key=0;
	DS1302_GetTime(now);
	now[0]=0;		//������
	write_com(0x0f);	//���α���ʾ
	now[1]=(now[1]/16*10+now[1]%16);
	now[2]=(now[2]/16*10+now[2]%16);
	time_view(point);
	while(1)
	{
	key=keyscan();
	if(K1==key)	
		{
			point++;
			if(point==2)	
			{
				write_com(0x0c);
				now[1]=(now[1]/10*16+now[1]%10);
				now[2]=(now[2]/10*16+now[2]%10);
				DS1302_SetTime(now);
				break;
			}
		}
	if(K2==key)	
		{
			if(point==0)	now[2]++;
			if(point==1)	now[1]++;
		}
	if(K3==key)	
		{
			if(point==0)	now[2]--;
			if(point==1)	now[1]--;
		}
	if(K4==key)	
		{
			write_com(0x0c);
			break;
		}
	if(now[1]>59)	now[1]=0;
	if(now[1]<0)	now[1]=59;
	if(now[2]>23)	now[2]=0;
	if(now[2]<0)	now[2]=23;
	if(key!=0)	time_view(point);
	}
}

void date_view(signed char point)
{
	clr_Scr();			//����
	lcd_set_xy(0,2);
	lcd_disp_string("��������");
	lcd_set_xy(2,1);
	lcd_disp_number(2);
	lcd_disp_number(0);
	lcd_disp_number(now[6]/10);
	lcd_disp_number(now[6]%10);
	lcd_disp_string("��");
	lcd_disp_number(now[4]/10);
	lcd_disp_number(now[4]%10);
	lcd_disp_string("��");
	lcd_disp_number(now[3]/10);
	lcd_disp_number(now[3]%10);
	if(point==0)	lcd_set_xy(2,2);
	else	if(point==1)	lcd_set_xy(2,4);
	else	if(point==2)	lcd_set_xy(2,6);
}
//���ڵ��ں���
void date_change(void)
{
	signed char point=0,key=0;
	signed char temp[7];
	DS1302_GetTime(now);
	write_com(0x0f);	//���α���ʾ
	now[3]=(now[3]/16*10+now[3]%16);
	now[4]=(now[4]/16*10+now[4]%16);
	now[6]=(now[6]/16*10+now[6]%16);
	date_view(point);
	while(1)
	{
	key=keyscan();
	if(K1==key)	
		{
			point++;
			if(point==3)	
			{
				write_com(0x0c);
				 if (now[4]==1 || now[4]==2)
					 {
						 now[4]+=12;
						 now[6]--;
					}
				now[5]=(now[3]+1+2*now[4] + 3*(now[4]+1)/5 + now[6] + now[6]/4 - now[6]/100 + now[6]/400)%7;
				now[3]=(now[3]/10*16+now[3]%10);
				now[4]=(now[4]/10*16+now[4]%10);
				now[6]=(now[6]/10*16+now[6]%10);
				now[5]=(now[5]/10*16+now[5]%10);
				DS1302_GetTime(temp);
				temp[3]=now[3];
				temp[4]=now[4];
				temp[5]=now[5];
				temp[6]=now[6];
				DS1302_SetTime(temp);
				break;
			}
		}
	if(K2==key)	
		{
			if(point==0)	now[6]++;
			if(point==1)	now[4]++;
			if(point==2)	now[3]++;
		}
	if(K3==key)	
		{
			if(point==0)	now[6]--;
			if(point==1)	now[4]--;
			if(point==2)	now[3]--;
		}
	if(K4==key)	
		{
			write_com(0x0c);
			break;
		}
	if(now[6]>99)	now[6]=0;
	if(now[6]<0)	now[6]=99;
	if(now[4]>12)	now[4]=1;
	if(now[4]<1)	now[4]=12;
	if(now[3]>31)	now[3]=1;
	if(now[3]<1)	now[3]=31;
	if(key!=0)	date_view(point);
	}
}


