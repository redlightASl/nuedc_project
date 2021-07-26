#ifndef __MPU9250_H
#define __MPU9250_H
#include <stdint.h>
#include <stdlib.h>

/* 定义MPU9250内部地址 */
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
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

#define MPU_INTBP_CFG_REG		0X37	//中断/外挂IIC设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)

#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

#define	GYRO_ADDRESS  	0xD0	//陀螺仪地址
#define ACCEL_ADDRESS	0xD0  //加速度传感器地址

//MPU9250内部封装了一个AK8963磁力计,地址和ID如下:
#define AK8963_ADDR		0X0C	//AK8963的I2C地址
#define AK8963_ID		0X48	//AK8963的器件ID
//AK8963的内部寄存器
#define MAG_WIA			0x00	//AK8963的器件ID寄存器地址
#define MAG_XOUT_L		0X03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


/* 
 * MPU6500/9250 六轴模块
 * 使用IIC协议
 * PF1	SCL
 * PF0	SDA
 */

uint8_t HAL_IIC1_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t HAL_IIC1_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);
uint8_t HAL_IIC1_ReadMultiByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t HAL_IIC1_WriteMultiByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t MPU9250_SingleWrite(uint8_t reg, uint8_t data);
uint8_t MPU9250_SingleRead(uint8_t reg, uint8_t *buf);
uint8_t MPU9250_MultiWrite(uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t MPU9250_MultiRead(uint8_t reg, uint8_t len, uint8_t *buf);
void MPU9250_Init(void);
void MPU9250_ReadAccelerateData(void);
void MPU9250_ReadGYROData(void);
void MPU9250_ReadMagneticData(void);
void MPU9250_CalculateTemperatureData(float *tempdata);
void MPU9250_Output(int16_t* buffer);
	
#endif
