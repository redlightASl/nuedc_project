#include "hmc5883l.h"
#include "delay.h"
#include "math.h"

void HMC5883_IIC_Init(void) //初始化IIC总线驱动
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = HMC5883L_GPIO_SCL | HMC5883L_GPIO_SDA; //SCL PD3、SDA PD4
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //内部上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	//应用设置
  GPIO_Init(HMC5883L_IIC_PERIPH, &GPIO_InitStructure);
	
	//默认拉高总线
	HMC5883L_IIC_SDA = 1;
  HMC5883L_IIC_SCL = 1;
}

void HMC5883_IIC_Start(void) //产生IIC起始信号
{
	HMC5883L_SDA_OUT(); //SDA输出模式
	
	HMC5883L_IIC_SDA = 1; //拉高SDA
	delay_us(10);
	HMC5883L_IIC_SCL = 1; //拉高SCL
	delay_us(10);
	HMC5883L_IIC_SDA = 0;
	delay_us(10); //可换成delay_us(4);
	HMC5883L_IIC_SCL = 0;
	delay_us(10); //可换成delay_us(4);
}

void HMC5883_IIC_Stop(void) //产生IIC停止信号 
{
	HMC5883L_SDA_OUT(); //SDA输出模式
	
	HMC5883L_IIC_SDA = 0; //SDA下降沿为停止信号
	delay_us(10);
	HMC5883L_IIC_SCL = 0;
	delay_us(10);
	HMC5883L_IIC_SDA = 1;
}

//IIC等待应答信号
//返回1：接收应答失败
//返回0：接收应答成功
u8 HMC5883_IIC_WaitAck(void)
{  
	u8 Err_time = 0;
	HMC5883L_SDA_IN(); //SDA输入模式
	
	HMC5883L_IIC_SDA = 1; //等待接收应答 
	delay_us(1);
	HMC5883L_IIC_SDA = 1;
	delay_us(1);
	
	while(HMC5883L_READ_SDA) //若等待时间过长则接收应答失败
	{
		Err_time++;
		if(Err_time > 250)
		{
			HMC5883_IIC_Stop();
			return 1;
		}
	}
	
	HMC5883L_IIC_SCL = 0; //挂起SCL
	return 0;
}

void HMC5883_IIC_Ack(void) //产生ACK应答
{
  HMC5883L_SDA_OUT();
	
  HMC5883L_IIC_SDA = 0; //持有总线
  delay_us(10);
  HMC5883L_IIC_SCL = 1; //产生1个时钟跳变
  delay_us(10);
  HMC5883L_IIC_SCL = 0;
  delay_us(10);
  HMC5883L_IIC_SDA = 1; //释放总线
}

void HMC5883_IIC_NAck(void) //不产生ACK应答
{
	//产生应答的反操作
  HMC5883L_SDA_OUT();
	
  HMC5883L_IIC_SDA = 1;
  delay_us(10);
  HMC5883L_IIC_SCL = 1;
  delay_us(10);
  HMC5883L_IIC_SCL = 0;
  delay_us(10);
}

void HMC5883_IIC_SendByte(u8 txd) //IIC发送一个字节
{
  u8 i = 0;
  HMC5883L_SDA_OUT(); //SDA输出模式

  /* 先发送字节高位 */  
  for (i = 0; i < 8; i++)  
  {
		//发送数据
		if (txd & 0x80)
    {
      HMC5883L_IIC_SDA = 1;
    }
    else
    {
      HMC5883L_IIC_SDA = 0;
    }
    delay_us(10);
    HMC5883L_IIC_SCL = 1;
    delay_us(10);
    HMC5883L_IIC_SCL = 0;
		
    if (i == 7) //如果已经发送了8位数据（1字节）
    {
      HMC5883L_IIC_SDA = 1; //释放总线
    }
    txd <<= 1; //待发送数据左移一位
    delay_us(10);
  }
}

u8 HMC5883_IIC_ReadByte(void) //IIC读取一个字节
{
  u8 i;
  u8 value;
  HMC5883L_IIC_SDA = 1;
  HMC5883L_SDA_IN(); //SDA输入模式
	
  /* 先读取数据低位 */  
  value = 0;  
  for (i = 0; i < 8; i++)  
  {
    value <<= 1;
    HMC5883L_IIC_SCL = 1;
    delay_us(10);
    if (HMC5883L_READ_SDA)
    {
      value++;  
    }
    HMC5883L_IIC_SCL = 0;
    delay_us(10);
  }  
  return value;  
}  

void HMC5883L_Init(void) //初始化HMC5883模块
{
	//初始化总线驱动
  HMC5883_IIC_Init();
	//发送起始信号
  HMC5883_IIC_Start();
	//发送地址数据位
  HMC5883_IIC_SendByte(HMC5883L_Write_Address);
  HMC5883_IIC_WaitAck();

	//发送命令
  HMC5883_IIC_SendByte(0x00);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0x70);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0x01);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0xe0);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0x02);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0x01);
  HMC5883_IIC_Stop();
}

void HMC5883L_CalculateCurrentMag_xyz(int* mag_x, int* mag_y, int* mag_z) //获取当前传感器回传磁偏角数据x、y、z
{
	u8 i;
  u8 a[6];

  HMC5883_IIC_Start();
  HMC5883_IIC_SendByte(HMC5883L_Write_Address);
  HMC5883_IIC_WaitAck();
  HMC5883_IIC_SendByte(0x03);
  HMC5883_IIC_WaitAck();

  HMC5883_IIC_Start();
  HMC5883_IIC_SendByte(HMC5883L_Read_Address);
  HMC5883_IIC_WaitAck();
	
  for(i=0;i<6;i++)
  {
    a[i] = HMC5883_IIC_ReadByte();
    if(i==5)
    {
      HMC5883_IIC_NAck();
    }
    else
    {
      HMC5883_IIC_NAck();
    }
  }
  HMC5883_IIC_Stop();
	
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
	
  X = (s16)x;    //x分量
  Y = (s16)y;    //y分量
  Z = (s16)z;    //z分量

  Curent_Angle = (atan2(Y,X) * (180 / PI) + 180); //计算实际水平角度
	
  return Curent_Angle;  
}
