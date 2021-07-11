#include "hmc5883l.h"
#include "delay.h"
#include "math.h"

void HMC5883_IIC_Init(void) //��ʼ��IIC��������
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = HMC5883L_GPIO_SCL | HMC5883L_GPIO_SDA; //SCL PD3��SDA PD4
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�ڲ�����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	//Ӧ������
  GPIO_Init(HMC5883L_IIC_PERIPH, &GPIO_InitStructure);
	
	//Ĭ����������
	HMC5883L_IIC_SDA = 1;
  HMC5883L_IIC_SCL = 1;
}

void HMC5883_IIC_Start(void) //����IIC��ʼ�ź�
{
	HMC5883L_SDA_OUT(); //SDA���ģʽ
	
	HMC5883L_IIC_SDA = 1; //����SDA
	delay_us(10);
	HMC5883L_IIC_SCL = 1; //����SCL
	delay_us(10);
	HMC5883L_IIC_SDA = 0;
	delay_us(10); //�ɻ���delay_us(4);
	HMC5883L_IIC_SCL = 0;
	delay_us(10); //�ɻ���delay_us(4);
}

void HMC5883_IIC_Stop(void) //����IICֹͣ�ź� 
{
	HMC5883L_SDA_OUT(); //SDA���ģʽ
	
	HMC5883L_IIC_SDA = 0; //SDA�½���Ϊֹͣ�ź�
	delay_us(10);
	HMC5883L_IIC_SCL = 0;
	delay_us(10);
	HMC5883L_IIC_SDA = 1;
}

//IIC�ȴ�Ӧ���ź�
//����1������Ӧ��ʧ��
//����0������Ӧ��ɹ�
u8 HMC5883_IIC_WaitAck(void)
{  
	u8 Err_time = 0;
	HMC5883L_SDA_IN(); //SDA����ģʽ
	
	HMC5883L_IIC_SDA = 1; //�ȴ�����Ӧ�� 
	delay_us(1);
	HMC5883L_IIC_SDA = 1;
	delay_us(1);
	
	while(HMC5883L_READ_SDA) //���ȴ�ʱ����������Ӧ��ʧ��
	{
		Err_time++;
		if(Err_time > 250)
		{
			HMC5883_IIC_Stop();
			return 1;
		}
	}
	
	HMC5883L_IIC_SCL = 0; //����SCL
	return 0;
}

void HMC5883_IIC_Ack(void) //����ACKӦ��
{
  HMC5883L_SDA_OUT();
	
  HMC5883L_IIC_SDA = 0; //��������
  delay_us(10);
  HMC5883L_IIC_SCL = 1; //����1��ʱ������
  delay_us(10);
  HMC5883L_IIC_SCL = 0;
  delay_us(10);
  HMC5883L_IIC_SDA = 1; //�ͷ�����
}

void HMC5883_IIC_NAck(void) //������ACKӦ��
{
	//����Ӧ��ķ�����
  HMC5883L_SDA_OUT();
	
  HMC5883L_IIC_SDA = 1;
  delay_us(10);
  HMC5883L_IIC_SCL = 1;
  delay_us(10);
  HMC5883L_IIC_SCL = 0;
  delay_us(10);
}

void HMC5883_IIC_SendByte(u8 txd) //IIC����һ���ֽ�
{
  u8 i = 0;
  HMC5883L_SDA_OUT(); //SDA���ģʽ

  /* �ȷ����ֽڸ�λ */  
  for (i = 0; i < 8; i++)  
  {
		//��������
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
		
    if (i == 7) //����Ѿ�������8λ���ݣ�1�ֽڣ�
    {
      HMC5883L_IIC_SDA = 1; //�ͷ�����
    }
    txd <<= 1; //��������������һλ
    delay_us(10);
  }
}

u8 HMC5883_IIC_ReadByte(void) //IIC��ȡһ���ֽ�
{
  u8 i;
  u8 value;
  HMC5883L_IIC_SDA = 1;
  HMC5883L_SDA_IN(); //SDA����ģʽ
	
  /* �ȶ�ȡ���ݵ�λ */  
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

void HMC5883L_Init(void) //��ʼ��HMC5883ģ��
{
	//��ʼ����������
  HMC5883_IIC_Init();
	//������ʼ�ź�
  HMC5883_IIC_Start();
	//���͵�ַ����λ
  HMC5883_IIC_SendByte(HMC5883L_Write_Address);
  HMC5883_IIC_WaitAck();

	//��������
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

void HMC5883L_CalculateCurrentMag_xyz(int* mag_x, int* mag_y, int* mag_z) //��ȡ��ǰ�������ش���ƫ������x��y��z
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

float HMC5883L_CalculateCurrentAngle(void) //���㵱ǰ�Ƕ�
{
	int x,y,z,X,Y,Z;
	float Curent_Angle;
	
  HMC5883L_CalculateCurrentMag_xyz(&x, &y, &z);
	
  X = (s16)x;    //x����
  Y = (s16)y;    //y����
  Z = (s16)z;    //z����

  Curent_Angle = (atan2(Y,X) * (180 / PI) + 180); //����ʵ��ˮƽ�Ƕ�
	
  return Curent_Angle;  
}
