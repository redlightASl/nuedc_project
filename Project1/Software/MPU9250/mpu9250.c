#include "mpu9250.h"
#include "delay.h"
#include "usart.h"
#include <math.h>

static u8		RX_BUFFER[8]; //接收数据缓存区
static int* X; //x轴数据
static int* Y; //y轴数据
static int* Z; //z轴数据
static int* Temperature; //温度数据

/* IIC驱动初始化 */
void MPU9250_IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 

  GPIO_InitStructure.GPIO_Pin =  MPU9250_GPIO_SCL | MPU9250_GPIO_SDA; //SCL PE8、SDA PE9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //内部上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	//应用配置
  GPIO_Init(MPU9250_IIC_PERIPH, &GPIO_InitStructure);
	
	//默认拉高总线
	MPU9250_IIC_SDA = 1;
  MPU9250_IIC_SCL = 1;
}

//产生IIC起始信号
//出错返回0
//成功返回1
u8 MPU9250_IIC_Start(void)
{
	MPU9250_SDA_OUT(); //SDA输出模式
	MPU9250_IIC_SDA = 1; //拉高SDA
	MPU9250_IIC_SCL = 1; //拉高SCL
	delay_us(10);
	
	if(!MPU9250_READ_SDA) //SDA为低电平则总线忙
		return 0;
	
	MPU9250_IIC_SDA = 0;
	
	delay_us(10); //可换成delay_us(4);
	
	if(MPU9250_READ_SDA) //SDA为高电平则总线忙
		return 0;
	
	MPU9250_IIC_SCL = 0;
	delay_us(10); //可换成delay_us(4);
	return 1;
}

//产生IIC停止信号
void MPU9250_IIC_Stop(void)
{
	MPU9250_SDA_OUT(); //SDA输出模式
	
	MPU9250_IIC_SDA = 0; //SDA下降沿为停止信号
	delay_us(10);
	MPU9250_IIC_SCL = 0;
	delay_us(10);
	MPU9250_IIC_SCL = 1;
	delay_us(10);
	MPU9250_IIC_SDA = 1;
} 

//产生ACK应答
void MPU9250_IIC_Ack(void)
{
	MPU9250_SDA_OUT();
	
  MPU9250_IIC_SDA = 0; //持有总线
  delay_us(10);
  MPU9250_IIC_SCL = 1; //产生1个时钟跳变
  delay_us(10);
  MPU9250_IIC_SCL = 0;
  delay_us(10);
  MPU9250_IIC_SDA = 1; //释放总线
}   

//不产生ACK应答
void MPU9250_IIC_NAck(void)
{
	//产生应答的反操作
  MPU9250_SDA_OUT();
	
	MPU9250_IIC_SCL = 0;
  MPU9250_IIC_SDA = 1;
  delay_us(10);
  MPU9250_IIC_SCL = 1;
  delay_us(10);
  MPU9250_IIC_SCL = 0;
  delay_us(10);
}

//IIC等待应答信号
//返回1：接收应答失败
//返回0：接收应答成功
u8 MPU9250_IIC_WaitAck(void)
{
	u8 Err_time = 0;
	MPU9250_SDA_IN(); //SDA输入模式
	
	MPU9250_IIC_SCL = 0; //等待接收应答
	delay_us(1);
	MPU9250_IIC_SDA = 1;
	delay_us(1);
	MPU9250_IIC_SCL = 1;
	delay_us(1);
	
	while(MPU9250_READ_SDA) //若等待时间过长则接收应答失败
	{
		Err_time++;
		if(Err_time > 250)
		{
			MPU9250_IIC_Stop();
			return 1;
		}
	}
	
	MPU9250_IIC_SCL = 0; //挂起SCL
	return 0;
}

//IIC发送一个字节
//数据从高位到低位
void MPU9250_IIC_SendByte(u8 SendByte)
{
	u8 i = 0;
  MPU9250_SDA_OUT(); //SDA输出模式

  /* 先发送字节高位 */  
  for (i = 0; i < 8; i++)  
  {
		MPU9250_IIC_SDA = 0;
		delay_us(10);
		
		//发送数据
		if (SendByte & 0x80)
    {
      MPU9250_IIC_SDA = 1;
    }
    else
    {
      MPU9250_IIC_SDA = 0;
    }
    delay_us(10);
    MPU9250_IIC_SCL = 1;
    delay_us(10);
    MPU9250_IIC_SCL = 0;
		
    if (i == 7) //如果已经发送了8位数据（1字节）
    {
      MPU9250_IIC_SDA = 1; //释放总线
    }
    SendByte <<= 1; //待发送数据左移一位
    delay_us(10);
  }  
}  

//IIC读取一个字节
//数据从高位到低位
u8 MPU9250_IIC_RadeByte(void)
{
	u8 i;
  u8 value;
  MPU9250_IIC_SDA = 1;
  MPU9250_SDA_IN(); //SDA输入模式
	
  /* 先读取数据低位 */  
  value = 0;  
  for (i = 0; i < 8; i++)  
  {
    value <<= 1;
    MPU9250_IIC_SCL = 1;
    delay_us(10);
    if (MPU9250_READ_SDA)
    {
      value++;  
    }
    MPU9250_IIC_SCL = 0;
    delay_us(10);
  }  
  return value;
}

//单字节写入传感器寄存器
//返回0：出现错误
//返回1：正常运行
u8 MPU9250_SingleWrite(u8 SlaveAddress, u8 RegisterAddress, u8 RegisterData)
{
  if(!MPU9250_IIC_Start())
		return 0;
	
	//I2C_SendByte(((RegisterAddress & 0x0700) >>7) | SlaveAddress & 0xFFFE); //设置高起始地址 + 器件地址
  MPU9250_IIC_SendByte(SlaveAddress); //设置设备地址
	if(!MPU9250_IIC_WaitAck()) //没有等待到响应信号
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_SendByte(RegisterAddress); //设置要写的寄存器
	if(!MPU9250_IIC_WaitAck()) //没有等待到响应信号
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_SendByte(RegisterData); //向寄存器写入数据
	if(!MPU9250_IIC_WaitAck()) //没有等待到响应信号
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_Stop();
	delay_ms(5);
  return 1;
}

//单字节读取传感器寄存器
//返回0：出现错误
//返回1：正常运行
u8 MPU9250_SingleRead(u8 SlaveAddress, u8 RegisterAddress)
{
	u8 RegisterData;
	
	if(!MPU9250_IIC_Start())
		return 0;
	
	//I2C_SendByte(((RegisterAddress & 0x0700) >>7) | SlaveAddress & 0xFFFE); //设置高起始地址 + 器件地址
  MPU9250_IIC_SendByte(SlaveAddress);
	if(!MPU9250_IIC_WaitAck())
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	//设置要读的寄存器
	MPU9250_IIC_SendByte(RegisterAddress);
	if(!MPU9250_IIC_WaitAck()) //没有等待到响应信号
	{
		MPU9250_IIC_Stop();
		return 0;
	}

	MPU9250_IIC_Start();
  MPU9250_IIC_SendByte(SlaveAddress + 1);
  if(!MPU9250_IIC_WaitAck()) //没有等待到响应信号
	{
		MPU9250_IIC_Stop();
		return 0;
	}

	RegisterData = MPU9250_IIC_RadeByte();
	MPU9250_IIC_NAck();
  MPU9250_IIC_Stop();

	return RegisterData;
}

//MPU9250配置初始化
void MPU9250_Init(void)
{
	MPU9250_IIC_Init();
  //Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
  //Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
  //Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
  //Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
  //Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //

  MPU9250_SingleWrite(GYRO_ADDRESS, PWR_MGMT_1, 0x00); //解除休眠状态，电源正常启用
	MPU9250_SingleWrite(GYRO_ADDRESS, SMPLRT_DIV, 0x07); //陀螺仪采样率125Hz
	MPU9250_SingleWrite(GYRO_ADDRESS, CONFIG, 0x06); //低通滤波频率5Hz
	MPU9250_SingleWrite(GYRO_ADDRESS, GYRO_CONFIG, 0x18); //陀螺仪不自检，测量范围2000deg/s
	MPU9250_SingleWrite(GYRO_ADDRESS, ACCEL_CONFIG, 0x01); //加速计不自检，测量范围2G，高通滤波频率5Hz

	//Single_Write(GYRO_ADDRESS, 0x6A, 0x00); //关闭主机模式
}

//读取加速度数据到缓存区
void MPU9250_ReadAccelerateData(void)
{
  RX_BUFFER[0] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_XOUT_L); //x轴
  RX_BUFFER[1] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_XOUT_H);
  RX_BUFFER[2] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_YOUT_L); //y轴
  RX_BUFFER[3] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_YOUT_H);
  RX_BUFFER[4] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_ZOUT_L); //z轴
  RX_BUFFER[5] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_ZOUT_H);
}

//计算加速度结果并保存到全局变量
void MPU9250_CalculateAccelerateData(void)
{
	*X =	(RX_BUFFER[1] << 8) | RX_BUFFER[0];
  *X /= 164; //读取计算X轴数据
	*Y =	(RX_BUFFER[3] << 8) | RX_BUFFER[2];
  *Y /= 164; //读取计算Y轴数据
  *Z =	(RX_BUFFER[5] << 8) | RX_BUFFER[4];
  *Z /= 164; //读取计算Z轴数据
}

//读取欧拉角数据到缓存区
void MPU9250_ReadGYROData(void)
{
  RX_BUFFER[0] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_XOUT_L); 
  RX_BUFFER[1] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_XOUT_H);
  RX_BUFFER[2] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_YOUT_L);
  RX_BUFFER[3] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_YOUT_H);
  RX_BUFFER[4] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_ZOUT_L);
  RX_BUFFER[5] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_ZOUT_H);
}

//计算欧拉角结果并保存到全局变量
void MPU9250_CalculateGYROData(void)
{
	*X =	(RX_BUFFER[1]<<8) | RX_BUFFER[0];
	*X /= 16.4; //读取计算X轴数据
	*Y =	(RX_BUFFER[3]<<8) | RX_BUFFER[2];
  *Y /= 16.4; //读取计算Y轴数据
	*Z =	(RX_BUFFER[5]<<8) | RX_BUFFER[4];
  *Z /= 16.4; //读取计算Z轴数据
}

//开启外挂的磁力传感器
void MPU9250_TurnOnMagnetic(void)
{
	MPU9250_SingleWrite(GYRO_ADDRESS, 0x37, 0x02); //开启磁力传感器
  delay_ms(10);	
  MPU9250_SingleWrite(MAG_ADDRESS, 0x0A, 0x01);
  delay_ms(10);
}

//读取磁偏角数据到缓存区
void MPU9250_ReadMagneticData(void)
{
	MPU9250_TurnOnMagnetic();
  RX_BUFFER[0] = MPU9250_SingleRead(MAG_ADDRESS, MAG_XOUT_L);
  RX_BUFFER[1] = MPU9250_SingleRead(MAG_ADDRESS, MAG_XOUT_H);
  RX_BUFFER[2] = MPU9250_SingleRead(MAG_ADDRESS, MAG_YOUT_L);
  RX_BUFFER[3] = MPU9250_SingleRead(MAG_ADDRESS, MAG_YOUT_H);
  RX_BUFFER[4] = MPU9250_SingleRead(MAG_ADDRESS, MAG_ZOUT_L);
  RX_BUFFER[5] = MPU9250_SingleRead(MAG_ADDRESS, MAG_ZOUT_H);
}

//计算磁偏角结果并保存到全局变量
void MPU9250_CalculateMagneticData(void)
{
	*X = (RX_BUFFER[1] << 8) | RX_BUFFER[0]; //读取计算X轴数据
	*Y = (RX_BUFFER[3] << 8) | RX_BUFFER[2]; //读取计算Y轴数据
	*Z = (RX_BUFFER[5] << 8) | RX_BUFFER[4]; //读取计算Z轴数据
}

//读取并计算温度数据保存到全局变量
void MPU9250_CalculateTemperatureData(void)
{
	RX_BUFFER[6] = MPU9250_SingleRead(GYRO_ADDRESS, TEMP_OUT_L); 
  RX_BUFFER[7] = MPU9250_SingleRead(GYRO_ADDRESS, TEMP_OUT_H); 
  *Temperature = (RX_BUFFER[7] << 8) | RX_BUFFER[6];
  *Temperature = 35 + ((double) (*Temperature + 13200)) / 280;// 读取计算出温度
}

//重定向数据指针
//向这个函数传入三个指针
//完成后使用传入的指针来读取计算得到的温度数据
void MPU9250_Redirect(int* x, int* y, int* z, int* temperature)
{
	X = x;
	Y = y;
	Z = z;
	Temperature = temperature;
}


