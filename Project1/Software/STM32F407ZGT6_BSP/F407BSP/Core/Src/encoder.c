#include "encoder.h"
#include "tim.h"
#include <stdio.h>
#include <stdint.h>

void Encoder_Init(void)
{
	return;
}

//使用编码器获得摆动方向
//正向输出1
//反向输出0
volatile uint8_t Encoder_GetDirection(void)
{
	uint32_t uwDirection = 0;//读取计数方向
	uwDirection = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
	
	if(uwDirection == 0) //正向
	{
		return 1;
	}
	else //反向
	{
		return 0;
	}
}

//使用编码器获得摆角
volatile float Encoder_Get_Angle(void)
{
	volatile float theta = 0;
	uint8_t flag = 0;
	theta = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim3));//获取定时器的值
	flag = Encoder_GetDirection();
	if(flag == 0) //正向摆动
	{
		return theta;
	}
	else //逆向摆动
	{
		return -1.0 * theta;
	}
}


