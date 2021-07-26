#include "hmc5883l.h"
#include "i2c.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

/* IIC底层驱动 */
uint8_t HAL_IIC2_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf)
{
	return HAL_I2C_Mem_Read(&hi2c1, I2C_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, 1, 100);
}

uint8_t HAL_IIC2_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data)
{
	uint8_t *pData;
	pData = &data;
	return HAL_I2C_Mem_Write(&hi2c1, I2C_Addr, reg, I2C_MEMADD_SIZE_8BIT, pData, 1, 100);
}

uint8_t HAL_IIC2_ReadMultiByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
	return HAL_I2C_Mem_Read(&hi2c1, dev, reg, I2C_MEMADD_SIZE_8BIT, data, length, 200);
}

uint8_t HAL_IIC2_WriteMultiByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
	return HAL_I2C_Mem_Write(&hi2c1, dev, reg, I2C_MEMADD_SIZE_8BIT, data, length, 200);
}

void HMC5883L_Init(void) //初始化HMC5883模块
{
	HAL_IIC2_WriteByteToSlave(HMC5883L_Write_Address, 0x00, 0x70);
	HAL_IIC2_WriteByteToSlave(HMC5883L_Write_Address, 0x01, 0xe0);
	HAL_IIC2_WriteByteToSlave(HMC5883L_Write_Address, 0x02, 0x01);
	//HAL_Delay(10);
	//HAL_IIC_WriteByteToSlave(HMC5883L_Write_Address, 0x02, 0x01);
}

void HMC5883L_CalculateCurrentMag_xyz(int* mag_x, int* mag_y, int* mag_z) //获取当前传感器回传磁偏角数据x、y、z
{
	uint8_t i;
	uint8_t a[6];

	for(i = 0; i < 6; i++)
	{
		HAL_IIC2_ReadByteFromSlave(HMC5883L_Write_Address, HMC5883L_Read_Address, a);
	}
	
	*mag_x = a[0];
	*mag_x <<= 8;
	*mag_x |= a[1];
	
	*mag_y = a[2];
	*mag_y <<= 8;
	*mag_y |= a[3];
	
	*mag_z = a[4];
	*mag_z <<= 8;  
	*mag_z |= a[5];
	
	if((*mag_x) > 32768)
	{
		*mag_x = -(0xFFFF - (*mag_x) + 1);
	}

	if((*mag_y)>32768)  
	{
		*mag_y = -(0xFFFF - (*mag_y) + 1);
	}
	
	if((*mag_z)>32768)
	{
		*mag_z = -(0xFFFF - (*mag_z) + 1);
	}
}

float HMC5883L_CalculateCurrentAngle(void) //计算当前角度
{
	int x,y,z,X,Y,Z;
	float Curent_Angle;
	
	HMC5883L_CalculateCurrentMag_xyz(&x, &y, &z);
	
	X = (uint16_t)x;    //x分量
	Y = (uint16_t)y;    //y分量
	Z = (uint16_t)z;    //z分量

	Curent_Angle = ((float)atan2(Y,X) * (180.0f / PI) + 180.0f); //计算实际水平角度
	
	return Curent_Angle;  
}
