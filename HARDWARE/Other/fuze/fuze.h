#ifndef FUZE_H
#define FUZE_H

#include "stm32f4xx.h"

#define H_ALM01_S_Pin 	GPIO_Pin_4
#define H_ALM02_S_Pin		GPIO_Pin_5

#define H_ALM01_S_Port 	GPIOA
#define H_ALM02_S_Port	GPIOA

#define P_A1_Pin		GPIO_Pin_0
#define P_A2_Pin		GPIO_Pin_1
#define P_B1_Pin		GPIO_Pin_6
#define P_B2_Pin		GPIO_Pin_7

#define P_A1_Port		GPIOA
#define P_A2_Port		GPIOA
#define P_B1_Port		GPIOA
#define P_B2_Port		GPIOA

void Fuze_Init(void);
void P_AB12_Init(void);
#endif
