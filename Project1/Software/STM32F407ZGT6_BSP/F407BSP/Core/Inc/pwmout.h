#ifndef __PWM_OUTPUT_H
#define __PWM_OUTPUT_H
#include <stdio.h>
#include <stdint.h>

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
void PWM_Control_Stop(void);
void PWM_Rotate(uint32_t angle);

#endif