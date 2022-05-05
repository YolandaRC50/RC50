#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx.h"                  // Device header
#define TIMx TIM3
#define RCC_APBxPeriph_TIMx RCC_APB1Periph_TIM3
#define RCC_APBxPeriphClockCmd RCC_APB1PeriphClockCmd
#define ARR 100-1
#define PSC 8400-1
#define TIMx_IRQn TIM3_IRQn
#define TIMx_IRQHandler TIM3_IRQHandler

void TIMx_Init(void);
void TIM4_Init(void);
//void TIMx_IRQHandler_1MS(void);
void TIMx_IRQHandler_10MS(void);
void TIM4_IRQHandler_10MS(void);
void TIMx_IRQHandler_100MS(void);
void TIMx_IRQHandler_1S(void);
#endif