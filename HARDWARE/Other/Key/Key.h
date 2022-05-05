#ifndef KEY_H
#define KEY_H

#include "stm32f4xx.h"                  // Device header


#define KEY_Pin1		GPIO_Pin_11
#define KEY_Pin2		GPIO_Pin_10

#define KEY_Port		GPIOG

void KEY_Init(void);
void KEY_EXTI_Init(void);

#endif
