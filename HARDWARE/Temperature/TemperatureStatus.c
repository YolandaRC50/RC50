#include "TemperatureStatus.h"

 

void Temperature_Status_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB, ENABLE);
 
	//DI
	
	GPIO_InitStructure.GPIO_Pin = TEMPERATURE_ALARM_STATUS_01|TEMPERATURE_ALARM_STATUS_02;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(TEMPERATURE_ALARM_DI_PORT, &GPIO_InitStructure);
}
uint8_t Temperature_Pump_Alarm_Check(void)
{
	uint8_t Motor_Controller_Alm=GPIO_ReadInputDataBit(TEMPERATURE_ALARM_DI_PORT,TEMPERATURE_ALARM_STATUS_02); 
	 
	if(Motor_Controller_Alm==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
uint8_t Temperature_Cartridge_Alarm_Check(void)
{
	uint8_t Motor_Controller_Alm=GPIO_ReadInputDataBit(TEMPERATURE_ALARM_DI_PORT,TEMPERATURE_ALARM_STATUS_01); 
	if(Motor_Controller_Alm==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
