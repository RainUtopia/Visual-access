#ifndef __MENU_H__
#define __MENU_H__


#define	K1 (1<<3)
#define	K2 (1<<4)
#define	K3 (1<<5)
#define	K4 (1<<6)
#define	person_number	6;

extern	struct 	MenuItem;
extern	struct 	MenuItem code 	find[7];
extern	struct	MenuItem code 	MainMenu[3];
extern	unsigned char 	 code	office[7];	//办公室成员
extern	struct 	MenuItem (*MenuPoint); 
extern	signed char		DisplayStart; 			       //显示菜单的开始位置   
extern	signed char 	UserChoose; 				   //用户的选择
extern	signed char		DisplayStart_Old; 			 //保存菜单的开始位置  
extern	signed char 	UserChoose_Old;     //保存用户的菜单编号
extern	signed char 	MaxItems;					//菜单的数量
extern	unsigned char key;							//按键值

unsigned char keyscan(void);	//按键扫描函数
void	NullSubs(void);
void	ShowMenu(void);
void Key_Menu(unsigned char KeyNum);
void gui(void);
void time_change(void);	//时间调节函数
void time_view(signed char point);
void date_change(void);	//日期调节函数
void date_view(signed char point);
void person_ui(void);
#endif