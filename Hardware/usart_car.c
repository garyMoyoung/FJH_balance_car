#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
u16 RX_DATA;
u16 RX_station;
u16 txpakage[4]={0,0,0,0};
u16 rxpakage[4]={0,0,0,0};
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel=USART1_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=0;
	nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic);
	
	USART_Cmd(USART1,ENABLE);
}
void K230_Init(void)
{
	/* GPIO端口设置 */
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA, ENABLE ); 
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );         /* 使能USART1，GPIOA时钟 */

	/* PA2 TXD2 */
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* PA3 RXD2 */
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* Usart1 NVIC 配置 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel			= USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;                            /* 抢占优先级3 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;                            /* 子优先级3 */
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;                       /* IRQ通道使能 */
	NVIC_Init( &NVIC_InitStructure );                                                       /* 根据指定的参数初始化VIC寄存器 */

	/* USART 初始化设置 */
	USART_InitStructure.USART_BaudRate		= 115200;                                /* 串口波特率 */
	USART_InitStructure.USART_WordLength		= USART_WordLength_8b;                  /* 字长为8位数据格式 */
	USART_InitStructure.USART_StopBits		= USART_StopBits_1;                     /* 一个停止位 */
	USART_InitStructure.USART_Parity		= USART_Parity_No;                      /* 无奇偶校验位 */
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;       /* 无硬件数据流控制 */
	USART_InitStructure.USART_Mode			= USART_Mode_Rx | USART_Mode_Tx;        /* 收发模式 */

	USART_Init(USART2,&USART_InitStructure);                                             /* 初始化串口1 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);                                        /* 开启串口接受中断 */
	USART_Cmd(USART2,ENABLE);                                                            /* 使能串口 2 */
}

// 串口2发送字符串函数
void Serial2_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        USART_SendData(USART2, String[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

void Serial_SendByte(uint16_t Byte)
{
	USART_SendData(USART1, Byte);//?BYTE?????
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//??????1
}
void Serial_SendArray(uint16_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}




uint8_t Serial_GetRxFlag(void)
{
	if (RX_station == 1)
	{
		RX_station = 0;
		return 1;
	}
	return 0;
}

u16 Serial_GetRxData(void)
{
	return RX_DATA;
}


void DATApakage(void)
{
	Serial_SendByte(0XFF);
	Serial_SendArray(rxpakage,4);
	Serial_SendByte(0xFE);
}

//void USART1_IRQHandler()
//{
//	static u16 RXSTATE;
//	static u16 pRX;
//	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
//	{
//		RX_DATA=USART_ReceiveData(USART1);
//		
//		if(RXSTATE==0)
//		{
//			if(RX_DATA==0xff)
//			{
//				RXSTATE=1;
//				pRX=0;
//			}
//		}
//		else if(RXSTATE==1)
//		{
//			rxpakage[pRX]=RX_DATA;
//			pRX++;
//			if(pRX>=4)
//			{
//				RXSTATE=2;
//			}
//		}
//		else if(RXSTATE==2)
//		{
//			if(RX_DATA==0xfe)
//			{
//				RXSTATE=0;
//				pRX=0;
//				RX_station=1;
//			}
//		}
//		
//		USART_ClearFlag(USART1,USART_FLAG_RXNE);
//	}
//}
