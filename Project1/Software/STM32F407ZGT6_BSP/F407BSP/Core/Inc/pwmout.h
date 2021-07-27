#ifndef __PWM_OUTPUT_H
#define __PWM_OUTPUT_H
#include <stdio.h>
#include <stdint.h>

//步进电机步进角1.8度
//电源12V
#define MOTOR_STEP_ANGLE 1.8

#define MOV_THETA 4.0/9.0

struct pid_struct
{
     float target_val;               		//目标值
     float actual_val;                  //实际值
     float err;                         //定义偏差值
     float err_last;                    //定义上一个偏差值
     float Kp,Ki,Kd;                    //定义比例、积分、微分系数
     float integral;                    //定义积分值
};
typedef struct pid_struct PID_t;

extern PID_t pid;

float PID_realize(float temp_val);
void PID_param_init(void);
void PWM_Control_Init(void);
void PWM_Control_Stop(void);
void PWM_Rotate(uint32_t angle);
void pid_laser_pointer_rotate(uint32_t angle, uint8_t direction);

#endif

