#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0    PEin(3)   	//PE3
#define WK_UP 	PAin(0)		//PA0
*/

#define KEY0_PRES 	1
#define WKUP_PRES   2

void KEY_Init(void); //IO��ʼ��
u8 KEY_Scan(u8); //����ɨ�躯��
#endif
