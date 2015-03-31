#ifndef __12864_H_
#define __12864_H_

#define CS		P14    //Ƭѡ�ź�
#define SID 	P13		//�����ź�
#define SCLK	P12		//ʱ���ź� 
#define BK		P47		//����

void sendbyte(unsigned char zdata);					//����Һ���Ĵ���ͨ��Э�飬��������
void write_com(unsigned char cmdcode);			//д����ָ��
void write_data(unsigned char Dispdata);		//д��������
void clr_Scr(void);													//��������
void lcdinit();															//lcd��ʼ��
void lcd_set_xy( unsigned char x, unsigned char y );	//������ʼ��ַ����,x��ʾ��0-3,y��ʾ��0-7
void lcd_disp_string(unsigned char *str); //��X,Y���꿪ʼд���ַ���
void lcd_disp_number(unsigned char s);			//������ʾ����
void	lcd_msg(unsigned char *s);						//��ʾ��Ϣ��
void	lcd_disp_week(unsigned char s);				//������ʾ
void find_person(void);											//�����ӳ���

#endif