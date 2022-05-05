#ifndef _Custom_RS232_H_
#define _Custom_RS232_H_

#include "stm32f4xx_it.h"     //根据用户MCU进行修改
#include "stm32f4xx.h"
#include "string.h"
#include "sys_config.h"

#if (1==1)  /* Custom_RS232*/
#define CUSTOM_RS232_TX_Pin GPIO_Pin_9 
#define CUSTOM_RS232_RX_Pin GPIO_Pin_10 

#define CUSTOM_RS232 USART6
#define RCC_APBxPeriph_USARTx RCC_APB2Periph_USART6
#define CUSTOM_RS232_PORT GPIOG
#define CUSTOM_RS232_BAUDRATE 19200
#define CUSTOM_RS232_IRQn USART6_IRQn
#define  CUSTOM_RS232_PORT_AHBxPeriph RCC_AHB1Periph_GPIOG

#endif




/******************************************************************************/

#if (1==1) /*驱动*/



/****************************************************************************
* 名    称： CUSTOM_RS232_Init()
* 功    能： 串口初始化 
* 出口参数： 无
****************************************************************************/
void Custom_RS232_Init(void);

/*****************************************************************
* 名    称： CUSTOM_RS232_SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
*****************************************************************/
void  Custom_RS232_SendChar(unsigned char t);

/*****************************************************************
* 名    称： USART1_IRQHandler()
* 功    能： USART1_IRQHandler接收中断
* 入口参数： 无
* 出口参数： 无
*****************************************************************/
void USART1_IRQHandler(void);

#endif


#endif

