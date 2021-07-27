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
     /* PID初始化参数 */
     pid.target_val=0.0;
     pid.actual_val=0.0;
     pid.err=0.0;
     pid.err_last=0.0;
     pid.integral=0.0;
	 
     pid.Kp = 1.0;
     pid.Ki = 0.0;
     pid.Kd = -0.4;
}

//PID算法
float PID_realize(float temp_val)
{
     /*计算目标值与实际值的误差*/
    pid.err=pid.target_val-temp_val;
     /*误差累积*/
    pid.integral+=pid.err;
     /*PID算法实现*/
    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
     /*误差传递*/
    pid.err_last=pid.err;
     /*返回当前实际值*/
    return pid.actual_val;
}

//PWM初始化
void PWM_Control_Init(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_ALL);
	
	*Motor_PhaseA_CCR = 1000-1;
	*Motor_PhaseB_CCR = 1000-1;
	
	//HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_ALL);
}

//停止PWM输出
void PWM_Control_Stop(void)
{
	HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
	*Motor_PhaseA_CCR = 0;
	*Motor_PhaseB_CCR = 0;
}

//输入一个角度，控制电机旋转
void PWM_Rotate(uint32_t angle)
{
	/* 在这里计算步进电机每一步旋转的角度 */
	motor_int_count = angle / MOTOR_STEP_ANGLE; //控制步进电机步数 = 角度数 / 步进角
	if(Encoder_GetDirection() == 1) //正转，顺时针
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //平板反转
		HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	}
	else //反转，逆时针
	{
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //平板正转
		HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	}
}

//输入一个角度和一个方向，控制电机旋转
void pid_laser_pointer_rotate(uint32_t angle, uint8_t direction)
{
	motor_int_count = PID_realize(angle / MOTOR_STEP_ANGLE); //控制步进电机步数 = 角度数 / 步进角
	if(direction == 0) //正转
	{
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //平板正转
		HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	}
	else
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //平板反转
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
			motor_int_count_i++; //计算步数
		}
		else
		{
            motor_int_count_i = 0;
			HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_ALL); //再次打开中断
		}
	}
	else if(htim->Instance == TIM1)
	{
		if(motor_int_count_i < motor_int_count)
		{
			motor_int_count_i++; //计算步数
		}
		else
		{
            motor_int_count_i = 0;
			HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_ALL); //再次打开中断
		}
	}
}

