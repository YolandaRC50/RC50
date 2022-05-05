#ifndef EEP_I2C_HEAD
#define EEP_I2C_HEAD

#include "stm32f4xx.h" 

#define EEP_I2C_PORT	GPIOB
#define EEP_I2C_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOB
#define EEP_I2C_SDA_Pin		GPIO_Pin_9
#define EEP_I2C_SCL_Pin		GPIO_Pin_8

#define EEP_I2C_SCL_H   	GPIO_SetBits(EEP_I2C_PORT, EEP_I2C_SCL_Pin)   //
#define EEP_I2C_SCL_L  		GPIO_ResetBits(EEP_I2C_PORT, EEP_I2C_SCL_Pin)  //

#define EEP_I2C_SDA_H  		GPIO_SetBits(EEP_I2C_PORT, EEP_I2C_SDA_Pin)   //
#define SEEP_I2C_SDA_L  		GPIO_ResetBits(EEP_I2C_PORT, EEP_I2C_SDA_Pin)  //

#define EEP_I2C_READ_SDA   GPIO_ReadInputDataBit(EEP_I2C_PORT, EEP_I2C_SDA_Pin)  // ‰»ÎSDA 

void EEP_I2C_Init(void);
void EEP_I2C_Start(void);
void EEP_I2C_Stop(void);
void EEP_I2C_Send_Byte(uint16_t byte);
uint16_t EEP_I2C_Read_Byte(uint8_t ack);
uint8_t EEP_I2C_Wait_Ack(void);
void EEP_I2C_SDA_OUT(void);
void EEP_I2C_SDA_IN(void);

#endif
