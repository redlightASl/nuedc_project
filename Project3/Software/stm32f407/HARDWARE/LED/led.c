#include "led.h"

//初始化PA6和PA7为输出口.并使能这两个口的时钟		    
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//LED0和LED1对应IO口
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
}






