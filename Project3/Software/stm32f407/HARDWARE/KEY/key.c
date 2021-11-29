#include "key.h"
#include "delay.h" 

/**
 * @brief ������ʼ������
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOA,GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //KEY0��ӦPE3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��GPIOE3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //WK_UP��Ӧ����PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIOA0
}

/**
 * @brief ������������
 * @param  mode             0,��֧��������;1,֧��������
 * @return u8 ����ֵ 0��û���κΰ������£�1��KEY0���£�2��WK_UP����
 * @note ��Ӧ���ȼ�,KEY0>WK_UP
 */
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1; //�������ɿ���־

	if (mode)
	{
		key_up = 1; //֧������		  
	}

	if (key_up && (KEY0 == 0 || WK_UP == 1))
	{
		delay_ms(10); //ȥ���� 
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
	return 0; // �ް�������
}



















