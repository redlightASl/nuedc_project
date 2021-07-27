#include "encoder.h"
#include "tim.h"
#include "mpu9250.h"
#include <stdio.h>
#include <stdint.h>

void Encoder_Init(void)
{
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

//ʹ�ñ�������ðڶ�����
//�������1
//�������0
uint8_t Encoder_GetDirection(void)
{
	uint32_t uwDirection = 0;//��ȡ��������
	uwDirection = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
	
	if(uwDirection == 0) //����
	{
		return 1;
	}
	else //����
	{
		return 0;
	}
}

//ʹ�ñ�������ðڽ�
float Encoder_Get_Angle(void)
{
	volatile float theta = 0;
	uint8_t flag = 0;
	theta = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim3));//��ȡ��ʱ����ֵ
	flag = Encoder_GetDirection();
	if(flag == 0) //����ڶ�
	{
		return theta;
	}
	else //����ڶ�
	{
		return -1.0 * theta;
	}
}

//�ж��Ƿ񿿽�ƽ��λ��
//�� ���1
//�� ���0
uint8_t close_to_middle(void)
{
	int16_t my_mpu9250_data[9];
	MPU9250_ReadAccelerateData();
	MPU9250_Output(my_mpu9250_data);
	if(my_mpu9250_data[0] > 0) //����y������ٶ�
	{
		return 1;
	}
	else if(my_mpu9250_data[0] < 0) //����y������ٶ�
	{
		return 0;
	}
}
