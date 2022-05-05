#ifndef _TEMPERATURESTATUS_H_
#define _TEMPERATURESTATUS_H_


#include "stm32f4xx.h"                  // Device header

//数字输入功能	使能，顺时针使能，逆时针使能，顺时针+逆时针使能，使能+旋向，停止，PWM设定值，RC Servo设定值，固定设定值
//模拟输入功能	设定值，电流极限，偏移，变速
//数字输出功能	就绪，转速检定仪，电流检定仪，换向频率
//模拟输出功能	电流监控器，转速监控器，温度，固定值



//数字输入
#define TEMPERATURE_ALARM_DI_PORT		GPIOA
#define TEMPERATURE_ALARM_STATUS_01		GPIO_Pin_4
#define TEMPERATURE_ALARM_STATUS_02		GPIO_Pin_5


void Temperature_Status_Init(void);
uint8_t Temperature_Pump_Alarm_Check(void);
uint8_t Temperature_Cartridge_Alarm_Check(void);


#endif
