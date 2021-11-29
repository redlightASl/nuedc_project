#include "pwm.h"
#include "led.h"
#include "usart.h"

/**
 * @brief TIM14 PWM输出初始化
 * @param  arr              自动重装值
 * @param  psc              时钟预分频数
 */
void TIM3_PWM_Init(u32 arr, u32 psc)
{
	//此部分需手动修改IO口设置

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OC1InitStructure;
	TIM_OCInitTypeDef  TIM_OC2InitStructure;
	TIM_OCInitTypeDef  TIM_OC3InitStructure;
	TIM_OCInitTypeDef  TIM_OC4InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTB时钟	

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); //GPIOB8\GPIOB9复用为定时器3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); //GPIOB8\GPIOB9复用为定时器3


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);              //初始化PB8,PB9


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);              //初始化PB8,PB9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);              //初始化PB8,PB9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);              //初始化PB8,PB9

	TIM_TimeBaseStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//初始化定时器3


	//初始化TIM3 Channel1 Channel2  PWM模式	 
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

	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC3InitStructure.TIM_Pulse = 1500;
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM3, &TIM_OC2InitStructure);  //根据T指定的参数初始化外设TIM1 4OC2

	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OC4InitStructure.TIM_Pulse = 1500;
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM3, &TIM_OC2InitStructure);  //根据T指定的参数初始化外设TIM1 4OC2

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPE使能 

	TIM_Cmd(TIM3, ENABLE);  //使能TIM14
}


