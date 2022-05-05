#ifndef DS1302_H
#define DS1302_H

#include "stm32f4xx.h"                  // Device header

#define DS1302_CLK_Pin		GPIO_Pin_11
#define DS1302_IO_Pin			GPIO_Pin_10
#define DS1302_RST_Pin		GPIO_Pin_15

#define DS1302_Port1			GPIOC
#define DS1302_Port2			GPIOA

#define DS1302_IO_H				GPIO_SetBits(DS1302_Port1, DS1302_IO_Pin)
#define DS1302_IO_L				GPIO_ResetBits(DS1302_Port1, DS1302_IO_Pin)

#define DS1302_CLK_H			GPIO_SetBits(DS1302_Port1, DS1302_CLK_Pin)
#define DS1302_CLK_L			GPIO_ResetBits(DS1302_Port1, DS1302_CLK_Pin)

#define DS1302_RST_H			GPIO_SetBits(DS1302_Port2, DS1302_RST_Pin)
#define DS1302_RST_L			GPIO_ResetBits(DS1302_Port2, DS1302_RST_Pin)

#define READ_1302IO				GPIO_ReadInputDataBit(DS1302_Port1, DS1302_IO_Pin)

extern unsigned char second,minute,hour,week,day,month,year;   
extern unsigned char time_date_dis[11];
extern unsigned char time_hms_dis[9];
void Ds1302_Init(void);
void read_time(void);
#endif
