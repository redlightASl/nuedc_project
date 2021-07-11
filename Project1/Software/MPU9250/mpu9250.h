#ifndef __MPU9250_H
#define __MPU9250_H
#include "sys.h"

/* 定义MPU9250内部地址 */
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG				0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)

#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x68，只读)

#define	GYRO_ADDRESS  0xD0	//陀螺仪地址
#define MAG_ADDRESS   0x18  //磁传感器地址
#define ACCEL_ADDRESS 0xD0  //加速度传感器地址

/* 
 * MPU6500/9250 六轴模块
 * 使用IIC协议
 * PE8	SCL
 * PE9	SDA
 */

/*模拟IIC端口输出输入定义*/
//SDA输入模式
#define MPU9250_SDA_IN() 		{GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=0<<9*2;}
//SDA输出模式
#define MPU9250_SDA_OUT() 	{GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=1<<9*2;}
//SDA输出配置
#define MPU9250_IIC_SDA    	PEout(9)
//SDA输入配置
#define MPU9250_READ_SDA   	PEin(9)
//SCL仅有输出
#define MPU9250_IIC_SCL    	PEout(8)

//SCL引脚 PE8
#define MPU9250_GPIO_SCL    GPIO_Pin_8
//SDA引脚 PE9
#define MPU9250_GPIO_SDA    GPIO_Pin_9
//IIC设备连接在GPIOE
#define MPU9250_IIC_PERIPH 	GPIOE

extern u8		RX_BUFFER[8]; //接收数据缓存区
extern int* X; //x轴数据
extern int* Y; //y轴数据
extern int* Z; //z轴数据
extern int* Temperature; //温度数据

void MPU9250_IIC_Init(void);
u8 MPU9250_IIC_Start(void);
void MPU9250_IIC_Stop(void);
void MPU9250_IIC_Ack(void);
void MPU9250_IIC_NAck(void);
u8 MPU9250_IIC_WaitAck(void);
void MPU9250_IIC_SendByte(u8 SendByte);
u8 MPU9250_IIC_RadeByte(void);
u8 MPU9250_SingleWrite(u8 SlaveAddress, u8 RegisterAddress, u8 RegisterData);
u8 MPU9250_SingleRead(u8 SlaveAddress, u8 RegisterAddress);
void MPU9250_Init(void);
void MPU9250_ReadAccelerateData(void);
void MPU9250_CalculateAccelerateData(void);
void MPU9250_ReadGYROData(void);
void MPU9250_CalculateGYROData(void);
void MPU9250_TurnOnMagnetic(void);
void MPU9250_ReadMagneticData(void);
void MPU9250_CalculateMagneticData(void);
void MPU9250_CalculateTemperatureData(void);
void MPU9250_Redirect(int* x, int* y, int* z, int* temperature);

#endif
