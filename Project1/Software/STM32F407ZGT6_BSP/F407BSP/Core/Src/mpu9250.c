#include "mpu9250.h"
#include "i2c.h"
#include "usart.h"
#include <stdio.h>
#include <math.h>

int16_t accData[3];
int16_t gyroData[3];
int16_t magData[3];

/* IIC�ײ����� */
uint8_t HAL_IIC1_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf)
{
	return HAL_I2C_Mem_Read(&hi2c2, I2C_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, 1, 100);
}

uint8_t HAL_IIC1_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data)
{
	uint8_t *pData;
	pData = &data;
	return HAL_I2C_Mem_Write(&hi2c2, I2C_Addr, reg, I2C_MEMADD_SIZE_8BIT, pData, 1, 100);
}
 
uint8_t HAL_IIC1_ReadMultiByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	return HAL_I2C_Mem_Read(&hi2c2, dev, reg, I2C_MEMADD_SIZE_8BIT, data, length, 200);
}

uint8_t HAL_IIC1_WriteMultiByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
	return HAL_I2C_Mem_Write(&hi2c2, dev, reg, I2C_MEMADD_SIZE_8BIT, data, length, 200);
}

//���ֽ�д�봫�����Ĵ���
//����0�����ִ���
//����1����������
uint8_t MPU9250_SingleWrite(uint8_t reg, uint8_t data)
{
	if(HAL_IIC1_WriteByteToSlave(GYRO_ADDRESS, reg, data))
		return 1;
	else
		return 0;
}  

//���ֽڶ�ȡ�������Ĵ���
//����0�����ִ���
//����1����������
uint8_t MPU9250_SingleRead(uint8_t reg, uint8_t *buf)
{
	if(HAL_IIC1_ReadByteFromSlave(GYRO_ADDRESS, reg, buf))
		return 1;
	else
		return 0;
}

uint8_t MPU9250_MultiWrite(uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(HAL_IIC1_WriteMultiByteToSlave(GYRO_ADDRESS, reg, len, buf))
		return 1;
	else
		return 0;
}

uint8_t MPU9250_MultiRead(uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(HAL_IIC1_ReadMultiByteFromSlave(GYRO_ADDRESS, reg, len, buf))
		return 1;
	else
		return 0;
}

//MPU9250���ó�ʼ��
void MPU9250_Init(void)
{
	uint8_t ret;
	MPU9250_SingleRead(WHO_AM_I, &ret);
	if(ret == 0x68)
	{
		MPU9250_SingleWrite(PWR_MGMT_1, 0x80); //��λMPU9250
		HAL_Delay(100);
		MPU9250_SingleWrite(PWR_MGMT_1, 0x00); //����MPU9250����ѡ��������x��PLLΪʱ��Դ (MPU9250_RA_PWR_MGMT_1, 0x01)
		MPU9250_SingleWrite(MPU_INT_EN_REG, 0x00); //��ֹ�ж�
		MPU9250_SingleWrite(GYRO_CONFIG, 0x18); //������������+-2000��/�� (��ͷֱ��� = 2^15/2000 = 16.4LSB/��/��
		MPU9250_SingleWrite(ACCEL_CONFIG, 0x08); //���ٶ�������+-4g   (��ͷֱ��� = 2^15/4g = 8196LSB/g )
		MPU9250_SingleWrite(CONFIG, 0x04);//�������ݵ����Ϊ1kHZ,DLPF=20Hz
		MPU9250_SingleWrite(SMPLRT_DIV, 0x00);  //������Ƶ (����Ƶ�� = ���������Ƶ�� / (1+DIV)������Ƶ��1000hz��
		MPU9250_SingleWrite(MPU_INTBP_CFG_REG, 0x02); //MPU ��ֱ�ӷ���MPU9250����I2C
	}
}

//��ȡ���ٶ����ݵ�������
void MPU9250_ReadAccelerateData(void)
{
	uint8_t buf[6];
   	MPU9250_MultiRead(ACCEL_XOUT_H, 6, buf);
    accData[0] = (int16_t)((buf[0] << 8) | buf[1]);
    accData[1] = (int16_t)((buf[2] << 8) | buf[3]);
    accData[2] = (int16_t)((buf[4] << 8) | buf[5]);
}

//��ȡŷ�������ݵ�������
void MPU9250_ReadGYROData(void)
{
	uint8_t buf[6];
	MPU9250_MultiRead(GYRO_XOUT_H, 6, buf);
    gyroData[0] = (int16_t)((buf[0] << 8) | buf[1]) ;
    gyroData[1] = (int16_t)((buf[2] << 8) | buf[3]) ;
    gyroData[2] = (int16_t)((buf[4] << 8) | buf[5]) ;
}

//��ȡ��ƫ�����ݵ�������
void MPU9250_ReadMagneticData(void)
{
	uint8_t buf[6];
    MPU9250_SingleWrite(MPU_INTBP_CFG_REG, 0x02); //�����Ŵ�����
    HAL_Delay(10);
    MPU9250_SingleWrite(0x0A, 0x11);
    HAL_Delay(10);
 
    MPU9250_MultiRead(MAG_XOUT_L, 6, buf);
    magData[0] = (int16_t)((buf[1] << 8) | buf[0]) ;
    magData[1] = (int16_t)((buf[3] << 8) | buf[2]) ;
    magData[2] = (int16_t)((buf[5] << 8) | buf[4]) ;
}

//��ȡ�¶����ݱ��浽ȫ�ֱ���
void MPU9250_CalculateTemperatureData(float *tempdata)
{
	uint8_t buf[2];
	short data;
	MPU9250_MultiRead(TEMP_OUT_H, 2, buf);
	data = (int16_t)((buf[0] << 8) | buf[1]) ;
	*tempdata = 21.0f + ((float)data/333.87f);
}

//������������
void MPU9250_Output(int16_t* buffer)
{
	buffer[0] = accData[0]; //x
	buffer[1] = accData[1]; //y
	buffer[2] = accData[2]; //z
	buffer[3] = gyroData[0];
	buffer[4] = gyroData[1];
	buffer[5] = gyroData[2];
	buffer[6] = magData[0];
	buffer[7] = magData[1];
	buffer[8] = magData[2];
}

