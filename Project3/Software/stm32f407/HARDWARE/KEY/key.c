#include "key.h"
#include "delay.h" 

/**
 * @brief 按键初始化函数
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //KEY0对应PE3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化GPIOE3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //WK_UP对应引脚PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA0
}

/**
 * @brief 按键处理函数
 * @param  mode             0,不支持连续按;1,支持连续按
 * @return u8 按键值 0，没有任何按键按下；1，KEY0按下；2，WK_UP按下
 * @note 响应优先级,KEY0>WK_UP
 */
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1; //按键按松开标志

	if (mode)
	{
		key_up = 1; //支持连按		  
	}

	if (key_up && (KEY0 == 0 || WK_UP == 1))
	{
		delay_ms(10); //去抖动 
		key_up = 0;
		if (KEY0 == 0)
			return 1;
		else if (WK_UP == 1)
			return 2;
	}
	else if (KEY0 == 1 && WK_UP == 0)
	{
		key_up = 1;
	}
	return 0; // 无按键按下
}




















