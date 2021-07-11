#ifndef __HMC5883_H
#define __HMC5883_H
#include "sys.h"

/*
 * ��������hmc5883lģ��
 * ʹ��IIC����Э��
 * PD3	SCL
 * PD4  SDA
 */

//SDA����ģʽ
#define HMC5883L_SDA_IN() 		{GPIOD->MODER&=~(3<<(4*2));GPIOD->MODER|=0<<4*2;}
//SDA���ģʽ
#define HMC5883L_SDA_OUT() 	{GPIOD->MODER&=~(3<<(4*2));GPIOD->MODER|=1<<4*2;}
//SDA�������
#define HMC5883L_IIC_SDA    	PDout(4)
//SDA��������
#define HMC5883L_READ_SDA   	PDin(4)
//SCL�������
#define HMC5883L_IIC_SCL    	PDout(3)

//SCL���� PD3
#define HMC5883L_GPIO_SCL    GPIO_Pin_3
//SDA���� PD4
#define HMC5883L_GPIO_SDA    GPIO_Pin_4
//IIC�豸������GPIOD
#define HMC5883L_IIC_PERIPH 	GPIOD

//����������λ��ַ
#define HMC5883L_Write_Address 0x3C
#define HMC5883L_Read_Address (0x3C + 1)

//��ѧ��������
#define PI 3.14159265

void HMC5883_IIC_Init(void);
void HMC5883_IIC_Start(void);
void HMC5883_IIC_Stop(void);
u8 HMC5883_IIC_WaitAck(void);
void HMC5883_IIC_Ack(void);
void HMC5883_IIC_NAck(void);
void HMC5883_IIC_SendByte(u8 txd);
u8 HMC5883_IIC_ReadByte(void);
void HMC5883L_Init(void);
void HMC5883L_CalculateCurrentMag_xyz(int* mag_x, int* mag_y, int* mag_z);
float HMC5883L_CalculateCurrentAngle(void);

#endif

