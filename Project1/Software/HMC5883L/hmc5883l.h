#ifndef __HMC5883_H
#define __HMC5883_H
#include "sys.h"

/*
 * 电子罗盘hmc5883l模块
 * 使用IIC总线协议
 * PD3	SCL
 * PD4  SDA
 */

//SDA输入模式
#define HMC5883L_SDA_IN() 		{GPIOD->MODER&=~(3<<(4*2));GPIOD->MODER|=0<<4*2;}
//SDA输出模式
#define HMC5883L_SDA_OUT() 	{GPIOD->MODER&=~(3<<(4*2));GPIOD->MODER|=1<<4*2;}
//SDA输出配置
#define HMC5883L_IIC_SDA    	PDout(4)
//SDA输入配置
#define HMC5883L_READ_SDA   	PDin(4)
//SCL仅有输出
#define HMC5883L_IIC_SCL    	PDout(3)

//SCL引脚 PD3
#define HMC5883L_GPIO_SCL    GPIO_Pin_3
//SDA引脚 PD4
#define HMC5883L_GPIO_SDA    GPIO_Pin_4
//IIC设备连接在GPIOD
#define HMC5883L_IIC_PERIPH 	GPIOD

//传感器控制位地址
#define HMC5883L_Write_Address 0x3C
#define HMC5883L_Read_Address (0x3C + 1)

//数学计算用数
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

