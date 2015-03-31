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
//目录结构体定义
struct 	MenuItem                           //结构体名称
{ 		unsigned char 	MenuCount;         //当前层节点数、每层菜单能显示的条目数
    	unsigned char 	*DisplayString;    //指向菜单标题的字符串
    	unsigned char 	(*Subs)();         //指向当前状态应该执行的功能函数
    	struct MenuItem 	*ChildrenMenus;//指向当前菜单的下级菜单
    	struct MenuItem 	*ParentMenus;  //指向当前菜单的上级菜单
}		Null;



unsigned char code	office[7]=	//办公室成员频道
{64,65,66,67,68,69,70};

struct	MenuItem code 	MainMenu[3] =      //结构体主菜单
{
		{3,"  找人  ",&NullSubs,&Find, &Null},	
		{3,"调整时间",&time_change,&Null, &Null},	
		{3,"调整日期",&date_change,&Null, &Null},	
//		{5,"串口传图",&Nrf24L01_Camera_Test,&Null, &Null},
//		{5,"无线传图",&Nrf24L01_Camera,&Null,&Null},	
};

struct	MenuItem  code	Find[7] = 
{
{7,"王清珍",&person_ui,&Null,&MainMenu},
{7,"王玉萍",&person_ui,&Null,&MainMenu},
{7,"曾毅",&person_ui,&Null,&MainMenu},
{7,"牛庆丽",&person_ui,&Null,&MainMenu},
{7,"鞠杰",&person_ui,&Null,&MainMenu},
{7,"李胜辉",&person_ui,&Null,&MainMenu},
{7,"黄于欣",&person_ui,&Null,&MainMenu},
};
void person_ui(void)
{
	unsigned char	rx_temp[32];

	lcd_msg("连接");
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
		else	{	lcd_msg("连接失败");	lcd_set_xy(2,0);	lcd_disp_string("请以其他方式联系");	delay_ms(3000);}

}
struct 	MenuItem (*MenuPoint) = MainMenu; 
signed char		DisplayStart  = 0; 			       //显示菜单的开始位置   
signed char 	UserChoose    = 0; 				   //用户的选择
signed char		DisplayStart_Old  = 0; 			 //保存菜单的开始位置  
signed char 	UserChoose_Old		  = 0;     //保存用户的菜单编号
signed char 	MaxItems      = 0;					//菜单的数量
#define		LCD_MAX_DISP_LINES		4						//屏幕最多显示条目数
//显示函数
void	ShowMenu(void)
{
		unsigned char 	DisplayPoint  = 0; 				   //显示菜单的编号
		unsigned char i = 0;
		clr_Scr();			//清屏
		MaxItems = MenuPoint[0].MenuCount;        //获取当前菜单的条目数
		DisplayPoint = DisplayStart;
		if(MaxItems >= LCD_MAX_DISP_LINES)
		{
			for (i = 0;i < LCD_MAX_DISP_LINES;i++)
			{
				  lcd_set_xy(i,1);
					if ((DisplayPoint) == (UserChoose))
						{
							lcd_disp_string("\XA1\XFA");	  //写小箭头
						}
					else
						{
							lcd_disp_string("  ");    //写空格
						}
						lcd_disp_string(MenuPoint[DisplayPoint].DisplayString);
					DisplayPoint += 1;	//字符串偏移量+1
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
							lcd_disp_string("\XA1\XFA");	  //写小箭头
						}
					else
						{
							lcd_disp_string("  ");    //写空格
						}
						lcd_disp_string(MenuPoint[DisplayPoint].DisplayString);
					DisplayPoint += 1;	//字符串偏移量+1
					if ((DisplayPoint) == (MaxItems))
						{
							DisplayPoint = 0;
						}
		 }
	 }

}

//--------------------------------------------------
// [按键说明] *
// ----------------------------------- *
// [K1] KEY_ENTER(确定) *
// [K2] KEY_DOWN(向上) 	*
// [K3] KEY_UP(向下) 		*
// [K4] KEY_ESC(后退) 	*
//---------------------------------------------------/
void Key_Menu(unsigned char KeyNum)
{
	if (KeyNum != 0)
	{
		MaxItems = MenuPoint[0].MenuCount;        //获取当前菜单的条目数
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
				//恢复到之前的选择菜单
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
			if (MenuPoint[UserChoose].Subs != NullSubs)	//如果操作函数不为空，运行操作函数
			{
				(*MenuPoint[UserChoose].Subs)();
			}
			if (MenuPoint[UserChoose].ChildrenMenus != &Null)	//如果操作函数不为空并且子菜单不为空，则进行菜单切换
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
		{//菜单翻页
			DisplayStart = UserChoose;
		}
		if( ((UserChoose + MaxItems - DisplayStart) > (LCD_MAX_DISP_LINES-1)) && (DisplayStart > UserChoose) )
		{//菜单翻页
			DisplayStart = UserChoose;
		}
		ShowMenu();
	}
}
//菜单入口
void gui(void)
{
	ShowMenu();
		while(1)
		{
		key=keyscan();
		if (MenuPoint[0].ParentMenus == &Null)
			if(key==K4)	break;		//当前为主菜单且按下返回键时退出菜单
		
	if(come)	{lcd_msg("  请进! ");	come=0;	delay_ms(3000);	key=0xff;}
	if(wait)	{lcd_msg("  请稍等");	wait=0;	delay_ms(3000);	key=0xff;}
	if(overlook)	{lcd_msg("对方忙绿");	overlook=0;	delay_ms(3000);	key=0xff;}
		Key_Menu(key);
	 
		}
}
void time_view(signed char point)
{
	clr_Scr();			//清屏
	lcd_set_xy(0,2);
	lcd_disp_string("时间设置");
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
//时间调节函数
void time_change(void)
{
	signed char point=0,key=0;
	DS1302_GetTime(now);
	now[0]=0;		//秒清零
	write_com(0x0f);	//开游标显示
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
	clr_Scr();			//清屏
	lcd_set_xy(0,2);
	lcd_disp_string("日期设置");
	lcd_set_xy(2,1);
	lcd_disp_number(2);
	lcd_disp_number(0);
	lcd_disp_number(now[6]/10);
	lcd_disp_number(now[6]%10);
	lcd_disp_string("年");
	lcd_disp_number(now[4]/10);
	lcd_disp_number(now[4]%10);
	lcd_disp_string("月");
	lcd_disp_number(now[3]/10);
	lcd_disp_number(now[3]%10);
	if(point==0)	lcd_set_xy(2,2);
	else	if(point==1)	lcd_set_xy(2,4);
	else	if(point==2)	lcd_set_xy(2,6);
}
//日期调节函数
void date_change(void)
{
	signed char point=0,key=0;
	signed char temp[7];
	DS1302_GetTime(now);
	write_com(0x0f);	//开游标显示
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


