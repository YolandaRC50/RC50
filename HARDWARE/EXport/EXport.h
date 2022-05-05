#ifndef EXPORT_H
#define EXPORT_H

#include "stm32f4xx.h"                  // Device header

#define  DI_01_S			GPIO_Pin_9
#define  DI_01_Port			GPIOD

#define  DI_02_S			GPIO_Pin_8
#define  DI_02_Port			GPIOD

#define  DI_03_S			GPIO_Pin_15
#define  DI_03_Port			GPIOB

#define  DI_04_S			GPIO_Pin_14
#define  DI_04_Port			GPIOB

#define  DI_05_S			GPIO_Pin_13
#define  DI_05_Port			GPIOB

#define  DI_06_S			GPIO_Pin_12
#define  DI_06_Port			GPIOB

#define  DI_07_S			GPIO_Pin_15
#define  DI_07_Port			GPIOF

#define  DI_08_S			GPIO_Pin_14
#define  DI_08_Port			GPIOF

#define  DI_09_S			GPIO_Pin_13
#define  DI_09_Port			GPIOF

#define  DI_10_S			GPIO_Pin_12
#define  DI_10_Port			GPIOF

#define  DI_11_S			GPIO_Pin_11
#define  DI_11_Port			GPIOF

#define  DI_12_S			GPIO_Pin_2
#define  DI_12_Port			GPIOB

#define  DI_13_S			GPIO_Pin_1
#define  DI_13_Port			GPIOB

#define  DI_14_S			GPIO_Pin_0
#define  DI_14_Port			GPIOB

#define  DI_15_S			GPIO_Pin_5
#define  DI_15_Port			GPIOC

#define  DI_16_S			GPIO_Pin_4
#define  DI_16_Port			GPIOC


#define  DI_17_S			GPIO_Pin_0
#define  DI_17_Port			GPIOC

#define  DI_18_S			GPIO_Pin_2
#define  DI_18_Port			GPIOC
//**********************************************************************

#define	 DO_01_S			GPIO_Pin_8
#define  DO_01_Port		GPIOC

#define	 DO_02_S			GPIO_Pin_7
#define  DO_02_Port		GPIOC

#define	 DO_03_S			GPIO_Pin_6
#define  DO_03_Port		GPIOC

#define	 DO_04_S			GPIO_Pin_8
#define  DO_04_Port		GPIOG

#define	 DO_05_S			GPIO_Pin_7
#define  DO_05_Port		GPIOG

#define	 DO_06_S			GPIO_Pin_6
#define  DO_06_Port		GPIOG

#define	 DO_07_S			GPIO_Pin_5
#define  DO_07_Port		GPIOG

#define	 DO_08_S			GPIO_Pin_4
#define  DO_08_Port		GPIOG

#define	 DO_09_S			GPIO_Pin_3
#define  DO_09_Port		GPIOG

#define	 DO_10_S			GPIO_Pin_2
#define  DO_10_Port		GPIOG

#define	 DO_11_S			GPIO_Pin_15
#define  DO_11_Port		GPIOD

#define	 DO_12_S			GPIO_Pin_14
#define  DO_12_Port		GPIOD

//#define	 DO_13_S			GPIO_Pin_12
//#define  DO_13_Port		GPIOD

//#define	 DO_14_S			GPIO_Pin_13
//#define  DO_14_Port		GPIOD

//#define	 DO_15_S			GPIO_Pin_10
//#define  DO_15_Port		GPIOD

//#define	 DO_16_S			GPIO_Pin_11
//#define  DO_16_Port		GPIOD

#define	 DO_13_S_L			GPIO_Pin_11
#define  DO_13_Port_L		GPIOD

#define	 DO_13_S_H			GPIO_Pin_10
#define  DO_13_Port_H		GPIOD

#define	 DO_14_S_L			GPIO_Pin_13
#define  DO_14_Port_L		GPIOD

#define	 DO_14_S_H			GPIO_Pin_12
#define  DO_14_Port_H		GPIOD

#define	 DO_15_S_L			GPIO_Pin_13
#define  DO_15_Port_L		GPIOE

#define	 DO_15_S_H			GPIO_Pin_15
#define  DO_15_Port_H		GPIOE

#define	 DO_16_S_L			GPIO_Pin_11
#define  DO_16_Port_L		GPIOE

#define	 DO_16_S_H			GPIO_Pin_12
#define  DO_16_Port_H		GPIOE

#define	 DO_H_L					GPIO_Pin_14
#define  DO_Port_H_L		GPIOE

//
#define VALVE_PRODUCE_SET() PUTIO(13,1)
#define SSR_A_SET() PUTIO(15,0)
#define SSR_B_SET() PUTIO(17,0)
#define SSR_C_SET() PUTIO(19,0)

#define VALVE_PRODUCE_RESET() PUTIO(13,0)
#define SSR_A_RESET() PUTIO(15,1)
#define SSR_B_RESET() PUTIO(17,1)
#define SSR_C_RESET() PUTIO(19,1)

#define READ_VALVE_PRODUCE_STATUS() !ReadOutStatus(13)
#define READ_SSR_A_STATUS() ReadOutStatus(15)
#define READ_SSR_B_STATUS() ReadOutStatus(17)
#define READ_SSR_C_STATUS() ReadOutStatus(19)
#define READ_EXHAUST_SWITCH_STATUS() ReadIO(17)

typedef struct EXport
{
	GPIO_TypeDef* GPIOx; 
	uint16_t GPIO_Pin;
}EXport;
 
void PUTIO(uint16_t ch, uint8_t status);
void EXPort_Init(void);
 uint8_t ReadIO(uint8_t ch);
void EXTIX_Init(void);
uint8_t ReadOutStatus(uint8_t ch);
#endif
