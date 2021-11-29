#include <stdlib.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "motion.h"
#include "control.h"
#include "led.h"
#include "key.h"


/**
 * @brief 硬件初始化
 */
void Board_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168); //初始化延时函数
	uart_init(115200); //初始化串口波特率为115200
	Uart3_init(521600); //初始化通信串口波特率为521600
	Led_Init(); //初始化LED
	KEY_Init(); //初始化按键
	Motion_Init(200 - 1, 8400 - 1); //84M/4200=20000hz的计数频率,重装载值400，所以PWM频率为 20000/400=50hz
	MPU9250_Init();//初始化MPU9250
	mpu_dmp_init();//初始化MPU9250 DMP
}

int main(void)
{
	u8 t = 0;
	float pitch, roll, yaw; //欧拉角
	Board_init();

	while (1)
	{
		if (mpu_mpl_get_data(&pitch, &roll, &yaw) == 0)
		{
			if ((t % 10) == 0)
			{
				printf("%.2f,%.2f,%.2f\r\n", roll, yaw, pitch);
				t = 0;
			}
			t++;
		}
	}

	return 0;
}


































// void  str_cut(char *str,unsigned int resf[3]);

// int main(void)
// { 
// 	unsigned int resf[3];
// 	char temp[20];
// 	int mode=3,speed,dir; 
// 	u8 t;
// 	u8 len;
// 	u8 init_flag=1;

// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
// 	delay_init(168);  //初始化延时函数
// 	uart_init(115200);//初始化串口波特率为115200
// 	Led_Init();
// 	Motion_Init(200-1,8400-1);//84M/4200=20000hz的计数频率,重装载值400，所以PWM频率为 20000/400=50hz.

//    while(1)
// 	{
// 		/*
// 		if(init_flag)
// 		{
// 			printf("init");
// 			Motion_Stop();
// 			init_flag=0;
// 		}
// 			*/
// 		if(USART_RX_STA&0x8000)
// 		{					   
// 			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
// 			for(t=0;t<len;t++)
// 			{
// 				temp[t]=USART_RX_BUF[t];
// 				//USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
// 				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
// 			}
// 			USART_RX_STA=0;

// 			str_cut(temp,resf);
// 			mode=resf[0];
// 			dir=resf[1];
// 			speed=resf[2];
// 			printf("%d,%d,%d",mode,dir,speed);
// 		}
// 		//mode=3停止
// 		//dir=0 后退 dir=1 前进
// 		if(mode==1)
// 		{
// 			Motion_Forward(speed,dir);
// 		}
// 		//dir=0左转  dir=1右转
// 		else if(mode==0)
// 		{
// 			Motion_Turn(dir, speed);
// 		}
// 		else if(mode==3)
// 		{
// 			Motion_Stop();
// 		}
// 		LED=!LED;
// 	}
// }


// void  str_cut(char *str,unsigned int resf[3])
// {
//     char *kni;          //切割指针
//     char mode[6],dir[6],par[6];//三个参数
//     unsigned int  modef,dirf,parf;//转整形
//     //unsigned int  stor[3];//储存数组

//     int num=0;
//     kni=str;
//     //printf("test=%c\n",*kni);

//     kni++;
//     while(*kni!='!')        //找模式
//     {
//         mode[num]=*kni;
//         kni++;
//         num++;

//     }
//     mode[num]='\0';
//     num=0;

//     kni++;
//     while(*kni!='!')        //找参数
//     {
//         par[num]=*kni;
//         kni++;
//         num++;
//     }
//     par[num]='\0';
//     num=0;

//     kni++;
//     while(*kni!='!')       //找方向
//     {
//         dir[num]=*kni;
//         kni++;
//         num++;
//     }
//     dir[num]='\0';


//     modef=atoi(mode);        //转整形
//     dirf=atoi(dir);
//     parf=atoi(par);

//     resf[0]=modef;
//     resf[1]=dirf;
//     resf[2]=parf;

//    /*for(i=0;i<3;i++)
//     {
//         printf("%d\n",resf[i]);

//     }
// 		*/


// }

