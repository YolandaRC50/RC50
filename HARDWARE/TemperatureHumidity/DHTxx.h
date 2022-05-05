#ifndef __DHTxx_H
#define __DHTxx_H 
#include "Includes.h"
#include "stm32f4xx.h" 

#define DHTxx_RCC_AHB1Periph_GPIOx RCC_AHB1Periph_GPIOG
#define DHTxx_GPIOx_Port GPIOG
#define DHTxx_WIRE_GPIO_Pin GPIO_Pin_9

 
 

//IO方向设置
#define DHTxx_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}	//PG9输入模式
#define DHTxx_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;} 	//PG9输出模式 
////IO操作函数										 
#define DHTxx_OUT_SET GPIO_SetBits(DHTxx_GPIOx_Port, DHTxx_WIRE_GPIO_Pin) 
#define DHTxx_OUT_RESET GPIO_ResetBits(DHTxx_GPIOx_Port, DHTxx_WIRE_GPIO_Pin) 
#define	DHTxx_DQ_IN  GPIO_ReadInputDataBit(DHTxx_GPIOx_Port,DHTxx_WIRE_GPIO_Pin)  //数据端口	PG9 

uint8_t DHTxx_Init(void);//初始化DHTxx
uint8_t DHTxx_Read_Data(void);//读取温湿度
uint8_t DHTxx_Read_Byte(void);//读出一个字节
uint8_t DHTxx_Read_Bit(void);//读出一个位
uint8_t DHTxx_Check(void);//检测是否存在DHTxx
void DHTxx_Rst(void);//复位DHTxx    
#endif