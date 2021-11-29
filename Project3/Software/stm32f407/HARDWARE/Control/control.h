#ifndef __CONTROL_H
#define __CONTROL_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"

//UART3 PB10->Tx PB11->Rx
#define USART3_MAX_RECV 256 //最大接收字节数

#define UART_MASK_RECV_DONE 0x8000   //[15]	 接收完成标志
#define UART_MASK_RECV_D 0x4000      //[14]	 接收到0x0d
#define UART_MASK_LENGTH 0x3fff      //[13:0] 接收到的有效字节数目

extern u8  UART3_BUFFER[USART3_MAX_RECV]; //接收缓冲,末字节为换行符 
extern u16 UART3_RX_STA; //接收状态标记


void Uart3_init(u32 boundrate);
u8* Uart3_receive(void);
void Uart3_send(u8* buffer);


#endif