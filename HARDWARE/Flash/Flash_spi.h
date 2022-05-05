#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h" 

#define FLASH_SPIx SPI1
#define FLASH_GPIO_AF_SPIx GPIO_AF_SPI1
#define FLASH_SPI_RCC_AHBxPeriph_GPIOx RCC_AHB1Periph_GPIOB
#define FLASH_SPI_RCC_APBxPeriph_SPIx RCC_APB2Periph_SPI1
#define FLASH_SPI_GPIO_PORTx GPIOB
#define FLASH_SPI_GPIO_Pin_SCK GPIO_Pin_3
#define FLASH_SPI_GPIO_Pin_MISO GPIO_Pin_4
#define FLASH_SPI_GPIO_Pin_MOSI GPIO_Pin_5
#define FLASH_SPI_GPIO_Pin_Source_SCK GPIO_PinSource3
#define FLASH_SPI_GPIO_Pin_Source_MISO GPIO_PinSource4
#define FLASH_SPI_GPIO_Pin_Source_MOSI GPIO_PinSource5

void FLASH_SPIx_Init(void);			 //初始化SPI1口
void FLASH_SPIx_SetSpeed(uint8_t SpeedSet); //设置SPI1速度   
uint8_t FLASH_SPIx_ReadWriteByte(uint8_t TxData);//SPI1总线读写一个字节
			
#endif

