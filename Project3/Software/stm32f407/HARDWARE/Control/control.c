#include "control.h"

u8  UART3_BUFFER[USART3_MAX_RECV];
u16 UART3_RX_STA = 0;

/**
 * @brief 初始化串口
 * @param  boundrate        波特率
 */
void Uart3_init(u32 boundrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //PB10 PB11复用
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //PB10 PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10 PB11

    USART_InitStructure.USART_BaudRate = boundrate; //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC);
    // USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

    // NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //串口1中断通道
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
    // NVIC_Init(&NVIC_InitStructure);
}

// /**
//  * @brief 串口3中断服务程序
//  */
// void USART3_IRQHandler(void)
// {

// }

/**
 * @brief 接收串口数据
 * @return u8* 接收缓冲区
 */
u8* Uart3_receive(void)
{
    u8 Res;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART1); //读取接收到的数据

        if ((UART3_RX_STA & UART_MASK_RECV_DONE) == 0) //接收未完成
        {
            if (UART3_RX_STA & UART_MASK_RECV_D) //接收到了0x0d
            {
                if (Res != 0x0a)
                {
                    UART3_RX_STA = 0; //接收错误,重新开始
                }
                else
                {
                    UART3_RX_STA |= UART_MASK_RECV_DONE; //接收完成了
                }
            }
            else //还没收到0X0D
            {
                if (Res == 0x0d)
                {
                    UART3_RX_STA |= UART_MASK_RECV_D;
                }
                else
                {
                    UART3_BUFFER[UART3_RX_STA & UART_MASK_LENGTH] = Res;
                    UART3_RX_STA++;
                    if (UART3_RX_STA > (USART3_MAX_RECV - 1))
                    {
                        UART3_RX_STA = 0;//接收数据错误,重新开始接收
                    }
                }
            }
        }
    }
}

/**
 * @brief 发送串口数据
 * @param  buffer           发送缓冲区
 */
void Uart3_send(u8* buffer)
{
    while (*buffer != '\0')
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        {
            USART_SendData(USART3, *buffer);
            buffer++;
        }
    }
}

