#ifndef _MOTION_H
#define _MOTION_H
#include "sys.h"


void Motion_Init(u32 arr,u32 psc);
void Motion_Forward(u8 speed,u8 dira);
void Motion_Turn(u8 dirt,u8 test_sp);
void Motion_Stop(void);

#endif
