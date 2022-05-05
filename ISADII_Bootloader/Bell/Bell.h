#ifndef BELL_H
#define BELL_H

#include "stm32f4xx.h"                  // Device header

#define BELL_Pin			GPIO_Pin_9
#define BELL_Port			GPIOA

void BELL_Init(void);
void BELL_Work(uint8_t cmd);

#endif
