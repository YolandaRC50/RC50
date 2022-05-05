#ifndef _BSP_DAC8562_H
#define _BSP_DAC8562_H

#include "stm32f4xx.h"                  // Device header
#include "sys_config.h"

/* 定义GPIO端口 */
#define A_RCC_SCLK 	RCC_AHB1Periph_GPIOE
#define A_PORT_SCLK	GPIOE
#define A_PIN_SCLK	GPIO_Pin_6

#define A_RCC_DIN 	RCC_AHB1Periph_GPIOE
#define A_PORT_DIN 	GPIOE
#define A_PIN_DIN 	GPIO_Pin_5

/* 片选 */
#define A_RCC_SYNC 	RCC_AHB1Periph_GPIOE
#define A_PORT_SYNC	GPIOE
#define A_PIN_SYNC	GPIO_Pin_3

/* LDAC, 可以不用 */
#define A_RCC_LDAC	RCC_AHB1Periph_GPIOE
#define A_PORT_LDAC	GPIOE
#define A_PIN_LDAC	GPIO_Pin_4

/* CLR, 可以不用 */
#define A_RCC_CLR 	RCC_AHB1Periph_GPIOE
#define A_PORT_CLR	GPIOE
#define A_PIN_CLR 	GPIO_Pin_2

 

/* 定义口线置0和置1的宏 */
#define A_SYNC_0()	A_PORT_SYNC->BSRRH = A_PIN_SYNC
#define A_SYNC_1()	A_PORT_SYNC->BSRRL = A_PIN_SYNC

#define A_SCLK_0()	A_PORT_SCLK->BSRRH = A_PIN_SCLK
#define A_SCLK_1()	A_PORT_SCLK->BSRRL = A_PIN_SCLK

#define A_DIN_0()		A_PORT_DIN->BSRRH = A_PIN_DIN
#define A_DIN_1()		A_PORT_DIN->BSRRL = A_PIN_DIN

#define A_LDAC_0()	A_PORT_LDAC->BSRRH = A_PIN_LDAC
#define A_LDAC_1()	A_PORT_LDAC->BSRRL = A_PIN_LDAC

#define A_CLR_0()		A_PORT_CLR->BSRRH = A_PIN_CLR
#define A_CLR_1()		A_PORT_CLR->BSRRL = A_PIN_CLR	
 

/* 定义GPIO端口 */
#define B_RCC_SCLK 	RCC_AHB1Periph_GPIOF
#define B_PORT_SCLK	GPIOF
#define B_PIN_SCLK	GPIO_Pin_3

#define B_RCC_DIN 	RCC_AHB1Periph_GPIOF
#define B_PORT_DIN 	GPIOF
#define B_PIN_DIN 	GPIO_Pin_4

/* 片选 */
#define B_RCC_SYNC 	RCC_AHB1Periph_GPIOF
#define B_PORT_SYNC	GPIOF
#define B_PIN_SYNC	GPIO_Pin_2

/* LDAC, 可以不用 */
#define B_RCC_LDAC	RCC_AHB1Periph_GPIOF
#define B_PORT_LDAC	GPIOF
#define B_PIN_LDAC	GPIO_Pin_1

/* CLR, 可以不用 */
#define B_RCC_CLR 	RCC_AHB1Periph_GPIOF
#define B_PORT_CLR	GPIOF
#define B_PIN_CLR 	GPIO_Pin_0

/* 定义口线置0和置1的宏 */
#define B_SYNC_0()	B_PORT_SYNC->BSRRH = B_PIN_SYNC
#define B_SYNC_1()	B_PORT_SYNC->BSRRL = B_PIN_SYNC

#define B_SCLK_0()	B_PORT_SCLK->BSRRH = B_PIN_SCLK
#define B_SCLK_1()	B_PORT_SCLK->BSRRL = B_PIN_SCLK

#define B_DIN_0()		B_PORT_DIN->BSRRH = B_PIN_DIN
#define B_DIN_1()		B_PORT_DIN->BSRRL = B_PIN_DIN

#define B_LDAC_0()	B_PORT_LDAC->BSRRH = B_PIN_LDAC
#define B_LDAC_1()	B_PORT_LDAC->BSRRL = B_PIN_LDAC

#define B_CLR_0()		B_PORT_CLR->BSRRH = B_PIN_CLR
#define B_CLR_1()		B_PORT_CLR->BSRRL = B_PIN_CLR	




void bsp_InitDAC8562(void);
void DAC8562_Reset(void);
void DAC8562_SetData(uint8_t address, uint8_t _ch,uint16_t _dac,uint8_t reset8562); 
void DAC8562_WriteCmd(uint32_t _cmd, uint8_t address);

#endif

