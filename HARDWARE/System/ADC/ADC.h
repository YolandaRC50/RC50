#ifndef ADC_H
#define ADC_H
#include "includes.h"
#include "stm32f4xx.h"                  // Device header

#define ADCx                     ADC3
#define ADC_CHANNEL              ADC_Channel_4
#define ADC_CHANNEL1             ADC_Channel_5
#define ADC_CHANNEL2             ADC_Channel_6
#define ADC_CHANNEL3             ADC_Channel_7
#define ADCx_CLK                 RCC_APB2Periph_ADC3
#define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
#define GPIO_PIN                 GPIO_Pin_6
#define GPIO_PIN1                GPIO_Pin_7
#define GPIO_PIN2                GPIO_Pin_8
#define GPIO_PIN3                GPIO_Pin_9
#define GPIO_PORT                GPIOF
#define DMA_CHANNELx             DMA_Channel_2
#define DMA_STREAMx              DMA2_Stream0
#define DMA_STREAMx_IRQN         DMA2_Stream0_IRQn

#define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)
	
extern float ADC_S[4];
void ADC_Config(void);
void DMA_Configuration(void);
void ADC3_Configuration(void);
void GPIO_Configuration(void);
u16 GetVolt(u16 advalue) ;
void filter(void);
#endif
