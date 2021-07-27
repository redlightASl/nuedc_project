#include "pwmout.h"
#include "tim.h"
#include "encoder.h"
#include <stdio.h>
#include <stdint.h>

PID_t pid;

volatile uint32_t motor_int_count;
volatile uint32_t *Motor_PhaseA_CCR = &TIM1->CCR1;
volatile uint32_t *Motor_PhaseB_CCR = &TIM8->CCR1;

void PID_param_init(void)
{
     /* PID��ʼ������ */
     pid.target_val=0.0;
     pid.actual_val=0.0;
     pid.err=0.0;
     pid.err_last=0.0;
     pid.integral=0.0;
	 
     pid.Kp = 1.0;
     pid.Ki = 0.0;
     pid.Kd = -0.4;
}

//PID�㷨
float PID_realize(float temp_val)
{
     /*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err=pid.target_val-temp_val;
     /*����ۻ�*/
    pid.integral+=pid.err;
     /*PID�㷨ʵ��*/
    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
     /*����*/
    pid.err_last=pid.err;
     /*���ص�ǰʵ��ֵ*/
    return pid.actual_val;
}

//PWM��ʼ��
void PWM_Control_Init(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_ALL);
	
	*Motor_PhaseA_CCR = 1000-1;
	*Motor_PhaseB_CCR = 1000-1;
	
	//HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_ALL);
}

//ֹͣPWM���
void PWM_Control_Stop(void)
{
	HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
	*Motor_PhaseA_CCR = 0;
	*Motor_PhaseB_CCR = 0;
}

//����һ���Ƕȣ����Ƶ����ת
void PWM_Rotate(uint32_t angle)
{
	/* ��������㲽�����ÿһ����ת�ĽǶ� */
	motor_int_count = angle / MOTOR_STEP_ANGLE; //���Ʋ���������� = �Ƕ��� / ������
	if(Encoder_GetDirection() == 1) //��ת��˳ʱ��
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //ƽ�巴ת
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	}
	else //��ת����ʱ��
	{
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //ƽ����ת
		HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	}
}

//����һ���ǶȺ�һ�����򣬿��Ƶ����ת
void pid_laser_pointer_rotate(uint32_t angle, uint8_t direction)
{
	motor_int_count = PID_realize(angle / MOTOR_STEP_ANGLE); //���Ʋ���������� = �Ƕ��� / ������
	if(direction == 0) //��ת
	{
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //ƽ����ת
		HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	}
	else
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //ƽ�巴ת
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t motor_int_count_i;
	
	if(htim->Instance == TIM8)
	{
		if(motor_int_count_i < motor_int_count)
		{
			motor_int_count_i++; //���㲽��
		}
		else
		{
            motor_int_count_i = 0;
			HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_ALL); //�ٴδ��ж�
		}
	}
	else if(htim->Instance == TIM1)
	{
		if(motor_int_count_i < motor_int_count)
		{
			motor_int_count_i++; //���㲽��
		}
		else
		{
            motor_int_count_i = 0;
			HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_ALL); //�ٴδ��ж�
		}
	}
}

