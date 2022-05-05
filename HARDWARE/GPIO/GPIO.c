
#include "gpio.h" 
POWER_STATUS  FORCE_ENABLE=POWER_OFF;
#if(1)
GPIO GPIO_IN[GP_I_LEN]=
{
	{DI_CLK_0,DI_PORT_0,DI_PIN_0,DI_PuPd_Type_0},
	{DI_CLK_1,DI_PORT_1,DI_PIN_1,DI_PuPd_Type_1},
	{DI_CLK_2,DI_PORT_2,DI_PIN_2,DI_PuPd_Type_2},
	{DI_CLK_3,DI_PORT_3,DI_PIN_3,DI_PuPd_Type_3},
	{DI_CLK_4,DI_PORT_4,DI_PIN_4,DI_PuPd_Type_4},
	{DI_CLK_5,DI_PORT_5,DI_PIN_5,DI_PuPd_Type_5}, 
	{DI_CLK_6,DI_PORT_6,DI_PIN_6,DI_PuPd_Type_6},
	{DI_CLK_7,DI_PORT_7,DI_PIN_7,DI_PuPd_Type_7},
	{DI_CLK_8,DI_PORT_8,DI_PIN_8,DI_PuPd_Type_8},
	{DI_CLK_9,DI_PORT_9,DI_PIN_9,DI_PuPd_Type_9},
	{DI_CLK_10,DI_PORT_10,DI_PIN_10,DI_PuPd_Type_10},
	{DI_CLK_11,DI_PORT_11,DI_PIN_11,DI_PuPd_Type_11}, 
};

GPIO GPIO_OUT[GP_O_LEN]=
{
	{DO_CLK_0,DO_PORT_0,DO_PIN_0,DO_PuPd_Type_0,0},
	{DO_CLK_1,DO_PORT_1,DO_PIN_1,DO_PuPd_Type_1,0},
	{DO_CLK_2,DO_PORT_2,DO_PIN_2,DO_PuPd_Type_2,0},
	{DO_CLK_3,DO_PORT_3,DO_PIN_3,DO_PuPd_Type_3,0},
	{DO_CLK_4,DO_PORT_4,DO_PIN_4,DO_PuPd_Type_4,0},
	{DO_CLK_5,DO_PORT_5,DO_PIN_5,DO_PuPd_Type_5,0},
	{DO_CLK_6,DO_PORT_6,DO_PIN_6,DO_PuPd_Type_6,0},
	{DO_CLK_7,DO_PORT_7,DO_PIN_7,DO_PuPd_Type_7,0}, 
	{DO_CLK_8,DO_PORT_8,DO_PIN_9,DO_PuPd_Type_8,0},
	{DO_CLK_9,DO_PORT_9,DO_PIN_8,DO_PuPd_Type_9,0},
//	{DO_CLK_10,DO_PORT_10,DO_PIN_10,DO_PuPd_Type_10},
//	{DO_CLK_11,DO_PORT_11,DO_PIN_11,DO_PuPd_Type_11}, 
};
DI_Exti DI_Exti_List[DI_EXTI_LEN]=
{
	{DI_FOOT_SWITCH_1_DI_Index,DI_FOOT_SWITCH_1_EXTI_PortSourceGPIOx,DI_FOOT_SWITCH_1_EXTI_PinSourcex,DI_FOOT_SWITCH_1_EXTIx_IRQn,DI_FOOT_SWITCH_1_EXTI_LineX},
	{DI_FOOT_SWITCH_2_DI_Index,DI_FOOT_SWITCH_2_EXTI_PortSourceGPIOx,DI_FOOT_SWITCH_2_EXTI_PinSourcex,DI_FOOT_SWITCH_2_EXTIx_IRQn,DI_FOOT_SWITCH_2_EXTI_LineX},
	{DI_CHANNEL_SWITCH_1_DI_Index,DI_CHANNEL_SWITCH_1_EXTI_PortSourceGPIOx,DI_CHANNEL_SWITCH_1_EXTI_PinSourcex ,DI_CHANNEL_SWITCH_1_EXTIx_IRQn ,DI_CHANNEL_SWITCH_1_EXTI_LineX },
	{DI_CHANNEL_SWITCH_2_DI_Index,DI_CHANNEL_SWITCH_2_EXTI_PortSourceGPIOx,DI_CHANNEL_SWITCH_2_EXTI_PinSourcex ,DI_CHANNEL_SWITCH_2_EXTIx_IRQn ,DI_CHANNEL_SWITCH_2_EXTI_LineX },
	{DI_PURGE_SWITCH_1_DI_Index,DI_PURGE_SWITCH_1_EXTI_PortSourceGPIOx,DI_PURGE_SWITCH_1_EXTI_PinSourcex,DI_PURGE_SWITCH_1_EXTIx_IRQn,DI_PURGE_SWITCH_1_EXTI_LineX},
	{DI_PURGE_SWITCH_2_DI_Index,DI_PURGE_SWITCH_2_EXTI_PortSourceGPIOx,DI_PURGE_SWITCH_2_EXTI_PinSourcex ,DI_PURGE_SWITCH_2_EXTIx_IRQn ,DI_PURGE_SWITCH_2_EXTI_LineX},

};
//#define DO_LOW_LEVEL_EMPTY 			2
//#define DO_LOW_LEVEL_LOWER 			3
//#define DO_CHANNEL_1_DISPSING 	4
//#define DO_CHANNEL_2_DISPSING 	5 
//#define DO_PRESSURE_ALARM 			6
//#define DO_RUN_STOP  						7
FORCE Force_List[6]=
{
	{DO_SYS_RUN,POWER_OFF},
	{DO_LOW_LEVEL_LOWER,POWER_OFF},
	{DO_CHANNEL_1_DISPSING,POWER_OFF},
	{DO_CHANNEL_2_DISPSING,POWER_OFF},
	{DO_PRESSURE_ALARM,POWER_OFF},
	{DO_PRE_ENA,POWER_OFF},
};

void GOIO_IN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	uint32_t _CLK=0;
	for(uint8_t index=0;index<GP_I_LEN;index++)
	{
		_CLK|=GPIO_IN[index].DO_CLKx;
	}
	
	RCC_AHB1PeriphClockCmd(_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	 
	for(uint8_t index = 0; index < GP_I_LEN; index++)
	{ 
		GPIO_InitStructure.GPIO_PuPd = GPIO_IN[index].GPIOPuPd_Type;	
		GPIO_InitStructure.GPIO_Pin = GPIO_IN[index].GPIO_Pinx;
		GPIO_Init(GPIO_IN[index].GPIOx, &GPIO_InitStructure);
	}
}
void GOIO_OUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t _CLK=0;
	for(uint8_t index=0;index<GP_O_LEN;index++)
	{
		_CLK|=GPIO_OUT[index].DO_CLKx;
	}
	
	RCC_AHB1PeriphClockCmd(_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	 
	for(uint8_t index = 0; index < GP_O_LEN; index++)
	{ 
		GPIO_InitStructure.GPIO_PuPd = GPIO_IN[index].GPIOPuPd_Type;	
		GPIO_InitStructure.GPIO_Pin = GPIO_OUT[index].GPIO_Pinx;
		GPIO_Init(GPIO_OUT[index].GPIOx, &GPIO_InitStructure); 
	
		GPIO_SetBits(GPIO_OUT[index].GPIOx,GPIO_OUT[index].GPIO_Pinx);  //蜂鸣器对应引脚GPIOF8拉低， 
	}
}


void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟 
	for(int i=0;i<DI_EXTI_LEN;i++)
	{
		SYSCFG_EXTILineConfig(DI_Exti_List[i].EXTI_PortSourceGPIOx,DI_Exti_List[i].EXTI_PinSourcex);
		
		EXTI_InitStructure.EXTI_Line=DI_Exti_List[i].EXTI_LineX;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //上升沿下降沿触发																													//
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
		EXTI_Init(&EXTI_InitStructure);//配置
		
			//
		NVIC_InitStructure.NVIC_IRQChannel = DI_Exti_List[i].EXTIx_IRQn;//外部中断2
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = i;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);//配置
	}  
}

void EXTI_IRQHandler(void)
{
	uint32_t EXTI_LineX = EXTI->PR;       //取读是那个引脚的中断 
	for(int i=0;i<DI_EXTI_LEN;i++)
	{
		if(EXTI_LineX==DI_Exti_List[i].EXTI_LineX)
		{ 
			if(EXTI_GetITStatus(EXTI_LineX) == SET)
			{   
				Refresh_GPIO_Value();	 
				Dispensing_Logic();
				EXTI_ClearITPendingBit(EXTI_LineX);//清除LINE上的中断标志位 
			}
		}
	} 
}
void EXTI0_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI1_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI2_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI3_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI4_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI9_5_IRQHandler(void)
{
	EXTI_IRQHandler();
}
void EXTI15_10_IRQHandler(void)
{
	EXTI_IRQHandler();
}

#endif
 

void GOIO_Init(void)
{
	GOIO_IN_Init();
	GOIO_OUT_Init();
	EXTIX_Init();
}

void Refresh_GPIO_Value()
{
	for(uint8_t index=0;index<GP_I_LEN;index++)
	{
		GPIO_IN[index].GPIO_Value=1-GPIO_ReadInputDataBit(GPIO_IN[index].GPIOx, GPIO_IN[index].GPIO_Pinx);	 
	}
	if(FORCE_ENABLE)
		{
//			#define DO_LOW_LEVEL_EMPTY_FORCE_INDEX 0 
//			#define	DO_LOW_LEVEL_LOWER_FORCE_INDEX	1
//			#define	DO_CHANNEL_1_DISPSING_FORCE_INDEX	2
//			#define	DO_CHANNEL_2_DISPSING_FORCE_INDEX	3
//			#define	DO_PRESSURE_ALARM_FORCE_INDEX	4
//			#define	DO_RUN_STOP_FORCE_INDEX	5
			GPIO_OUT[DO_SYS_RUN].GPIO_Value=Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status;
			GPIO_OUT[DO_LOW_LEVEL_LOWER].GPIO_Value=Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status;
			GPIO_OUT[DO_CHANNEL_1_DISPSING].GPIO_Value=Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status;
			GPIO_OUT[DO_CHANNEL_2_DISPSING].GPIO_Value=Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status;
			GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value=Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status;
			GPIO_OUT[DO_PRE_ENA].GPIO_Value=Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status;
		} 
	for(uint8_t index=0;index<GP_O_LEN;index++)
	{ 
		
		if(GPIO_OUT[index].GPIO_Value) 
		{
			GPIO_ResetBits(GPIO_OUT[index].GPIOx, GPIO_OUT[index].GPIO_Pinx); 
		}
		else 
		{
			GPIO_SetBits(GPIO_OUT[index].GPIOx, GPIO_OUT[index].GPIO_Pinx);
		} 
	}
 
}


void Cartridge_Level_Status(Cartridge_Status viStatus)
{
	switch(viStatus)
	{
		case	LOWER:
		{
			GPIO_OUT[DO_LOW_LEVEL_LOWER].GPIO_Value=POWER_ON;
			//GPIO_OUT[DO_LOW_LEVEL_EMPTY].GPIO_Value=POWER_OFF;
			//GPIO_OUT[DO_LOW_LEVEL_FULL].GPIO_Value=POWER_OFF;
			break;
		}
		case	EMPTY:
		{
			GPIO_OUT[DO_LOW_LEVEL_LOWER].GPIO_Value=POWER_ON;
			//GPIO_OUT[DO_LOW_LEVEL_EMPTY].GPIO_Value=POWER_ON;
			//GPIO_OUT[DO_LOW_LEVEL_FULL].GPIO_Value=POWER_OFF;
			break;
		}
		case	FULL:
		{
			GPIO_OUT[DO_LOW_LEVEL_LOWER].GPIO_Value=POWER_OFF;
			//GPIO_OUT[DO_LOW_LEVEL_EMPTY].GPIO_Value=POWER_OFF;
			//GPIO_OUT[DO_LOW_LEVEL_FULL].GPIO_Value=POWER_ON;
			break;
		}
	}
}

void Dispensing(System_Channel Chx,POWER_STATUS Power)
{
	//Power=!Power;
	if(Chx==CH1)
	{
		 GPIO_OUT[DO_CHANNEL_1_VALVE].GPIO_Value=Power;
		 GPIO_OUT[DO_CHANNEL_1_DISPSING].GPIO_Value=Power;
	}
	else
	{
		 GPIO_OUT[DO_CHANNEL_2_VALVE].GPIO_Value=Power;
		 GPIO_OUT[DO_CHANNEL_2_DISPSING].GPIO_Value=Power;
	}
}

POWER_STATUS Check_Dispensing_IO_Status(System_Channel CHx)
{
	POWER_STATUS _Power_Status=POWER_OFF;
	if(CHx==CH1)
	{
		_Power_Status=(GPIO_IN[DI_FOOT_SWITCH_1].GPIO_Value)||(GPIO_IN[DI_CHANNEL_SWITCH_1].GPIO_Value);
	}
	else
	{
		_Power_Status=(GPIO_IN[DI_FOOT_SWITCH_2].GPIO_Value||GPIO_IN[DI_CHANNEL_SWITCH_2].GPIO_Value);
	}
	return _Power_Status;
}


POWER_STATUS Check_Purge_IO_Status(System_Channel CHx)
{
	POWER_STATUS _Power_Status=POWER_OFF;
	if(CHx==CH1)
	{
		_Power_Status=(GPIO_IN[DI_PURGE_SWITCH_1].GPIO_Value);
	}
	else
	{
		_Power_Status=(GPIO_IN[DI_PURGE_SWITCH_2].GPIO_Value);
	}
	return _Power_Status;
}
