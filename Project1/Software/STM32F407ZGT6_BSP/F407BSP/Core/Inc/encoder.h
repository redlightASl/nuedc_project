#ifndef __ENCODER_H
#define __ENCODER_H
#include <stdio.h>
#include <stdint.h>

volatile uint8_t Encoder_GetDirection(void);
volatile float Encoder_Get_Angle(void);

#endif


