#ifndef _PRESSURE_H_
#define _PRESSURE_H_
#include "stm32f4xx.h"                  // Device header

//GPIO
 #if(1)
 
 #endif
//ADC

 #if(1)
  
	#define SENSOR_AN_IN_RCCx 						RCC_AHB1Periph_GPIOA
	#define SENSOR_AN_IN_PORTx 						GPIOA
	#define SENSOR_AN_IN_GOIP_PINx_0 			GPIO_Pin_5
	//#define SENSOR_AN_IN_GOIP_PINx_1 			GPIO_Pin_1
	#define SENSOR_AN_IN_ADCx_RCCx 				RCC_APB2Periph_ADC1
	#define SENSOR_AN_IN_ADCx 						ADC1
	#define SENSOR_AN_IN_ADCx_Channelx_0 ADC_Channel_5
	//#define SENSOR_AN_IN_ADCx_Channelx_1 ADC_Channel_1
	#define ADCx_DR_ADDRESS          		(uint32_t)(&ADC1->DR)//((uint32_t)0x4001224C)
 #endif
//DAC

 #if(1)
 	#define SENSOR_AN_O_RCCx 							RCC_AHB1Periph_GPIOA
	#define SENSOR_AN_O_PORTx 						GPIOA
	#define SENSOR_AN_O_GOIP_PINx_0 			GPIO_Pin_4
	//#define SENSOR_AN_O_GOIP_PINx_1 			GPIO_Pin_3
	
	//
	#define SENSOR_AN_O_DAC_RCCx RCC_APB1Periph_DAC
	#define SENSOR_AN_DCA_Channel_0 DAC_Channel_1
	//#define SENSOR_AN_DCA_Channel_1 DAC_Channel_2
	#define SENSOR_AN_DCA_SetChannel0Data DAC_SetChannel1Data
	//#define SENSOR_AN_DCA_SetChannel1Data DAC_SetChannel2Data
 #endif
//DMA

 #if(1)
#define DMA_CHANNELx             DMA_Channel_0
#define DMA_STREAMx              DMA2_Stream0
#define DMA_STREAMx_IRQN         DMA2_Stream0_IRQn
#define RCC_DMAx 								 RCC_AHB1Periph_DMA2
#define DMA_STREAMx_IRQHandler 	 DMA2_Stream0_IRQHandler
 #endif


//extern float Pressure_RT[2];
void PressureSensor_Init(void); 
void  SetPressureSensorTargetValue();
void Enable_PressureSensor_Auto_Detect(void);
#endif
