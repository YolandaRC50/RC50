#ifndef ADS1115HEAD
#define ADS1115HEAD

#include "stm32f4xx.h"

#define I2C_ADC_SCL_PORT	GPIOG
#define I2C_ADC_SDA_PORT	GPIOE
#define I2C_ADC_SDA_Pin		GPIO_Pin_7
#define I2C_ADC_SCL_Pin		GPIO_Pin_1

#define I2C_ADC_SCL_H   	GPIO_SetBits(I2C_ADC_SCL_PORT, I2C_ADC_SCL_Pin)   //
#define I2C_ADC_SCL_L  		GPIO_ResetBits(I2C_ADC_SCL_PORT, I2C_ADC_SCL_Pin)  //

#define I2C_ADC_SDA_H  		GPIO_SetBits(I2C_ADC_SDA_PORT, I2C_ADC_SDA_Pin)   //
#define I2C_ADC_SDA_L  		GPIO_ResetBits(I2C_ADC_SDA_PORT, I2C_ADC_SDA_Pin)  //

#define I2C_ADC_READ_SDA   GPIO_ReadInputDataBit(I2C_ADC_SDA_PORT, I2C_ADC_SDA_Pin)  // ‰»ÎSDA 


void Confige_1115(unsigned char channel);
void Point_reg(void);
uint16_t Read_1115(void);
void ADS1115_Init(void);
uint16_t Get_AD(uint8_t channel);
float ADS1115_Get_channelRes(uint8_t channel);

#endif
