#ifndef BELL_H
#define BELL_H
	    
#include "stm32f4xx.h"                   // Device header

#define BELL_Pin			GPIO_Pin_8
#define BELL_Port			GPIOF
#define BELL_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOF

void BELL_Init(void);
void BELL_Work(u8 cmd);

#endif
