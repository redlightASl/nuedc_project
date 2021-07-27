#ifndef __ENCODER_H
#define __ENCODER_H
#include <stdio.h>
#include <stdint.h>

/* 
编码器使用TIM3驱动
PA6 TIM3CH1 A相
PA7 TIM3CH1 B相
*/

void Encoder_Init(void);
uint8_t Encoder_GetDirection(void);
float Encoder_Get_Angle(void);
uint8_t close_to_middle(void);

#endif


