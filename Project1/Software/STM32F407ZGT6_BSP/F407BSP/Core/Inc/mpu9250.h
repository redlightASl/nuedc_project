#ifndef __MPU9250_H
#define __MPU9250_H
#include <stdint.h>
#include <stdlib.h>

/* ����MPU9250�ڲ���ַ */
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

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

#define MPU_INTBP_CFG_REG		0X37	//�ж�/���IIC���üĴ���
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)

#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

#define	GYRO_ADDRESS  	0xD0	//�����ǵ�ַ
#define ACCEL_ADDRESS	0xD0  //���ٶȴ�������ַ

//MPU9250�ڲ���װ��һ��AK8963������,��ַ��ID����:
#define AK8963_ADDR		0X0C	//AK8963��I2C��ַ
#define AK8963_ID		0X48	//AK8963������ID
//AK8963���ڲ��Ĵ���
#define MAG_WIA			0x00	//AK8963������ID�Ĵ�����ַ
#define MAG_XOUT_L		0X03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


/* 
 * MPU6500/9250 ����ģ��
 * ʹ��IICЭ��
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
