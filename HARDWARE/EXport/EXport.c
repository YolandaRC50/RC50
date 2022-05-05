#include "EXport.h"
#include "includes.h"  
#include "StatusAndAlm.h"


EXport EXport_IN[19] = 
{
	{DI_01_Port, DI_01_S},//0
	{DI_02_Port, DI_02_S},//1
	{DI_03_Port, DI_03_S},//2
	{DI_04_Port, DI_04_S},//3
	{DI_05_Port, DI_05_S},//4
	{DI_06_Port, DI_06_S},//5
	{DI_07_Port, DI_07_S},//6    Z   PF15
	{DI_08_Port, DI_08_S},//7    Potential-AI----DispensingFinished
	{DI_09_Port, DI_09_S},//8    Reset Alm
	{DI_10_Port, DI_10_S},//9
	{DI_11_Port, DI_11_S},//10   Y   PF11
	{DI_12_Port, DI_12_S},//11   X   PB2
	{DI_13_Port, DI_13_S},//12  
	{DI_14_Port, DI_14_S},//13
	{DI_15_Port, DI_15_S},//14
	{DI_16_Port, DI_16_S},//15 
	{DI_17_Port, DI_17_S},//16 
	{DI_18_Port, DI_18_S},//17 //排气开关
	{DO_Port_H_L, DO_H_L},
};

EXport EXport_OUT[20] = 
{
	{DO_01_Port, DO_01_S},
	{DO_02_Port, DO_02_S},
	{DO_03_Port, DO_03_S},
	{DO_04_Port, DO_04_S},
	{DO_05_Port, DO_05_S},
	{DO_06_Port, DO_06_S},
	{DO_07_Port, DO_07_S},
	{DO_08_Port, DO_08_S},
	{DO_09_Port, DO_09_S},
	{DO_10_Port, DO_10_S},//9   Refill_tank
	{DO_11_Port, DO_11_S},
	{DO_12_Port, DO_12_S},
//	{DO_13_Port, DO_13_S},
//	{DO_14_Port, DO_14_S},
//	{DO_15_Port, DO_15_S},
//	{DO_16_Port, DO_16_S},
	{DO_13_Port_L, DO_13_S_L},//12
	{DO_13_Port_H, DO_13_S_H},//13
	{DO_14_Port_L, DO_14_S_L},//14
	{DO_14_Port_H, DO_14_S_H},//15
	{DO_15_Port_L, DO_15_S_L},//16
	{DO_15_Port_H, DO_15_S_H},//17
	{DO_16_Port_L, DO_16_S_L},//18
	{DO_16_Port_H, DO_16_S_H},//19
};

void EXPort_in_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD
	| RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	
	for(uint8_t i = 0; i < 19; i++)
	{
		if(i==17)
		{
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;// GPIO_PuPd_DOWN;//
			//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
		}
		GPIO_InitStructure.GPIO_Pin = EXport_IN[i].GPIO_Pin;
		GPIO_Init(EXport_IN[i].GPIOx, &GPIO_InitStructure);
	}
}

void EXPort_out_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD
	| RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_NOPULL;
	for(uint8_t i = 0; i < 20; i++)
	{
		GPIO_InitStructure.GPIO_Pin = EXport_OUT[i].GPIO_Pin;
		GPIO_Init(EXport_OUT[i].GPIOx, &GPIO_InitStructure);
	}
}

void EXPort_Init(void)
{
//	EXPort_in_Init();
//	EXPort_out_Init();
//	EXTIX_Init();
}

uint8_t ReadIO(uint8_t ch)
{
	return GPIO_ReadInputDataBit(EXport_IN[ch].GPIOx, EXport_IN[ch].GPIO_Pin);
}

uint8_t ReadOutStatus(uint8_t ch)
{
	//return GPIO_ReadInputDataBit(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
	return !GPIO_ReadOutputDataBit(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
}

void PUTIO(uint16_t ch, uint8_t status)
{
	if(ch<12)
	{
		if(status)
			GPIO_ResetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
		else
			GPIO_SetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
	}
	else if(ch>=12 && ch<20)
	{
		if(ch%2==0)
		{
			if(status)
			{	
				GPIO_SetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
				GPIO_SetBits(EXport_OUT[ch+1].GPIOx, EXport_OUT[ch+1].GPIO_Pin);
			}
			else
			{
				GPIO_SetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
				GPIO_ResetBits(EXport_OUT[ch+1].GPIOx, EXport_OUT[ch+1].GPIO_Pin);
			}
		}
		else
		{
			if(status)
			{	
				GPIO_SetBits(EXport_OUT[ch-1].GPIOx, EXport_OUT[ch-1].GPIO_Pin);
				GPIO_SetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
			}
			else
			{
				GPIO_SetBits(EXport_OUT[ch-1].GPIOx, EXport_OUT[ch-1].GPIO_Pin);
				GPIO_ResetBits(EXport_OUT[ch].GPIOx, EXport_OUT[ch].GPIO_Pin);
			}
		}
	}
 
}

 
//////////配置触发信号

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);		//PB2 	连接到     START 	X	中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);		//PF11 	连接到     START 	Y 中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource14);		//PF14 	连接到     Potential-AI----DispensingFinished
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource15);		//PF15 	连接到	 START	        Z 中断线4
	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource13);		//PF13 	连接到	 Reset_fault 中断线5
	
	
	/* 配置EXTI_Line2  START X 中断事件*/ 
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿下降沿触发																													//
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置


	/* 配置EXTI_Line 11-15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line15| EXTI_Line13| EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	//
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置


	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10 11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

	   
}





//#define EXPORT_START_Y				0x01
//#define EXPORT_START_Z				0x02	
//#define EXPORT_START_X				0x03
//#define EXPORT_STOP				·	0x04 
///获取Y/Z的触发信号
void EXTI15_10_IRQHandler(void)
{
	 //PeOUT(2) = ~PeOUT(2);
	uint32_t tmp = EXTI->PR;       //取读是那个引脚的中断 
	switch(tmp)
	{
		case EXTI_Line11:
		{
			if(EXTI_GetITStatus(EXTI_Line11) == SET)
			{ 
				
				if(GPIO_ReadInputDataBit(EXport_IN[10].GPIOx,EXport_IN[10].GPIO_Pin))
				{
					//Running_status.EXPORT_Status=4;
				}
				else
				{
					//unning_status.EXPORT_Status= 1; 
				}
	
				EXTI_ClearITPendingBit(EXTI_Line11);//清除LINE上的中断标志位 	
			}
					
			break;
		}
		case EXTI_Line13:  // Reset_fault
		{
			if(EXTI_GetITStatus(EXTI_Line13) == SET)
			{ 
				if(GPIO_ReadInputDataBit(EXport_IN[8].GPIOx,EXport_IN[8].GPIO_Pin))
				{
					 EXPORT_ALARM_RESET=0; 
				}
				else
				{
					 EXPORT_ALARM_RESET=1; 
				}
					EXTI_ClearITPendingBit(EXTI_Line13);//清除LINE上的中断标志位 	
			}
					
			break;
		}
		case EXTI_Line14:
		{
			if(EXTI_GetITStatus(EXTI_Line14) == SET)
			{ 
				if(GPIO_ReadInputDataBit(EXport_IN[7].GPIOx,EXport_IN[7].GPIO_Pin))
				{
					// DIspensingFinished_Mark=POWER_OFF; 
				}
				else
				{
					  //DIspensingFinished_Mark=POWER_ON; 
				}
					EXTI_ClearITPendingBit(EXTI_Line14);//清除LINE上的中断标志位 	
			}
					
			break;
		}
		case EXTI_Line15:
		{
			if(EXTI_GetITStatus(EXTI_Line15) == SET)
			{ 
				
				if(GPIO_ReadInputDataBit(EXport_IN[6].GPIOx,EXport_IN[6].GPIO_Pin))
				{
					//Running_status.EXPORT_Status=4; 
				}
				else
				{
					//Running_status.EXPORT_Status= 2; 
				} 
				
				EXTI_ClearITPendingBit(EXTI_Line15);//清除LINE上的中断标志位
			} 
			break;
		}
	}
}

/////////// Dispensing Start X 触发信号
void EXTI2_IRQHandler(void)
{
	
	// PEout(2) = ~PEout(2);
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{  
		
		if(GPIO_ReadInputDataBit(EXport_IN[11].GPIOx,EXport_IN[11].GPIO_Pin))
		{
			//Running_status.EXPORT_Status=4; 
		}
		else
		{
			//Running_status.EXPORT_Status= 3; 
		}
	
		EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE上的中断标志位	
	}
}
