#include "Key.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = KEY_Pin1 | KEY_Pin2; //对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_Init(KEY_Port, &GPIO_InitStructure);//初始化GPIOE2,3
}

void KEY_EXTI_Init(void)
{	
	//KEY_Init(); //按键对应的IO口初始化
}

