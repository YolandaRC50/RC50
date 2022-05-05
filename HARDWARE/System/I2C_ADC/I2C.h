#ifndef I2CHEAD
#define I2CHEAD

#include "stm32f4xx.h"

#define I2C_PORT	GPIOB
#define SDA_Pin		GPIO_Pin_9
#define SCL_Pin		GPIO_Pin_8

#define SCL_H   	GPIO_SetBits(I2C_PORT, SCL_Pin)   //
#define SCL_L  		GPIO_ResetBits(I2C_PORT, SCL_Pin)  //

#define SDA_H  		GPIO_SetBits(I2C_PORT, SDA_Pin)   //
#define SDA_L  		GPIO_ResetBits(I2C_PORT, SDA_Pin)  //

#define READ_SDA   GPIO_ReadInputDataBit(I2C_PORT, SDA_Pin)  // ‰»ÎSDA 

void EEPI2C_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint16_t byte);
uint16_t IIC_Read_Byte(uint8_t ack);
uint8_t IIC_Wait_Ack(void);
void SDA_OUT(void);
void SDA_IN(void);

#endif
