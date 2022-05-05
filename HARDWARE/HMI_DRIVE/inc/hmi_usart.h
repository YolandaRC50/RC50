#ifndef __HMI_USART_H
#define __HMI_USART_H  
	    
#include "includes.h" 

#define HMI_USARTx USART1
#define HMI_USARTx_IRQHandler USART1_IRQHandler
#define HMI_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOA
#define HMI_RCC_APB2Periph_USARTx RCC_APB2Periph_USART1
#define HMI_GPIO_AF_USARTx GPIO_AF_USART1
#define HMI_GPIOx GPIOA
#define HMI_GPIO_PinSource_TX GPIO_PinSource9
#define HMI_GPIO_PinSource_RX GPIO_PinSource10
#define HMI_GPIO_Pin_TX GPIO_Pin_9
#define HMI_GPIO_Pin_RX GPIO_Pin_10
#define HMI_USARTx_IRQn   USART1_IRQn
#define HMI_BOUND 115200

void HMI_USART_Init();
void HMI_SendSingleChar(uint8_t dat);
#endif