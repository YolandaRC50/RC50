#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h" 
						  	    													  
void SPI3_Init(void);			 //初始化SPI1口
void SPI3_SetSpeed(uint8_t SpeedSet); //设置SPI1速度   
uint8_t SPI3_ReadWriteByte(uint8_t TxData);//SPI1总线读写一个字节
			
#endif

