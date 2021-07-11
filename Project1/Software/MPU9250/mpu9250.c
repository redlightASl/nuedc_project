#include "mpu9250.h"
#include "delay.h"
#include "usart.h"
#include <math.h>

static u8		RX_BUFFER[8]; //�������ݻ�����
static int* X; //x������
static int* Y; //y������
static int* Z; //z������
static int* Temperature; //�¶�����

/* IIC������ʼ�� */
void MPU9250_IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 

  GPIO_InitStructure.GPIO_Pin =  MPU9250_GPIO_SCL | MPU9250_GPIO_SDA; //SCL PE8��SDA PE9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�ڲ�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
	//Ӧ������
  GPIO_Init(MPU9250_IIC_PERIPH, &GPIO_InitStructure);
	
	//Ĭ����������
	MPU9250_IIC_SDA = 1;
  MPU9250_IIC_SCL = 1;
}

//����IIC��ʼ�ź�
//������0
//�ɹ�����1
u8 MPU9250_IIC_Start(void)
{
	MPU9250_SDA_OUT(); //SDA���ģʽ
	MPU9250_IIC_SDA = 1; //����SDA
	MPU9250_IIC_SCL = 1; //����SCL
	delay_us(10);
	
	if(!MPU9250_READ_SDA) //SDAΪ�͵�ƽ������æ
		return 0;
	
	MPU9250_IIC_SDA = 0;
	
	delay_us(10); //�ɻ���delay_us(4);
	
	if(MPU9250_READ_SDA) //SDAΪ�ߵ�ƽ������æ
		return 0;
	
	MPU9250_IIC_SCL = 0;
	delay_us(10); //�ɻ���delay_us(4);
	return 1;
}

//����IICֹͣ�ź�
void MPU9250_IIC_Stop(void)
{
	MPU9250_SDA_OUT(); //SDA���ģʽ
	
	MPU9250_IIC_SDA = 0; //SDA�½���Ϊֹͣ�ź�
	delay_us(10);
	MPU9250_IIC_SCL = 0;
	delay_us(10);
	MPU9250_IIC_SCL = 1;
	delay_us(10);
	MPU9250_IIC_SDA = 1;
} 

//����ACKӦ��
void MPU9250_IIC_Ack(void)
{
	MPU9250_SDA_OUT();
	
  MPU9250_IIC_SDA = 0; //��������
  delay_us(10);
  MPU9250_IIC_SCL = 1; //����1��ʱ������
  delay_us(10);
  MPU9250_IIC_SCL = 0;
  delay_us(10);
  MPU9250_IIC_SDA = 1; //�ͷ�����
}   

//������ACKӦ��
void MPU9250_IIC_NAck(void)
{
	//����Ӧ��ķ�����
  MPU9250_SDA_OUT();
	
	MPU9250_IIC_SCL = 0;
  MPU9250_IIC_SDA = 1;
  delay_us(10);
  MPU9250_IIC_SCL = 1;
  delay_us(10);
  MPU9250_IIC_SCL = 0;
  delay_us(10);
}

//IIC�ȴ�Ӧ���ź�
//����1������Ӧ��ʧ��
//����0������Ӧ��ɹ�
u8 MPU9250_IIC_WaitAck(void)
{
	u8 Err_time = 0;
	MPU9250_SDA_IN(); //SDA����ģʽ
	
	MPU9250_IIC_SCL = 0; //�ȴ�����Ӧ��
	delay_us(1);
	MPU9250_IIC_SDA = 1;
	delay_us(1);
	MPU9250_IIC_SCL = 1;
	delay_us(1);
	
	while(MPU9250_READ_SDA) //���ȴ�ʱ����������Ӧ��ʧ��
	{
		Err_time++;
		if(Err_time > 250)
		{
			MPU9250_IIC_Stop();
			return 1;
		}
	}
	
	MPU9250_IIC_SCL = 0; //����SCL
	return 0;
}

//IIC����һ���ֽ�
//���ݴӸ�λ����λ
void MPU9250_IIC_SendByte(u8 SendByte)
{
	u8 i = 0;
  MPU9250_SDA_OUT(); //SDA���ģʽ

  /* �ȷ����ֽڸ�λ */  
  for (i = 0; i < 8; i++)  
  {
		MPU9250_IIC_SDA = 0;
		delay_us(10);
		
		//��������
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
		
    if (i == 7) //����Ѿ�������8λ���ݣ�1�ֽڣ�
    {
      MPU9250_IIC_SDA = 1; //�ͷ�����
    }
    SendByte <<= 1; //��������������һλ
    delay_us(10);
  }  
}  

//IIC��ȡһ���ֽ�
//���ݴӸ�λ����λ
u8 MPU9250_IIC_RadeByte(void)
{
	u8 i;
  u8 value;
  MPU9250_IIC_SDA = 1;
  MPU9250_SDA_IN(); //SDA����ģʽ
	
  /* �ȶ�ȡ���ݵ�λ */  
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

//���ֽ�д�봫�����Ĵ���
//����0�����ִ���
//����1����������
u8 MPU9250_SingleWrite(u8 SlaveAddress, u8 RegisterAddress, u8 RegisterData)
{
  if(!MPU9250_IIC_Start())
		return 0;
	
	//I2C_SendByte(((RegisterAddress & 0x0700) >>7) | SlaveAddress & 0xFFFE); //���ø���ʼ��ַ + ������ַ
  MPU9250_IIC_SendByte(SlaveAddress); //�����豸��ַ
	if(!MPU9250_IIC_WaitAck()) //û�еȴ�����Ӧ�ź�
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_SendByte(RegisterAddress); //����Ҫд�ļĴ���
	if(!MPU9250_IIC_WaitAck()) //û�еȴ�����Ӧ�ź�
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_SendByte(RegisterData); //��Ĵ���д������
	if(!MPU9250_IIC_WaitAck()) //û�еȴ�����Ӧ�ź�
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	MPU9250_IIC_Stop();
	delay_ms(5);
  return 1;
}

//���ֽڶ�ȡ�������Ĵ���
//����0�����ִ���
//����1����������
u8 MPU9250_SingleRead(u8 SlaveAddress, u8 RegisterAddress)
{
	u8 RegisterData;
	
	if(!MPU9250_IIC_Start())
		return 0;
	
	//I2C_SendByte(((RegisterAddress & 0x0700) >>7) | SlaveAddress & 0xFFFE); //���ø���ʼ��ַ + ������ַ
  MPU9250_IIC_SendByte(SlaveAddress);
	if(!MPU9250_IIC_WaitAck())
	{
		MPU9250_IIC_Stop();
		return 0;
	}
	
	//����Ҫ���ļĴ���
	MPU9250_IIC_SendByte(RegisterAddress);
	if(!MPU9250_IIC_WaitAck()) //û�еȴ�����Ӧ�ź�
	{
		MPU9250_IIC_Stop();
		return 0;
	}

	MPU9250_IIC_Start();
  MPU9250_IIC_SendByte(SlaveAddress + 1);
  if(!MPU9250_IIC_WaitAck()) //û�еȴ�����Ӧ�ź�
	{
		MPU9250_IIC_Stop();
		return 0;
	}

	RegisterData = MPU9250_IIC_RadeByte();
	MPU9250_IIC_NAck();
  MPU9250_IIC_Stop();

	return RegisterData;
}

//MPU9250���ó�ʼ��
void MPU9250_Init(void)
{
	MPU9250_IIC_Init();
  //Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
  //Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
  //Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //��2000��
  //Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
  //Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //

  MPU9250_SingleWrite(GYRO_ADDRESS, PWR_MGMT_1, 0x00); //�������״̬����Դ��������
	MPU9250_SingleWrite(GYRO_ADDRESS, SMPLRT_DIV, 0x07); //�����ǲ�����125Hz
	MPU9250_SingleWrite(GYRO_ADDRESS, CONFIG, 0x06); //��ͨ�˲�Ƶ��5Hz
	MPU9250_SingleWrite(GYRO_ADDRESS, GYRO_CONFIG, 0x18); //�����ǲ��Լ죬������Χ2000deg/s
	MPU9250_SingleWrite(GYRO_ADDRESS, ACCEL_CONFIG, 0x01); //���ټƲ��Լ죬������Χ2G����ͨ�˲�Ƶ��5Hz

	//Single_Write(GYRO_ADDRESS, 0x6A, 0x00); //�ر�����ģʽ
}

//��ȡ���ٶ����ݵ�������
void MPU9250_ReadAccelerateData(void)
{
  RX_BUFFER[0] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_XOUT_L); //x��
  RX_BUFFER[1] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_XOUT_H);
  RX_BUFFER[2] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_YOUT_L); //y��
  RX_BUFFER[3] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_YOUT_H);
  RX_BUFFER[4] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_ZOUT_L); //z��
  RX_BUFFER[5] = MPU9250_SingleRead(ACCEL_ADDRESS, ACCEL_ZOUT_H);
}

//������ٶȽ�������浽ȫ�ֱ���
void MPU9250_CalculateAccelerateData(void)
{
	*X =	(RX_BUFFER[1] << 8) | RX_BUFFER[0];
  *X /= 164; //��ȡ����X������
	*Y =	(RX_BUFFER[3] << 8) | RX_BUFFER[2];
  *Y /= 164; //��ȡ����Y������
  *Z =	(RX_BUFFER[5] << 8) | RX_BUFFER[4];
  *Z /= 164; //��ȡ����Z������
}

//��ȡŷ�������ݵ�������
void MPU9250_ReadGYROData(void)
{
  RX_BUFFER[0] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_XOUT_L); 
  RX_BUFFER[1] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_XOUT_H);
  RX_BUFFER[2] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_YOUT_L);
  RX_BUFFER[3] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_YOUT_H);
  RX_BUFFER[4] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_ZOUT_L);
  RX_BUFFER[5] = MPU9250_SingleRead(GYRO_ADDRESS, GYRO_ZOUT_H);
}

//����ŷ���ǽ�������浽ȫ�ֱ���
void MPU9250_CalculateGYROData(void)
{
	*X =	(RX_BUFFER[1]<<8) | RX_BUFFER[0];
	*X /= 16.4; //��ȡ����X������
	*Y =	(RX_BUFFER[3]<<8) | RX_BUFFER[2];
  *Y /= 16.4; //��ȡ����Y������
	*Z =	(RX_BUFFER[5]<<8) | RX_BUFFER[4];
  *Z /= 16.4; //��ȡ����Z������
}

//������ҵĴ���������
void MPU9250_TurnOnMagnetic(void)
{
	MPU9250_SingleWrite(GYRO_ADDRESS, 0x37, 0x02); //��������������
  delay_ms(10);	
  MPU9250_SingleWrite(MAG_ADDRESS, 0x0A, 0x01);
  delay_ms(10);
}

//��ȡ��ƫ�����ݵ�������
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

//�����ƫ�ǽ�������浽ȫ�ֱ���
void MPU9250_CalculateMagneticData(void)
{
	*X = (RX_BUFFER[1] << 8) | RX_BUFFER[0]; //��ȡ����X������
	*Y = (RX_BUFFER[3] << 8) | RX_BUFFER[2]; //��ȡ����Y������
	*Z = (RX_BUFFER[5] << 8) | RX_BUFFER[4]; //��ȡ����Z������
}

//��ȡ�������¶����ݱ��浽ȫ�ֱ���
void MPU9250_CalculateTemperatureData(void)
{
	RX_BUFFER[6] = MPU9250_SingleRead(GYRO_ADDRESS, TEMP_OUT_L); 
  RX_BUFFER[7] = MPU9250_SingleRead(GYRO_ADDRESS, TEMP_OUT_H); 
  *Temperature = (RX_BUFFER[7] << 8) | RX_BUFFER[6];
  *Temperature = 35 + ((double) (*Temperature + 13200)) / 280;// ��ȡ������¶�
}

//�ض�������ָ��
//�����������������ָ��
//��ɺ�ʹ�ô����ָ������ȡ����õ����¶�����
void MPU9250_Redirect(int* x, int* y, int* z, int* temperature)
{
	X = x;
	Y = y;
	Z = z;
	Temperature = temperature;
}


