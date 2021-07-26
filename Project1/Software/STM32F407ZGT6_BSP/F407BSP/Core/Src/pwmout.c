#include "pwmout.h"
#include "tim.h"

#include <stdio.h>
#include <stdint.h>

PID_t pid;

volatile uint32_t *Motor_PhaseA_CCR = &TIM8->CCR1;
volatile uint32_t *Motor_PhaseB_CCR = &TIM8->CCR2;
volatile uint32_t *Motor_FREQ = &TIM8->PSC;

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
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	
	*Motor_PhaseA_CCR = 1000;
	*Motor_PhaseB_CCR = 1000;
}

//停止PWM输出
void PWM_Control_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
	*Motor_PhaseA_CCR = 0;
	*Motor_PhaseB_CCR = 0;
}

//输入一个角度，控制电机旋转
void PWM_Rotate(uint32_t angle)
{
	uint32_t temp;
	
	*Motor_FREQ = temp;

}



