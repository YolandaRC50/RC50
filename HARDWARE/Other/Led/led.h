#ifndef LED_H
#define LED_H

	    
#include "stm32f4xx.h"                    // Device header
#include "sys.h"

#define LED_Pin_ALM			GPIO_Pin_9
#define LED_Pin_RUN			GPIO_Pin_10
#define LED_Port			GPIOF
#define LED_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOF

void LED_Init(void);
void LED_Work(uint8_t cmd);

#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1

//bluetooth 

//bluetooth 
#define BLUETOOTH_KEY  PFout(6)	// DS1
//#define BLUETOOTH_KEY  PCin(0)	// 

#define BLUETOOTH_AT    PGout(6)	// DS1
#define BLUETOOTH_REST  PGout(8)	// 



void Bluetoorh_io_init(void);
#endif
