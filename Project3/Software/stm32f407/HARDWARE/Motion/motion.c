/**
 * @file motion.c
 * @brief
 * @author
 * @version 1.0
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 * @par �޸���־:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-11-29 <td>1.0     <td>wangh     <td>Content
 * </table>
 *
 * @note �ڱ������ж��壺
 * PC6��PC7�������֣�PC8��PC9��������
 * #ȡPC6Ϊǰ������
 * PC6:TIM_SetCompare1
 * PC7:TIM_SetCompare2
 * #ȡPC8Ϊǰ������
 * PC8:TIM_SetCompare3
 * PC9:TIM_SetCompare4
 */
#include "motion.h"

 /**
  * @brief �������г�ʼ������
  * @param  arr              ARR�Ĵ���ֵ
  * @param  psc              ��ʱ��Ԥ��Ƶ��ֵ
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
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
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


	//��ʼ��TIM3 Channel1 Channel2 Channel3 Channel4  PWMģʽ	 
	TIM_OC1InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC1InitStructure.TIM_Pulse = 1500;
	TIM_OC1InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM3, &TIM_OC1InitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1


	TIM_OC2InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC2InitStructure.TIM_Pulse = 1500;
	TIM_OC2InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM3, &TIM_OC2InitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC2

	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC3InitStructure.TIM_Pulse = 1500;
	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC3Init(TIM3, &TIM_OC2InitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC3

	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OC4InitStructure.TIM_Pulse = 1500;
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM3, &TIM_OC2InitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC4

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPEʹ�� 

	TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief ֱ�к���
 * @param  speed            ֱ���ٶ�
 * @param  dira             ֱ�з���,0������;1��ǰ��
 * @note ʹС������ֱ��
 * PC6��PC8Ϊ�����������
 * PC7��PC9Ϊ�����������
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
 * @brief ת����
 * @param  dirt             ��ת����,0:��ת;1:��ת
 * @param  test_sp          ����ת���ٶȽӿڣ�ʵ�����и�Ĭ��ֵ
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
 * @brief ����ֹͣ����
 * @note ��С������ֹͣ
 */
void Motion_Stop(void)
{
	TIM_SetCompare1(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);
	TIM_SetCompare3(TIM3, 0);
	TIM_SetCompare4(TIM3, 0);
}
