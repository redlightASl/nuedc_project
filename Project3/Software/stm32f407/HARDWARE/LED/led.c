#include "led.h"

//��ʼ��PA6��PA7Ϊ�����.��ʹ���������ڵ�ʱ��		    
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOF, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//LED0��LED1��ӦIO��
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
}






