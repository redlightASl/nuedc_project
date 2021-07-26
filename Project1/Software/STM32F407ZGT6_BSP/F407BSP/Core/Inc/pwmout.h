#ifndef __PWM_OUTPUT_H
#define __PWM_OUTPUT_H
#include <stdio.h>
#include <stdint.h>

struct pid_struct
{
     float target_val;               		//Ŀ��ֵ
     float actual_val;                  //ʵ��ֵ
     float err;                         //����ƫ��ֵ
     float err_last;                    //������һ��ƫ��ֵ
     float Kp,Ki,Kd;                    //������������֡�΢��ϵ��
     float integral;                    //�������ֵ
};
typedef struct pid_struct PID_t;

extern PID_t pid;

float PID_realize(float temp_val);
void PID_param_init(void);
void PWM_Control_Stop(void);
void PWM_Rotate(uint32_t angle);

#endif