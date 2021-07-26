#ifndef __HMC5883_H
#define __HMC5883_H
#include <stdlib.h>
#include <stdint.h>

/*
 * 电子罗盘hmc5883l模块
 * 使用IIC总线协议
 * PB6	SCL
 * PB7  SDA
 */

//传感器控制位地址
#define HMC5883L_Write_Address 0x3C
#define HMC5883L_Read_Address (0x3C + 1)

#define QMC5883L_Write_Address 0x0D
#define QMC5883L_Read_Address (0x0D + 1)

//数学计算用数
#define PI 3.14159265

uint8_t HAL_IIC2_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t HAL_IIC2_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);
uint8_t HAL_IIC2_ReadMultiByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t HAL_IIC2_WriteMultiByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
void HMC5883L_Init(void);
void HMC5883L_CalculateCurrentMag_xyz(int* mag_x, int* mag_y, int* mag_z);
float HMC5883L_CalculateCurrentAngle(void);

#endif

