#include "Bell.h"


void BELL_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BELL_RCC_AHB1Periph_GPIOx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BELL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(BELL_Port, &GPIO_InitStructure);
	
	
  GPIO_ResetBits(BELL_Port,BELL_Pin);  //蜂鸣器对应引脚GPIOF8拉低， 
}

void BELL_Work(uint8_t cmd)
{ 
	return;
	if(cmd)
		GPIO_SetBits(BELL_Port, BELL_Pin);
	else
		GPIO_ResetBits(BELL_Port, BELL_Pin);
}
