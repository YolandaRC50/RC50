#ifndef I2C_ADC_H
#define I2C_ADC_H

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

void I2C_ADC_Init(void);
void I2C_ADC_Start(void);
void I2C_ADC_Stop(void);
void I2C_ADC_Send_Byte(uint16_t byte);
uint16_t I2C_ADC_Read_Byte(void);
uint8_t I2C_ADC_Wait_Ack(void);
void SDA_OUT(void);
void SDA_IN(void);

#endif
