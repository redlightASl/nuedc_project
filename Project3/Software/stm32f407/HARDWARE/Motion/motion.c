/**
 * @file motion.c
 * @brief
 * @author
 * @version 1.0
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-11-29 <td>1.0     <td>wangh     <td>Content
 * </table>
 *
 * @note 在本函数中定义：
 * PC6与PC7控制左轮，PC8与PC9控制右轮
 * #取PC6为前进方向
 * PC6:TIM_SetCompare1
 * PC7:TIM_SetCompare2
 * #取PC8为前进方向
 * PC8:TIM_SetCompare3
 * PC9:TIM_SetCompare4
 */
#include "motion.h"

 /**
  * @brief 动作运行初始化函数
  * @param  arr              ARR寄存器值
  * @param  psc              定时器预分频器值
  */
void Motion_Init(u32 arr, u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OC1InitStructure;
	TIM_OCInitTypeDef  TIM_OC2InitStructure;
	TIM_OCInitTypeDef  TIM_OC3InitStructure;
	TIM_OCInitTypeDef  TIM_OC4InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	//初始化TIM3 Channel1 Channel2 Channel3 Channel4  PWM模式	 
	TIM_OC1InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC1InitStructure.TIM_Pulse = 1500;
	TIM_OC1InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM3, &TIM_OC1InitStructure);  //根据T指定的参数初始化外设TIM1 4OC1


	TIM_OC2InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC2InitStructure.TIM_Pulse = 1500;
	TIM_OC2InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM3, &TIM_OC2InitStructure);  //根据T指定的参数初始化外设TIM1 4OC2

	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC3InitStructure.TIM_Pulse = 1500;
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM3, &TIM_OC2InitStructure);  //根据T指定的参数初始化外设TIM1 4OC3

	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC4InitStructure.TIM_Pulse = 1500;
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM3, &TIM_OC2InitStructure);  //根据T指定的参数初始化外设TIM1 4OC4

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPE使能 

	TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 直行函数
 * @param  speed            直行速度
 * @param  dira             直行方向,0：后退;1：前进
 * @note 使小车进行直行
 * PC6、PC8为正脉冲输出脚
 * PC7、PC9为负脉冲输出脚
 */
void Motion_Forward(u8 speed, u8 dira)
{
	if (dira == 0)
	{
		TIM_SetCompare1(TIM3, 0);
		TIM_SetCompare2(TIM3, speed);
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, speed);
	}
	else if (dira == 1)
	{
		TIM_SetCompare1(TIM3, speed);
		TIM_SetCompare2(TIM3, 0);
		TIM_SetCompare3(TIM3, speed);
		TIM_SetCompare4(TIM3, 0);
	}
}

/**
 * @brief 转向函数
 * @param  dirt             旋转方向,0:左转;1:右转
 * @param  test_sp          测试转向速度接口，实际运行给默认值
 */
void Motion_Turn(u8 dirt, u8 test_sp)
{
	if (dirt == 0)
	{
		TIM_SetCompare1(TIM3, 0);
		TIM_SetCompare2(TIM3, test_sp);
		TIM_SetCompare3(TIM3, test_sp);
		TIM_SetCompare4(TIM3, 0);
	}
	else if (dirt == 1)
	{
		TIM_SetCompare1(TIM3, test_sp);
		TIM_SetCompare2(TIM3, 0);
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, test_sp);
	}
}

/**
 * @brief 动作停止函数
 * @note 让小车动作停止
 */
void Motion_Stop(void)
{
	TIM_SetCompare1(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);
	TIM_SetCompare3(TIM3, 0);
	TIM_SetCompare4(TIM3, 0);
}
