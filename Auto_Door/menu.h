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
extern	unsigned char 	 code	office[7];	//�칫�ҳ�Ա
extern	struct 	MenuItem (*MenuPoint); 
extern	signed char		DisplayStart; 			       //��ʾ�˵��Ŀ�ʼλ��   
extern	signed char 	UserChoose; 				   //�û���ѡ��
extern	signed char		DisplayStart_Old; 			 //����˵��Ŀ�ʼλ��  
extern	signed char 	UserChoose_Old;     //�����û��Ĳ˵����
extern	signed char 	MaxItems;					//�˵�������
extern	unsigned char key;							//����ֵ

unsigned char keyscan(void);	//����ɨ�躯��
void	NullSubs(void);
void	ShowMenu(void);
void Key_Menu(unsigned char KeyNum);
void gui(void);
void time_change(void);	//ʱ����ں���
void time_view(signed char point);
void date_change(void);	//���ڵ��ں���
void date_view(signed char point);
void person_ui(void);
#endif