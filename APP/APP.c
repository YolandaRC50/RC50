#include "APP.h" 

#include "shell.h" 
#include "stddef.h"


//#include "Cartridge_Detect.h"
//USBH_HOST  USB_Host;
//USB_OTG_CORE_HANDLE  USB_OTG_Core;
//static uint8_t update_en = 0;                                                          //更新标记
uint8_t timer_tick_last_update = 0;                                                 //上一次更新的时间 
uint8_t StartLoop=0;
uint8_t m_LoadLoopMax=0x05;
uint8_t LoadSuccess=0; 
uint64_t m_Timer_10MS=0;
uint64_t m_Timer_100MS=0;
uint64_t m_Timer_1S=0;
POWER_STATUS m_Pressure_Alm_Status=POWER_OFF; 
POWER_STATUS m_Last_Pressure_Enable_Status=POWER_OFF;
uint8_t m_Get_HMI_RTC_MARK=1;
uint16_t m_Pressure_Building_Timer=0;
const uint16_t m_Pressure_Building_Timer_Max=10;

POWER_STATUS m_Last_DI_Pressure_Enable_Status =POWER_OFF;
POWER_STATUS m_Last_DI_Run_Stop_Status =POWER_OFF;
POWER_STATUS m_Last_DI_Dispensing_Status[2]={POWER_OFF,POWER_OFF};

POWER_STATUS m_Last_Cycle_Manual_Status[2]={POWER_OFF,POWER_OFF};

POWER_STATUS m_Last_Cycle_DI_Dispensing_Status[2]={POWER_OFF,POWER_OFF};
uint16_t m_Last_DI_Pressure_Enable_Status_Counter =0;
uint16_t m_Last_DI_Run_Stop_Status_Counter =0; 
uint8_t  m_WIFI_Connect_Mark=0;
POWER_STATUS m_Wifi_Search_Success=POWER_OFF;  
char m_WIFI_Ecn[5][10]={"OPEN","WEP","WPA","WPA2","WPA_WPA2"};
/********************************** 内核对象句柄 *********************************/
// 事件标志组
EventGroupHandle_t Event_Handle =NULL;

// 软件定时器句柄 
static TimerHandle_t Swtmr1_Handle =NULL;   

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
  
/////////////
void TIMx_IRQHandler_10MS(void)
{
	m_Timer_10MS++;
}
void TIM4_IRQHandler_10MS(void)
{ 
	//m_Timer_10MS++;
	for(int CHx=CH1;CHx<=CH2;CHx++)
	{
		if(Sys_Params.Channel_Param[CHx].Run_Mode_Params==TIMER 
			&& Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
		{
			switch(Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step)
			{
				case HEAD_DELAY:
				{
					if(Sys_RT_Status.CH_Dispensing_Status[CHx].HEAD_COUNT_DOWN>0)
					{
						Sys_RT_Status.CH_Dispensing_Status[CHx].HEAD_COUNT_DOWN--;
					}
					else
					{ 
						Sys_RT_Status.CH_Dispensing_Status[CHx].HEAD_COUNT_DOWN=0;
						Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STATUS;
					}
					break;
				}
				case DISPENSING_STATUS:
				{
					if(Sys_RT_Status.CH_Dispensing_Status[CHx].DISPENSING_COUNT_DOWN>0)
					{
						Sys_RT_Status.CH_Dispensing_Status[CHx].DISPENSING_COUNT_DOWN--;
					}
					else
					{ 
						Sys_RT_Status.CH_Dispensing_Status[CHx].DISPENSING_COUNT_DOWN=0;
						//Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=TAIL_DELAY;
						Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP;    //Yolanda
						CHx_Manual_Dispensing[CHx]= POWER_OFF;
						Sys_RT_Status.CH_Status[CHx]=READY;
						
						
						if(
							Sys_RT_Status.Cartridge_Status==LOWER 
						  && Sys_RT_Status.Lower_Empty_Counter>0 
						)
						{
								if(Sys_Params.LowSensor_Enable==POWER_ON)
									Sys_RT_Status.Lower_Empty_Counter--;
								else 
									Sys_RT_Status.Lower_Empty_Counter=10;
						}
						
					}
					break;
				}
				case TAIL_DELAY:
				{
					if(Sys_RT_Status.CH_Dispensing_Status[CHx].TAIL_COUNT_DOWN>0)
					{
						Sys_RT_Status.CH_Dispensing_Status[CHx].TAIL_COUNT_DOWN--;
					}
					else
					{ 
						Sys_RT_Status.CH_Dispensing_Status[CHx].TAIL_COUNT_DOWN=0;
						Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP; 
						CHx_Manual_Dispensing[CHx]= POWER_OFF;
						Sys_RT_Status.CH_Status[CHx]=READY;
						
						
						if(
							Sys_RT_Status.Cartridge_Status==LOWER 
						  && Sys_RT_Status.Lower_Empty_Counter>0 
						)
						{
								if(Sys_Params.LowSensor_Enable==POWER_ON)
									Sys_RT_Status.Lower_Empty_Counter--;
								else 
									Sys_RT_Status.Lower_Empty_Counter=10;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			} 
		}
	}
}
void TIMx_IRQHandler_100MS(void)
{
	m_Timer_100MS++;
}
void TIMx_IRQHandler_1S(void)
{
	m_Timer_1S++;
	
	Sys_RT_Status.System_RTC.second++;
	if(Sys_RT_Status.System_RTC.second>=60)
	{
		Sys_RT_Status.System_RTC.second=0;
	  Sys_RT_Status.System_RTC.minute++;
		m_Get_HMI_RTC_MARK=1;
	} 
}
void Dispensing_Logic(void)
{
	for(int CHx=CH1;CHx<=CH2;CHx++)
			{
				if(Sys_RT_Status.System_Option_Mode==STOP_MODE)
				{ 
					#if(1)
					{
						if(Sys_RT_Status.CH_Manuan_Run_Mode[CHx]==POWER_ON || PURGE_IO_STATUS(CHx))
						{
							//START()
							DISPENSING_RUN(CHx);
						}
						else
						{
							//STOP()
							DISPENSING_STOP(CHx);
						}
						
					}
					#endif
					#if(1)
					CHx_Manual_Dispensing[CHx]= POWER_OFF;
					
					Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP;	 
					#endif
				}
				else
				{ 
					#if(1)
//						if(Sys_RT_Status.System_Run_Mode==POWER_OFF  //没有达到自动
//						||m_Pressure_Building_Timer>0								 //压力报警
//						||Sys_RT_Status.Cartridge_Status==EMPTY			 //料没了
//						)
//						{
//							Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP;
//							DISPENSING_STOP(CHx);
//							continue;
//						}
					#endif
					#if(1)
					{
						 switch(Sys_Params.Channel_Param[CHx].Run_Mode_Params)
						 {
							 
							 case CONTINUE:
							 {
								 if(m_Last_Cycle_DI_Dispensing_Status[CHx]!=DISPENSING_IO_STATUS(CHx)
									 || m_Last_Cycle_Manual_Status[CHx]!=CHx_Manual_Dispensing[CHx])
								 {  
									 m_Last_Cycle_DI_Dispensing_Status[CHx]=DISPENSING_IO_STATUS(CHx);
									 m_Last_Cycle_Manual_Status[CHx]=CHx_Manual_Dispensing[CHx];
									 
									 break;
								 }
								 if((DISPENSING_IO_STATUS(CHx)== POWER_ON||CHx_Manual_Dispensing[CHx]== POWER_ON)
									 &&Sys_RT_Status.Cartridge_Status!=EMPTY)			 //料空)
								 { 
										Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STATUS;
										
										DISPENSING_RUN(CHx);
										
								 }
								 else
								 {
									 if(Sys_RT_Status.Cartridge_Status==LOWER 
											&& Sys_RT_Status.Lower_Empty_Counter>0 
											&& (m_Last_DI_Dispensing_Status[CHx]==POWER_ON &&DISPENSING_IO_STATUS(CHx)== POWER_OFF && CHx_Manual_Dispensing[CHx]==POWER_OFF)
										)
										{
											if(Sys_Params.LowSensor_Enable==POWER_ON)
												Sys_RT_Status.Lower_Empty_Counter--;
											else 
												Sys_RT_Status.Lower_Empty_Counter=10;
										}
										CHx_Manual_Dispensing[CHx]=POWER_OFF;
										//STOP()
										Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP;
										DISPENSING_STOP(CHx);
								 }
								 break;
							 }
							 case TIMER:
							 {
								 switch(Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step)
								 {
									 case DISPENSING_STOP://=0,
									 {
										 if(
											 ((m_Last_DI_Dispensing_Status[CHx]==POWER_OFF &&DISPENSING_IO_STATUS(CHx)== POWER_ON)||CHx_Manual_Dispensing[CHx]== POWER_ON)
												&& Sys_RT_Status.Cartridge_Status!=EMPTY ) 
										 {
											 Sys_RT_Status.CH_Dispensing_Status[CHx].HEAD_COUNT_DOWN=Sys_Params.Channel_Param[CHx].Head_Time*100.0;
											 Sys_RT_Status.CH_Dispensing_Status[CHx].DISPENSING_COUNT_DOWN=Sys_Params.Channel_Param[CHx].Timer*100.0;
											 Sys_RT_Status.CH_Dispensing_Status[CHx].TAIL_COUNT_DOWN=Sys_Params.Channel_Param[CHx].Tail_Time*100.0;
											 //Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=HEAD_DELAY;
											 Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STATUS; //Yolanda
											 TIM4_Init();
											 
//												if(Sys_RT_Status.Cartridge_Status==LOWER && Sys_RT_Status.Lower_Empty_Counter>0)
//												{
//													Sys_RT_Status.Lower_Empty_Counter--;
//												} 
										 }
										//STOP()
										DISPENSING_STOP(CHx);
										 break;
									 }
									 case HEAD_DELAY://=1,
									 {
										//STOP()
										DISPENSING_STOP(CHx);
										 CHx_Manual_Dispensing[CHx]= POWER_OFF;
										 break;
									 }
									 case DISPENSING_STATUS://=2,
									 {
										//START()
										DISPENSING_RUN(CHx);
										 break;
									 }
									 case TAIL_DELAY://=3
									 {
										//START()
										DISPENSING_RUN(CHx);
										 break;
									 } 
								 }
								 
								 break;
							 }
						 }
					}
					#endif
					
					#if(1)
					{
					}
					#endif
					
					#if(1)
					{
					}
					#endif
				}
				
				m_Last_DI_Dispensing_Status[CHx]=DISPENSING_IO_STATUS(CHx)||CHx_Manual_Dispensing[CHx];
			}
}


static void vUSB_Task(void *pvParameters)
{
	//初始化USB主机
 // USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); //USB初始化
	
//	portTickType xLastWarkTime=xTaskGetTickCount();
//	portTickType xThisWarkTime=xLastWarkTime;
	while (1) 
	{
		//USBH_Process(&USB_OTG_Core, &USB_Host);  //USB进程
		//vTaskDelay(100);
		
//			xLastWarkTime=xThisWarkTime;
//		vTaskDelayUntil(&xThisWarkTime,(1000/portTICK_RATE_MS)); 
		
		vTaskDelay(200);
  }
}

static void vINF_Task(void *pvParameters)
{		 
	uint8_t ch = 1; 
//	uint8_t TMP_Getloop=0;   
//	USART3_RX_STA=0;
	while (1)
	{  
		LED_Work(ch);
		ch=1-ch;
		vTaskDelay(100);
  }
}

static void vUI_Task(void *pvParameters)
{
	UI_Init();
	qsize  size = 0;  
	current_screen_id=_SCREEN_LOAD_0; 
	while (1) 
	{ 
		if(StartLoop<=m_LoadLoopMax)
		{
 			StartLoop++;
		}
		
		  size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); 		//从缓冲区中获取一条指令         
		
		 if(StartLoop<m_LoadLoopMax)
		 {
				BELL_Work(1);
				vTaskDelay(100);
				BELL_Work(0);
				vTaskDelay(200); 
				BELL_Work(1);
				vTaskDelay(100);
				BELL_Work(0);
			  		
		}
		else if(StartLoop>=m_LoadLoopMax)
		{

			if(LoadSuccess==0)
			{
				
				GetRTCTime();
				SetScreen(_SCREEN_MAIN_2); 
				NotifyScreen(_SCREEN_MAIN_2); 
				LoadSuccess=1;
			}
			if(size>0&&cmd_buffer[1]!=0x07)                                              //接收到指令 ，及判断是否为开机提示
			{                                                                           
			    ProcessMessage((PCTRL_MSG)cmd_buffer, size);                             //指令处理  
			}                                                                           
			else if(size>0&&cmd_buffer[1]==0x07)                                         //如果为指令0x07就软重置STM32  
			{                                                                           
			    __disable_fault_irq();                                                   
			    NVIC_SystemReset();                                                                                                                                          
			} 
		}
		else
		{
			current_screen_id=_SCREEN_LOAD_0;
		}
		 
		UpdateUI();  
		vTaskDelay(100);
	}
}

static void vStatus_Task(void *pvParameters)
{ 
		while (1) 
		{
			DHTxx_Read_Data();  
			SetPressureSensorTargetValue(); 
			
			if(PURGE_LOW_STATUS==POWER_ON)
			{
				Sys_RT_Status.Cartridge_Status=FULL;
				
				Sys_RT_Status.Lower_Empty_Counter=Sys_Params.Low_to_Empty_Counter; 
				CARTRIDGE_LEVEL(FULL);
			}
			else
			{
				if(Sys_RT_Status.Lower_Empty_Counter>0||Sys_Params.LowSensor_Enable==POWER_OFF)
				{
					Sys_RT_Status.Cartridge_Status=LOWER;
					CARTRIDGE_LEVEL(LOWER);
				}
				else
				{
					Sys_RT_Status.Cartridge_Status=EMPTY;
					CARTRIDGE_LEVEL(EMPTY);
				}
			}
			 
			 
			if(Sys_RT_Status.System_Run_Mode==POWER_ON)
			{
				//SET_RUN_STOP_STATUS(POWER_ON);
				if((Sys_RT_Status.RT_Pressure>= Sys_Params.Pressure_Param.TargetPreessure-Sys_Params.Pressure_Param.TargetPreessure_Range)
				&&(Sys_RT_Status.RT_Pressure<= Sys_Params.Pressure_Param.TargetPreessure+Sys_Params.Pressure_Param.TargetPreessure_Range))
				{  
					m_Pressure_Building_Timer=0; 
					PRESSURE_ALARM(POWER_OFF);
				}
				else
				{ 
					PRESSURE_ALARM(POWER_ON);
					if(m_Pressure_Building_Timer<=(m_Pressure_Building_Timer_Max*10))
					{
						m_Pressure_Building_Timer++;  
					}

					if(Sys_RT_Status.CH_Dispensing_Status[CH1].CH_Dispensing_Step==DISPENSING_STOP
						&&Sys_RT_Status.CH_Dispensing_Status[CH2].CH_Dispensing_Step==DISPENSING_STOP)
					{
						
						Sys_RT_Status.System_Option_Mode=STOP_MODE;
						if(m_Pressure_Building_Timer>(m_Pressure_Building_Timer_Max*10))
						{
							Pressure_Display_Enable=POWER_ON;  
							Sys_RT_Status.System_Run_Mode=POWER_OFF;
						}
					}
				}
			}
			else
			{
				Sys_RT_Status.System_Option_Mode=STOP_MODE;
				SET_RUN_STOP_STATUS(POWER_OFF);
				m_Pressure_Building_Timer=0; 
				PRESSURE_ALARM(POWER_OFF);
			}

			//CH STATUS
			for(int CHx=CH1;CHx<=CH2;CHx++)
			{
				if(Sys_RT_Status.System_Run_Mode!=POWER_ON)
				{
					Sys_RT_Status.System_Option_Mode=STOP_MODE;
				}
				if(Sys_RT_Status.System_Option_Mode!=AUTORUN_MODE)
				{
					Sys_RT_Status.CH_Status[CHx]=STOP; 
					Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step=DISPENSING_STOP; 
					CHx_Manual_Dispensing[CHx]=POWER_OFF;
					
				}
				else
				{
					
					if(Sys_RT_Status.CH_Dispensing_Status[CHx].CH_Dispensing_Step==DISPENSING_STOP)
					{
						if((Sys_RT_Status.RT_Pressure>= Sys_Params.Pressure_Param.TargetPreessure-Sys_Params.Pressure_Param.TargetPreessure_Range)
						&&(Sys_RT_Status.RT_Pressure<= Sys_Params.Pressure_Param.TargetPreessure+Sys_Params.Pressure_Param.TargetPreessure_Range)
						/*&& Sys_RT_Status.Cartridge_Status!=EMPTY*/)
						{  
							Sys_RT_Status.CH_Status[CHx]=READY;
						}
						else
						{ 
							Sys_RT_Status.CH_Status[CHx]=ALM;
						} 
					}
					else
					{ 
						Sys_RT_Status.CH_Status[CHx]=DISPENSING;
					} 
				}
			}
			
			//System Status
			
			if(Sys_RT_Status.System_Run_Mode!=POWER_ON)
			{
				Sys_RT_Status.System_Status=STOP;
			}
			else  
			{	
				if(Sys_RT_Status.System_Option_Mode!=STOP_MODE)
				{
					if(Sys_RT_Status.CH_Status[CH1]==DISPENSING || Sys_RT_Status.CH_Status[CH2]==DISPENSING )
					{
						Sys_RT_Status.System_Status=DISPENSING;
					} 
					else if(/*Sys_RT_Status.CH_Status[CH1]==READY 
						&& Sys_RT_Status.CH_Status[CH2]==READY
						&& Sys_RT_Status.Cartridge_Status!=EMPTY 
						&&*/ m_Pressure_Building_Timer==0)
					{
						Sys_RT_Status.System_Status=READY;
					}
					else
					{
						Sys_RT_Status.System_Status=ALM;
					}
				}
				else
				{
					if(m_Pressure_Building_Timer>0)
					{
						Sys_RT_Status.System_Status=ALM;
					}
					else
					{
							Sys_RT_Status.System_Status=STOP;
					}
				}
			}
			
			
			SET_PRE_ENA_STATUS(Sys_RT_Status.System_Run_Mode);
			SET_RUN_STOP_STATUS(Sys_RT_Status.System_Option_Mode);
			
			vTaskDelay(100);
		}
}

void Pointer_stringcat(char *str1,const char *str2)
{

    while (*(str1++)!='\0');     //一直将指向str1的指针移到字符串的末尾
    str1--;
    while (*str2!='\0')
    {
        *(str1++) = *(str2++);  //开始连接
    }
    *str1 = '\0';               //连接完后,添加上字符串结束标识符

}

uint16_t recv_Count=0;
static void vCommunicationResolving_Task(void *pvParameters)
{
	  
		if(Sys_Params.IOT_Params.Commication_Mode!=ETHERNET_MODE
			&&Sys_Params.IOT_Params.Commication_Mode!=WIFI_MODE
		&&Sys_Params.IOT_Params.Commication_Mode!=BLUETOOTH_MODE)
		{
			Sys_Params.IOT_Params.Commication_Mode=ETHERNET_MODE;
		}
		
		while (1) 
		{ 
				 
		  taskENTER_CRITICAL();           //进入临界区	
			if(Sys_Params.IOT_Params.Commication_Mode!=WIFI_MODE || (m_WIFI_Connect_Mark==1 && Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE ))
			{ 
      #if (1)//  收消息			
			if(communication_back_poll()==1)
			{		
					
				  uint8_t _Recv_Finish=0;
//				uint16_t out_len=5;
//				char m_Buffer[100]="Reced_And_Send:"; 
//				out_len=		*Struct_Communication0.struct_data.p_in_u8_l+strlen(m_Buffer); 
//				Pointer_stringcat(m_Buffer,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
//				strcpy( Sys_RT_Status.RT_IOT_Status.Send_Buffer, m_Buffer);
//				*Struct_Communication0.struct_data.p_out_u8_l =out_len;
//				
//				communication_send(&Struct_Communication0); 
//				file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer); 
//				*Struct_Communication0.struct_data.p_in_u8_l=0; 
				 char *token  = strtok(Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8, " ");
				 if(token != NULL) 
				 { 
					 POWER_STATUS __Read=POWER_ON; 
					 char Respond[200]="";
					 //token = strtok(NULL, " "); 
					 #if (1)  /*读取状态*/ 
					 					/*
					客户端请求读取状态或参数
					S_STATUS	
					S_TEM_HUM	
					S_LOW_LEVEL_STATUS	
					S_PRESSURE	
					*///
					 if(strcmp(token , "S_STATUS")==0)
					 {
						 sprintf((char *)Respond,"{\"S_STATUS\": {\"Run\":%s}}","true");		//{“S_STATUS”: {“Run”: True}}
						 _Recv_Finish=1;
					 }
					 else  if(strcmp(token , "S_TEM_HUM")==0)
					 {
						 sprintf((char *)Respond,"{\"S_TEM_HUM\": {\"TEM\":%.2f,\"Hum\":%.2f}}",Sys_RT_Status.Temperature_RT, Sys_RT_Status.Humidity_RT);		
						 _Recv_Finish=1;
					 }
					 else  if(strcmp(token , "S_LOW_LEVEL_STATUS")==0)
					 {
						 sprintf((char *)Respond,"{\"S_LOW_LEVEL_STATUS\": {\"LOW_LEVEL_STATUS\":%s}}",Sys_RT_Status.Cartridge_Status==0?"\"FULL\"":Sys_RT_Status.Cartridge_Status==1?"\"LOWER\"":"\"EMPTY\"");		
						 _Recv_Finish=1;
					 }
					 else  if(strcmp(token , "S_PRESSURE")==0)
					 {
						 sprintf((char *)Respond,"{\"S_PRESSURE\": {\"PRESSURE\":%.2f}}",Sys_RT_Status.RT_Pressure);	
						 _Recv_Finish=1;
					 }
					 #endif 
					 #if (1)  /*设置状态或参数*/
					 /*
						客户端请求写入状态或参数
						W_MODE
						W_CH1_MODE	
						W_CH2_MODE	
						W_CH1_TIMER	
						W_CH2_TIMER	
						W_TARGET_PRESSURE	
						W_PRESSURE_RANGE	
						W_PRESSURE_UNIT	
						W_LOW_LEVEL_COUNT	
						W_LOW_LEVEL_EN	
						W_CH1_PURGE	
						W_CH2_PURGE	
						W_CH1_DISPENSING	
						W_CH2_DISPENSING	
						W_PRESSURE_STATUS
						*/
					 
						 else  if(strcmp(token , "W_MODE")==0)
						 {
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer); 
								 if(strcmp(token , "ON")==0)
								 {
										Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
										Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF;
										Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF;
								 }
								 else
								 {
										Sys_RT_Status.System_Option_Mode=STOP_MODE;
								 } 
								 
								 sprintf((char *)Respond,"{\"W_MODE\": {\"MODE\":%s}}",Sys_RT_Status.System_Option_Mode==AUTORUN_MODE?"true":"false");	
						     _Recv_Finish=1;
							 } 
							 //sprintf((char *)Respond,"MODE %s",Sys_RT_Status.System_Run_Mode==POWER_ON?"TRUE":"FALSE");	
						 }
						 else  if(strcmp(token , "W_CH1_MODE")==0)
						 {
							 /*
							 	CONTINUE=0,
								TIMER=1, 
							 */
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "TIMER")==0)
								 {
										Sys_Params.Channel_Param[CH1].Run_Mode_Params=TIMER; 
								 }
								 else if(strcmp(token , "CONTINUE")==0)
								 {
										Sys_Params.Channel_Param[CH1].Run_Mode_Params=CONTINUE; 
								 }
								 
								 SaveSystemParams();
								 
								 sprintf((char *)Respond,"{\"W_CH1_MODE\": {\"CH1_MODE\":%s}}",Sys_Params.Channel_Param[CH1].Run_Mode_Params==TIMER?"\"TIMER\"":"\"CONTINUE\"");	
						     _Recv_Finish=1;
							 } 
						 }
						 else  if(strcmp(token , "W_CH2_MODE")==0)
						 {
							 /*
							 	CONTINUE=0,
								TIMER=1, 
							 */
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "TIMER")==0)
								 {
										Sys_Params.Channel_Param[CH2].Run_Mode_Params=TIMER; 
								 }
								 else if(strcmp(token , "CONTINUE")==0)
								 {
										Sys_Params.Channel_Param[CH2].Run_Mode_Params=CONTINUE; 
								 }
								 
								 SaveSystemParams();
								 sprintf((char *)Respond,"{\"W_CH2_MODE\": {\"CH2_MODE\":%s}}",Sys_Params.Channel_Param[CH2].Run_Mode_Params==TIMER?"\"TIMER\"":"\"CONTINUE\"");	
						     _Recv_Finish=1;
							 } 
						 }
						 else  if(strcmp(token , "W_CH1_TIMER")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 /* 
							 */
							  
							  token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								float value_double = 0;
							  sscanf((char *)token,"%f",&value_double);//把字符串转换为数
							 
								Sys_Params.Channel_Param[CH1].Timer=value_double;
								SaveSystemParams(); 
								 
							   sprintf((char *)Respond,"{\"W_CH1_TIMER\": {\"CH1_TIMER\":%.2f}}",Sys_Params.Channel_Param[CH1].Timer);	
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_CH2_TIMER")==0)
						 { 
							  
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								float value_double = 0;
							  sscanf((char *)token,"%f",&value_double);//把字符串转换为数
							 
								Sys_Params.Channel_Param[CH2].Timer=value_double;
								SaveSystemParams(); 
							   sprintf((char *)Respond,"{\"W_CH2_TIMER\": {\"CH2_TIMER\":%.2f}}",Sys_Params.Channel_Param[CH2].Timer);	
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_TARGET_PRESSURE")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								float value_double = 0;
							  sscanf((char *)token,"%f",&value_double);//把字符串转换为数
								 //yolanda 20220208 
								if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
								{
										Sys_Params.Pressure_Param.TargetPreessure=value_double;
								}
								else
								{
									Sys_Params.Pressure_Param.TargetPreessure=(value_double/BAR_PSI_TR);
								}
								//Sys_Params.Pressure_Param.TargetPreessure=value_double;   // replaced by 765-772
								 
								SaveSystemParams();
								sprintf((char *)Respond,"{\"W_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}",Sys_Params.Pressure_Param.TargetPreessure);	
						     _Recv_Finish=1;
							 }
						 }
						 else  if(strcmp(token , "W_PRESSURE_RANGE")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								float value_double = 0;
							  sscanf((char *)token,"%f",&value_double);//把字符串转换为数
							 
								Sys_Params.Pressure_Param.TargetPreessure_Range=value_double;
								SaveSystemParams();
								 sprintf((char *)Respond,"{\"W_PRESSURE_RANGE\": {\"PRESSURE_RANGE\":%.2f}}",Sys_Params.Pressure_Param.TargetPreessure_Range);	
						     _Recv_Finish=1;
							 }
						 }
						 else  if(strcmp(token , "W_PRESSURE_UNIT")==0)
						 { 
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 { 
								 
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
									//BAR=0,
									//PSI=1,
								 if(strcmp(token , "BAR")==0)
								 {
									  	Sys_Params.Pressure_Param.Pressure_Unit=BAR; 
								 }
								 else if(strcmp(token , "PSI")==0)
								 {
									  	Sys_Params.Pressure_Param.Pressure_Unit=PSI; 
								 }
								 SaveSystemParams();
							   sprintf((char *)Respond,"{\"W_PRESSURE_UNIT\": {\"PRESSURE_UNIT\":%s}}",Sys_Params.Pressure_Param.Pressure_Unit==BAR?"\"BAR\"":"\"PSI\"");
						     _Recv_Finish=1;
							 } 
						 }
						 else  if(strcmp(token , "W_LOW_LEVEL_COUNT")==0)
						 { 
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								float value_double = 0;
							  sscanf((char *)token,"%f",&value_double);//把字符串转换为数
							 
								Sys_Params.Low_to_Empty_Counter =value_double;
								SaveSystemParams(); 
								 sprintf((char *)Respond,"{\"W_LOW_LEVEL_COUNT\": {\"LOW_LEVEL_COUNT\":%d}}",Sys_Params.Low_to_Empty_Counter);	
						     _Recv_Finish=1;
							 }								 
						 }
						  else  if(strcmp(token , "W_LOW_LEVEL_EN")==0)
						 {
								 
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0 && Sys_Params.LowSensor_Enable == POWER_OFF)
								 { 
										Sys_Params.LowSensor_Enable=POWER_ON; 
								 }
								 else if(strcmp(token , "OFF")==0)
								 { 
										Sys_Params.LowSensor_Enable=POWER_OFF; 
								 } 
								 
								SaveSystemParams(); 
								 sprintf((char *)Respond,"{\"W_LOW_LEVEL_EN\": {\"LOW_LEVEL_EN\":%s}}",Sys_Params.LowSensor_Enable==POWER_ON?"ON":"OFF");	
						   _Recv_Finish=1;
							 }
						 }
						 else  if(strcmp(token , "W_CH1_PURGE")==0)
						 {
								 
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0 && Sys_RT_Status.System_Option_Mode== STOP_MODE)
								 { 
										Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_ON; 
								 }
								 else if(strcmp(token , "OFF")==0)
								 { 
										Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF; 
								 } 
								 sprintf((char *)Respond,"{\"W_CH1_PURGE\": {\"CH1_PURGE\":%s}}",Sys_RT_Status.CH_Manuan_Run_Mode[CH1]==POWER_ON?"true":"false");	
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_CH2_PURGE")==0)
						 { 
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0 && Sys_RT_Status.System_Option_Mode== STOP_MODE)
								 { 
										Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_ON; 
								 }
								 else if(strcmp(token , "OFF")==0)
								 { 
										Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF; 
								 } 
								 sprintf((char *)Respond,"{\"W_CH2_PURGE\": {\"CH2_PURGE\":%s}}",Sys_RT_Status.CH_Manuan_Run_Mode[CH2]==POWER_ON?"true":"false");	
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_CH1_DISPENSING")==0)
						 {
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0 && Sys_RT_Status.System_Option_Mode== AUTORUN_MODE)
								 { 
										CHx_Manual_Dispensing[CH1]=POWER_ON; 
								 }
								 else if(strcmp(token , "OFF")==0)
								 { 
										CHx_Manual_Dispensing[CH1]=POWER_OFF; 
								 } 
								 sprintf((char *)Respond,"{\"W_CH1_DISPENSING\": {\"CH1_DISPENSING\":%s}}",CHx_Manual_Dispensing[CH1]==POWER_ON?"true":"false");	 
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_CH2_DISPENSING")==0)
						 {
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 {
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0 && Sys_RT_Status.System_Option_Mode== AUTORUN_MODE)
								 { 
										CHx_Manual_Dispensing[CH2]=POWER_ON; 
								 }
								 else if(strcmp(token , "OFF")==0)
								 { 
										CHx_Manual_Dispensing[CH2]=POWER_OFF; 
								 } 
								 sprintf((char *)Respond,"{\"W_CH2_DISPENSING\": {\"CH2_DISPENSING\":%s}}",CHx_Manual_Dispensing[CH2]==POWER_ON?"true":"false");	 
						     _Recv_Finish=1; 
							 }
						 }
						 else  if(strcmp(token , "W_PRESSURE_STATUS")==0)
						 {
							 token=strtok(NULL, " ");
							 if(token!=NULL)
							 { 
								 file_write(RUN_STOP_LOG,Sys_RT_Status.RT_IOT_Status.Recive_Buffer);
								 if(strcmp(token , "ON")==0)
								 {
									  Sys_RT_Status.System_Run_Mode=POWER_ON;
								 }
								 else
								 {
									  Sys_RT_Status.System_Run_Mode=POWER_OFF;
								 }
								 sprintf((char *)Respond,"{\"W_PRESSURE_STATUS\": {\"PRESSURE_STATUS\":%s}}",Sys_RT_Status.System_Run_Mode==POWER_ON?"true":"false");	 
						     _Recv_Finish=1;
							 } 
						 }
					 #endif  
			 
					 #if (1)  /*读取状态参数*/
						  
					 
						/*
						回复状态或参数
						R_MODE	
						R_CH1_MODE	
						R_CH2_MODE	
						R_CH1_TIMER	
						R_CH2_TIMER	
						R_TARGET_PRESSURE	
						R_PRESSURE_RANGE	
						R_PRESSURE_UNIT	
						R_LOW_LEVEL_COUNT	
						R_LOW_LEVEL_EN
						R_CH２_PURGE	
						R_CH１_PURGE	
						R_CH１_DISPENSING	
						R_CH２_DISPENSING	
						R_PRESSURE_STATUS
						*/
						else  if(strcmp(token , "R_MODE")==0)
						 { 
							 sprintf((char *)Respond,"{\"R_MODE\": {\"MODE\":%s}}",Sys_RT_Status.System_Option_Mode==AUTORUN_MODE?"true":"false");	
						   _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH1_MODE")==0)
						 {
							 /*
							 	CONTINUE=0,
								TIMER=1, 
							 */
							 sprintf((char *)Respond,"{\"R_CH1_MODE\": {\"CH1_MODE\":%s}}",Sys_Params.Channel_Param[CH1].Run_Mode_Params==TIMER?"\"TIMER\"":"\"CONTINUE\"");	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH2_MODE")==0)
						 {
							 /*
							 	CONTINUE=0,
								TIMER=1, 
							 */
							 sprintf((char *)Respond,"{\"R_CH2_MODE\": {\"CH2_MODE\":%s}}",Sys_Params.Channel_Param[CH2].Run_Mode_Params==TIMER?"\"TIMER\"":"\"CONTINUE\"");	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH1_TIMER")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 /* 
							 */
							 sprintf((char *)Respond,"{\"R_CH1_TIMER\": {\"CH1_MODE\":%.2f}}",Sys_Params.Channel_Param[CH1].Timer);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH2_TIMER")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 sprintf((char *)Respond,"{\"R_CH2_TIMER\": {\"CH2_MODE\":%.2f}}",Sys_Params.Channel_Param[CH2].Timer);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_TARGET_PRESSURE")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 sprintf((char *)Respond,"{\"R_TARGET_PRESSURE\": {\"TARGET_PRESSURE\":%.2f}}",Sys_Params.Pressure_Param.TargetPreessure);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_PRESSURE_RANGE")==0)
						 {
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
							 sprintf((char *)Respond,"{\"R_PRESSURE_RANGE\": {\"PRESSURE_RANGE\":%.2f}}",Sys_Params.Pressure_Param.TargetPreessure_Range);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_PRESSURE_UNIT")==0)
						 {
							 sprintf((char *)Respond,"{\"R_PRESSURE_UNIT\": {\"R_PRESSURE_UNIT\":%s}}",Sys_Params.Pressure_Param.Pressure_Unit==BAR?"\"BAR\"":"\"PSI\"");
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_LOW_LEVEL_COUNT")==0)
						 {
							 sprintf((char *)Respond,"{\"R_LOW_LEVEL_COUNT\": {\"LOW_LEVEL_COUNT\":%d}}",Sys_RT_Status.Lower_Empty_Counter);	
						     _Recv_Finish=1;
						 }
						 
						 else  if(strcmp(token , "R_LOW_LEVEL_EN")==0)
						 {
							 
							 sprintf((char *)Respond,"{\"R_LOW_LEVEL_EN\": {\"R_LOW_LEVEL_EN\":%s}}",Sys_Params.LowSensor_Enable==POWER_ON?"ON":"OFF");	
						   _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH1_PURGE")==0)
						 {
							 //
							 //
							 //	READY=0,
//									DISPENSING=1,
//									STOP=2,
//									ALM=3,
							 sprintf((char *)Respond,"{\"R_CH1_PURGE\": {\"R_CH1_PURGE\":%s}}",Sys_RT_Status.CH_Manuan_Run_Mode[CH1]==POWER_ON?"true":"false");	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH2_PURGE")==0)
						 {
							 sprintf((char *)Respond,"{\"R_CH2_PURGE\": {\"R_CH2_PURGE\":%s}}",Sys_RT_Status.CH_Manuan_Run_Mode[CH2]==POWER_ON?"true":"false");	
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH1_DISPENSING")==0)
						 {
							 sprintf((char *)Respond,"{\"R_CH1_DISPENSING\": {\"R_CH1_DISPENSING\":%s}}",CHx_Manual_Dispensing[CH1]==POWER_ON?"true":"false");	
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_CH2_DISPENSING")==0)
						 {
							 sprintf((char *)Respond,"{\"R_CH2_DISPENSING\": {\"R_CH2_DISPENSING\":%s}}",CHx_Manual_Dispensing[CH2]==POWER_ON?"true":"false");	
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
						     _Recv_Finish=1;
						 }
						 else  if(strcmp(token , "R_PRESSURE_STATUS")==0)
						 {
							 
							 sprintf((char *)Respond,"{\"R_PRESSURE_STATUS\": {\"R_PRESSURE_STATUS\":%s}}",Sys_RT_Status.System_Run_Mode==POWER_ON?"true":"false");	
							 //sprintf((char *)Respond,"PRESSURE %2f",Sys_RT_Status.RT_Pressure);	
						     _Recv_Finish=1;
						 }
						 
					 #endif 
					 
					
						 
					 #if (1)  /*读取SD卡文件*/
					 /*
						读取SD卡文件
						 R_MACHINE_INFO_LOG
						 R_RUN_STOP_LOG
						 R_ERROR_MESSAGE_LOG
						 R_PARAMETER_LOG
						*/ 
						else  if(strcmp(token , "R_MACHINE_INFO_LOG")==0)
						 { 
							 if(Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE)
							 {
								 token=strtok(NULL, " ");
								 if(token!=NULL)
								 { 
									 file_read(MACHINE_INFO_LOG,  token);
									 if(strlen(ReadBuffer)>0)
									 {
										 
										strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, ReadBuffer);
										*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(ReadBuffer);
										communication_send(); 
									 }
						     _Recv_Finish=1;
								 } 
							  }
						 }
						else  if(strcmp(token , "R_RUN_STOP_LOG")==0)
						 {
							 if(Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE)
							 { 
								 token=strtok(NULL, " ");
								 if(token!=NULL)
								 {
									 file_read(RUN_STOP_LOG, token);
									 if(strlen(ReadBuffer)>0)
									 {
										 strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, ReadBuffer);
										*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(ReadBuffer);
										communication_send(); 
									 }
								 }
						     _Recv_Finish=1;
							 }							 
						 }
						else  if(strcmp(token , "R_ERROR_MESSAGE_LOG")==0)
						 {
							 if(Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE)
							 { 
								 token=strtok(NULL, " ");
								 if(token!=NULL)
								 {
									 file_read(ERROR_MESSAGE_LOG, token);
									 if(strlen(ReadBuffer)>0)
									 {
										strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, ReadBuffer);
										*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(ReadBuffer);
										communication_send(); 
									 }
						     _Recv_Finish=1;
								 } 
							 
							 }
						 }
						else  if(strcmp(token , "R_PARAMETER_LOG")==0)
						 {
							 if(Sys_Params.IOT_Params.Commication_Mode==ETHERNET_MODE)
							 { 
								 token=strtok(NULL, " ");
								 if(token!=NULL)
								 {
									 file_read(PARAMETER_LOG, token);
									 if(strlen(ReadBuffer)>0)
									 {
										strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, ReadBuffer);
										*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(ReadBuffer);
										communication_send(); 
									 }
						     _Recv_Finish=1;
								 }  
							 }
						 }
					 #endif
						
					*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l=0; 
					memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN); 
				  
					if(__Read==POWER_ON && strlen(Respond)>0)
					{  
						strcpy( Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, Respond);
						*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =strlen(Respond);
						communication_send(); 
					} 
				 } 
				 
			 
					*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l=0; 
					memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN); 
				   
			}
			#endif
			#if (1) //发消息
			if(	*Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l==0)
			{
				//communication_send(); 
			}
			#endif
			
			}
			taskEXIT_CRITICAL();            //退出临界区
//			puts("1231");
			//vTaskDelay(500);  
			vTaskDelay(20);      //YOLANDA 20220421
			
		}
}

static void vGPIO_Task(void *pvParameters)
{  
	
		vTaskDelay(5000);
		DO_PRESSURE_SENSOR_POWER_ON;
		//ADC_Config();
		vTaskDelay(1000);
		Enable_PressureSensor_Auto_Detect();
	
		while (1) 
		{  
				Refresh_GPIO_Value();
			  if(CHECK_PRESSURE_ENABLE==POWER_ON && m_Last_DI_Pressure_Enable_Status==POWER_OFF&& m_Last_DI_Pressure_Enable_Status_Counter<=10)
				{
					m_Last_DI_Pressure_Enable_Status_Counter++;
					if(m_Last_DI_Pressure_Enable_Status_Counter>=8)
					{
						if(Sys_RT_Status.System_Run_Mode!=POWER_ON)
						{
							Sys_RT_Status.System_Run_Mode=POWER_ON;
						}
						else
						{ 
							Sys_RT_Status.System_Run_Mode=POWER_OFF;
							
						}
						
					m_Last_DI_Pressure_Enable_Status=CHECK_PRESSURE_ENABLE;
					m_Last_DI_Pressure_Enable_Status_Counter=0;
					}
				}
				else
				{
					m_Last_DI_Pressure_Enable_Status=CHECK_PRESSURE_ENABLE;
					m_Last_DI_Pressure_Enable_Status_Counter=0;
				}
			
			  if(CHECK_RUN_STOP==POWER_ON && m_Last_DI_Run_Stop_Status==POWER_OFF && m_Last_DI_Run_Stop_Status_Counter<=10)
				{
					m_Last_DI_Run_Stop_Status_Counter++;
					if(m_Last_DI_Run_Stop_Status_Counter>=8)
					{
						if(Sys_RT_Status.System_Option_Mode != AUTORUN_MODE)
						{
							Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
							Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF;
							Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF;
						}
						else
						{
							Sys_RT_Status.System_Option_Mode = STOP_MODE; 
						}
					m_Last_DI_Run_Stop_Status=CHECK_RUN_STOP;
					m_Last_DI_Run_Stop_Status_Counter=0;
					}
				}
				else
				{
					m_Last_DI_Run_Stop_Status=CHECK_RUN_STOP;
					m_Last_DI_Run_Stop_Status_Counter=0;
				}
				
			  vTaskDelay(1);
			
		}
}
static void vDispensing_Task(void *pvParameters)
{  
		while (1) 
		{   
			Dispensing_Logic();
			vTaskDelay(4);
		}
}
   
static void vWifi_Bluetooth_Recive_Task(void *pvParameters)
{
	
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		case BLUETOOTH_MODE:
		{ 
			break ;
		}
		case ETHERNET_MODE:
		{ 
			 
			return;
		}
	}
	
	WIFI_BLUETOOTH_DMA_UART *p_Struct_DMA_Uart=&Wifi_Bluetooth_DMA_Uart;
	//Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer = Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
	EventBits_t r_event;  /* 定义一个事件接收变量 */
	while(1)
	{
		switch(Sys_Params.IOT_Params.Commication_Mode)
		 {
			case WIFI_MODE: 
			case BLUETOOTH_MODE:
			{
				#if 1
				r_event = xEventGroupWaitBits(Event_Handle,  /* 事件对象句柄 */
																			 (1)<<0,/* 接收线程感兴趣的事件 */
																				pdTRUE,   /* 退出时清除事件位 */
																				pdTRUE,   /* 满足感兴趣的所有事件 */
																				portMAX_DELAY);/* 指定超时事件,一直等 */
				
				switch(r_event&((1)<<0))
				{
					case 1:
							{
								p_Struct_DMA_Uart->Dma_R_Flag=0;
								switch (Sys_Params.IOT_Params.Commication_Mode)
								{
									case WIFI_MODE://WiFi
									{
										puts("--------->\r");
										puts((char*)p_Struct_DMA_Uart->Rxd_Fifo);
									
										//预防数据越界
										if( IOT_SENDANDRECEIVE_BUFFER_LEN< Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len+p_Struct_DMA_Uart->Rxd_L)
										{
											
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len=0;
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
										}
										if(m_WIFI_Connect_Mark==1)
										{
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
										}
										memcpy(Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer,
															 p_Struct_DMA_Uart->Rxd_Fifo,
															 p_Struct_DMA_Uart->Rxd_L);
										
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len +=p_Struct_DMA_Uart->Rxd_L;		
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer+=p_Struct_DMA_Uart->Rxd_L;
										memset( p_Struct_DMA_Uart->Rxd_Fifo,0, IOT_SENDANDRECEIVE_BUFFER_LEN );
									}
										break;
									case BLUETOOTH_MODE://bluetooth
									{
										puts("--------->");
										puts((char*)p_Struct_DMA_Uart->Rxd_Fifo);
									 
										//预防数据越界
										if(IOT_SENDANDRECEIVE_BUFFER_LEN < Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len+p_Struct_DMA_Uart->Rxd_L)
										{
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len =0;
										}
											Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
										memcpy(Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer,
													 p_Struct_DMA_Uart->Rxd_Fifo,
													 p_Struct_DMA_Uart->Rxd_L);
										
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len +=p_Struct_DMA_Uart->Rxd_L;		
										Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer+=p_Struct_DMA_Uart->Rxd_L;
										memset( p_Struct_DMA_Uart->Rxd_Fifo,0, IOT_SENDANDRECEIVE_BUFFER_LEN );
										//strtok(Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer, "a");
										 
										//xEventGroupSetBits(Event_Handle, EVENT_HANDLE_BLUE);
									}
										break;
									
									default:
										break;
								}
							}
						break;
						default :
							break;
				}
				#endif
				break;
			}
			case ETHERNET_MODE:
			{
				
				break;
			}
		 }
	}
}
static void vWifi_Bluetooth_Task(void *pvParameters)
{
	
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{ 
			Wifi_Init();
			break;
		}
		case BLUETOOTH_MODE:
		{ 
			Bluetooth_Init();
			break;
		}
		case ETHERNET_MODE:
		{ 
			return;
		}
	}
	
	BLUETOOTH_KEY=1;
		
	
	#if 1 ///WIFI_MODE
	char *p=NULL;
	uint16_t i=0;
	Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step =0;
	#endif
	
	#if 1 //BLUETOOTH_MODE 
	EventBits_t r_event;  /* 定义一个事件接收变量 */ 
	POWER_STATUS __LoadFirst=POWER_OFF;
	#endif
	
	vTaskDelay(1000);
	while(1)
	{ 
		
			#if 1 //BLUETOOTH_MODE
			if(__LoadFirst==POWER_OFF && Sys_Params.IOT_Params.Commication_Mode==BLUETOOTH_MODE)
			{ 
				/*
				默认配置：
				修改波特率
				配置扫描模式
				配置连接模式
				
				外部输入配置：
				扫描当前周围蓝牙数目，更新入列表中
				选择对应的蓝牙设备
				输入密码
				返回连接状态
				*/  
				 BLUETOOTH_AT=1;
         vTaskDelay(200);
         BLUETOOTH_REST =0;
         vTaskDelay(200);
         BLUETOOTH_REST =1;
				 vTaskDelay(2000);
				//模块角色 0:从机 1:主机
				BLUETOOTH_SEND("AT+ROLE=0\r\n");
				vTaskDelay(1000);
        //设置波特率
        BLUETOOTH_SEND("AT+UART=38400,0,0\r\n");
        vTaskDelay(1000);
        BLUETOOTH_SEND("AT+PSWD=1234\r\n");
         vTaskDelay(1000);
         //重命名
         BLUETOOTH_SEND("AT+NAME=\"ISADII\"\r\n");//15s
				
				vTaskDelay(1000);
        BLUETOOTH_AT=0;
        vTaskDelay(1000);
        BLUETOOTH_REST =0;
        vTaskDelay(200);
        BLUETOOTH_REST =1;
				//Bluetooth_Cmd(15);
				
				__LoadFirst=POWER_ON;
			}
			#endif
     switch(Sys_Params.IOT_Params.Commication_Mode)
		 {
			case WIFI_MODE:
			{
				
					#if 1
			//		puts( "vWifi_Task" );
					switch(Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step)
					{
						case 0://连接wifi
						{
			//				AT+CWJAP_DEF=“newifi_F8A0”,“anxinke123” !!连接网络
							WIFI_SEND("+++");
							vTaskDelay(2000);
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWMODE=1\r\n" );
							
							# if(0) //Set host Station name
							strcat(Struct_Wifi0.array_wifi_send, "AT+CWHOSTNAME=\"" );
							strcat(Struct_Wifi0.array_wifi_send, "001002003004" );
							strcat(Struct_Wifi0.array_wifi_send, "\"\r\n" ); 
							
							#endif
							
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(2000);
							
							
							#if (1) // 强制更改 WIFI 连接名称
							
							//memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,0,20);
							//sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,"%s","Rody_PLC_LINK");	
							
							#endif
							
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0,  IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWJAP_DEF=\"" );  
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\",\"" );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer,  Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\"\r\n" );
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "WIFI CONNECTED");
							p =strstr(p, "WIFI GOT IP");
							if( p!=NULL )
							{
								puts("连接成功\r\n");
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							}
							else
							{
								p=strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "OK");
								if( p!=NULL )
								{
									puts("连接成功\r\n");
									Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								}
									puts("连接失败\r\n");
							}
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer=Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 1://关闭使能上电自动连接AP
						{
							WIFI_SEND("AT+CWAUTOCONN=0\r\n");
							vTaskDelay(5000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							//检测返回值
			//				p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CWAUTOCONN");
			//				p =strstr(p, "OK");
			//				if( p!=NULL )
			//				{
			//					Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
			//					puts("AT+CWAUTOCONN 连接成功\r\n");
			//				}
			//				else
			//				{
			//						puts("AT+CWAUTOCONN 连接失败\r\n");
			//				}
							
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 2://连接连接服务器
						{
							char array_port[8];
							memset(array_port,0,8);
							sprintf(array_port,"%d",Sys_Params.IOT_Params.Server_Params.Sever_Port);
							//AT+CIPSTART=“TCP”,“192.168.99.217”,6001 　　!!连接TCP服务器,本实验用网络调试助手进行测试
							memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CIPSTART=\"TCP\",\"" );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, Sys_Params.IOT_Params.Server_Params.Server_IPAddress);
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\"," );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, array_port );
							strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "\r\n" );
							
							WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPSTART");
							p =strstr(p, "CONNECT");
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPSTART=\"TCP\" 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPSTART=\"TCP\" 连接失败\r\n");
							}
							//检测返回值
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0,IOT_SENDANDRECEIVE_BUFFER_LEN);
						}break;
						case 3://使用透传模式发送数据
						{
							WIFI_SEND("AT+CIPMODE=1\r\n");
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPMODE=1");
							p =strstr(p, "OK");
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							/*
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPMODE=1 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPMODE=1 连接失败\r\n");
							}
							*/
						
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 4://使用透传模式发送数据 发送数据
						{
							WIFI_SEND("AT+CIPSEND\r\n");
							vTaskDelay(5000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
							/*
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+CIPSEND");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;
								puts("AT+CIPSEND 连接成功\r\n");
							}
							else
							{
									puts("AT+CIPSEND 连接失败\r\n");
							}
							*/
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 5://发送数据
						{
							vTaskDelay(5000);
							WIFI_SEND("Ready\r\n");
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step=8;
						}
							break;
						
						case 6://退出透传模式
						{
							vTaskDelay(1000);
							WIFI_SEND("+++");
							vTaskDelay(2000);
							Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;				
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 7://断开服务器 
						{
							WIFI_SEND("AT+CIPCLOSE\r\n");
							vTaskDelay(2000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "CLOSED");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								puts("AT+CIPCLOSE 连接成功\r\n");
								Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step++;	
							}
							else
							{
									puts("AT+CIPCLOSE连接失败\r\n");
							}
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 8:
						{
				
							m_WIFI_Connect_Mark=1;
						}
						break;
						case 10://版本信息
						{
							WIFI_SEND("AT+GMR\r\n");
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							vTaskDelay(5000);
							p =strstr(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, "AT+GMR");
							p =strstr(p, "OK");
							if( p!=NULL )
							{
								puts("--->ok\r\n");
							}
							else
							{
								puts("--->err\r\n");
							}
							puts("\r\n--->\r\n");
							puts( Sys_RT_Status.RT_IOT_Status.Recive_Buffer );
							
							Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
							memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0, IOT_SENDANDRECEIVE_BUFFER_LEN);
						}
							break;
						case 20:
						{
							/*
							AT+CWLAP 列出当前可用AP
							执行指令 AT+CWLAP
							响应 终端返回AP列表+ CWLAP: <ecn>,<ssid>,<rssi> OK ERROR
							参数说明 
							< ecn >
							0 OPEN 
							1 WEP 
							2 WPA_PSK 
							3 WPA2_PSK 
							4 WPA_WPA2_PSK 
							<ssid> 字符串参数，接入点名称
							<rssi> 信号强度 
							*/
							if(m_Wifi_Search_Success==POWER_OFF)
							{
								m_WIFI_Connect_Mark=0;
								
								WIFI_SEND("+++");
								vTaskDelay(2000);
								memset(Sys_RT_Status.RT_IOT_Status.Send_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN);
								strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWMODE=1\r\n" );
								WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
								vTaskDelay(1000);
								
								strcat(Sys_RT_Status.RT_IOT_Status.Send_Buffer, "AT+CWLAP\r\n" );
								WIFI_SEND(Sys_RT_Status.RT_IOT_Status.Send_Buffer);
								vTaskDelay(5000);
								m_Wifi_Search_Success=POWER_ON; 
								
							}
							break;
						}
						default:
			//				Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step=100;
							break;
					}
					#endif
				break;
			}
			case BLUETOOTH_MODE:
			{
				#if 1
				r_event = xEventGroupWaitBits(Event_Handle,  /* 事件对象句柄 */
																			EVENT_HANDLE_BLUE,/* 接收线程感兴趣的事件 */
																			pdTRUE,   /* 退出时清除事件位 */
																			pdTRUE,   /* 满足感兴趣的所有事件 */
																			portMAX_DELAY);/* 指定超时事件,一直等 */
				switch(r_event&EVENT_HANDLE_BLUE)
				{
					case EVENT_HANDLE_BLUE:
					{
						switch(Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command)
						{
							case 01://进入AT模式
							{
									BLUETOOTH_KEY=1;
							 }
							 break;
							 case 02://退出进入AT模式
							 {
									BLUETOOTH_KEY=0;
							 }
							 break;
							 case 15://模块初始化配置
							 { 
								BLUETOOTH_KEY=1;
								vTaskDelay(2000);
								//设置波特率
								BLUETOOTH_SEND("AT+UART=115200,0,0\r\n");
								vTaskDelay(1000);
								//模块角色 0:从机 1:主机
								BLUETOOTH_SEND("AT+ROLE=0\r\n");
								vTaskDelay(1000);
								//连接模式 ：0指定地址连接 1:任意地址连接
								BLUETOOTH_SEND("AT+CMODE=0\r\n");
								vTaskDelay(1000);
								//查询访问模式 ：param1：param2:最多蓝牙数 param3:超时时间x1.28
								BLUETOOTH_SEND("AT+INQM=1,9,12\r\n\r\n");//15s
								vTaskDelay(1000);
							}
							 break;
							case 16://模块初始化数据查询
							{
								BLUETOOTH_KEY=1;
								vTaskDelay(2000);
								//设置波特率
								BLUETOOTH_SEND("AT+UART?\r\n");
								vTaskDelay(1000);
								//模块角色 0:从机 1:主机
								BLUETOOTH_SEND("AT+ROLE?\r\n");
								vTaskDelay(1000);
								//连接模式 ：0指定地址连接 1:任意地址连接
								BLUETOOTH_SEND("AT+CMODE?\r\n");
								vTaskDelay(1000);
								//查询访问模式 ：param1：param2:最多蓝牙数 param3:超时时间x1.28
								BLUETOOTH_SEND("AT+INQM?\r\n\r\n");//15s
								vTaskDelay(1000);
								//指定连接蓝牙
								BLUETOOTH_SEND("AT+BIND?\r\n");
								vTaskDelay(1000);
								//连接蓝牙名称
								BLUETOOTH_SEND("AT+RNAME?0020,08,00400F\r\n");
								vTaskDelay(1000);
								//软件版本号
								 BLUETOOTH_SEND("AT+VERSION?\r\n");
								 vTaskDelay(1000);
								 //密码
								BLUETOOTH_SEND("AT+PSWD?\r\n");
								vTaskDelay(1000);
								BLUETOOTH_KEY=1;
							}
							break;
							case 20://断开连接
							BLUETOOTH_SEND("AT+DISC\r\n");
							break;
							case 21://扫描设备
							{
								//AT模式
								BLUETOOTH_KEY=1;
								vTaskDelay(2000);
								//断开连接
								BLUETOOTH_SEND("AT+DISC\r\n");
								
								vTaskDelay(1500);
								memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN );
								Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
								Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.scan_num=0;
							   
								//扫描设备
								BLUETOOTH_SEND("AT+INQ\r\n");
								//延时检测
								//存储数据到数组
								vTaskDelay(10000);
								puts("\r\n<-r_org->\r\n");
								puts( Sys_RT_Status.RT_IOT_Status.Recive_Buffer );
								string_handle();
								//停止扫描设备
								BLUETOOTH_SEND("AT+INQC\r\n");
							}
							break;
							case 23://选择连接蓝牙,并输入密码
							{
								char array_send[60];
								memset( array_send,0,60 );
								strcpy( array_send,"AT+BIND=" );
								strcat( array_send, Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_connect_ip );
								strcat( array_send, "\r\n" );
								BLUETOOTH_SEND(array_send);
								
								vTaskDelay(1000);
								memset( array_send,0,60 );
								strcpy( array_send,"AT+PSWD=" );
								strcat( array_send,Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_password);
								strcat( array_send, "\r\n" );
								BLUETOOTH_SEND(array_send);
								vTaskDelay(1000);
								BLUETOOTH_SEND("AT+RESET\r\n");
								vTaskDelay(50);
								BLUETOOTH_KEY=0;
							}
							break;
							case 90://设备复位
							{
								BLUETOOTH_KEY=1;
								vTaskDelay(3000);
								//断开连接
								BLUETOOTH_SEND("AT+DISC\r\n");
								vTaskDelay(1000);
								BLUETOOTH_KEY=0;
								BLUETOOTH_SEND("AT+RESET\r\n");
								
							}
							break;
					 }
						Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command=0;
					}
						break;
					default:
						break;
				}
				if( Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command  !=0 )
				{
					switch(Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command)
					{
					 case 01://进入AT模式
					 {
							BLUETOOTH_KEY=1;
					 }
					 break;
					 case 02://退出进入AT模式
					 {
							BLUETOOTH_KEY=0;
					 }
					 break;
					case 15://模块初始化配置
					{
						BLUETOOTH_KEY=1;
						vTaskDelay(2000);
						//设置波特率
						BLUETOOTH_SEND("AT+UART=115200,0,0\r\n");
						vTaskDelay(1000);
						//模块角色 0:从机 1:主机
						BLUETOOTH_SEND("AT+ROLE=1\r\n");
						vTaskDelay(1000);
						//连接模式 ：0指定地址连接 1:任意地址连接
						BLUETOOTH_SEND("AT+CMODE=0\r\n");
						vTaskDelay(1000);
						//查询访问模式 ：param1：param2:最多蓝牙数 param3:超时时间x1.28
						BLUETOOTH_SEND("AT+INQM=1,9,12\r\n\r\n");//15s
						vTaskDelay(1000);
					}
					break;
					
					case 16://模块初始化数据查询
					{
						BLUETOOTH_KEY=1;
						vTaskDelay(2000);
						//设置波特率
						BLUETOOTH_SEND("AT+UART?\r\n");
						vTaskDelay(1000);
						//模块角色 0:从机 1:主机
						BLUETOOTH_SEND("AT+ROLE?\r\n");
						vTaskDelay(1000);
						//连接模式 ：0指定地址连接 1:任意地址连接
						BLUETOOTH_SEND("AT+CMODE?\r\n");
						vTaskDelay(1000);
						//查询访问模式 ：param1：param2:最多蓝牙数 param3:超时时间x1.28
						BLUETOOTH_SEND("AT+INQM?\r\n\r\n");//15s
						vTaskDelay(1000);
						//指定连接蓝牙
						BLUETOOTH_SEND("AT+BIND?\r\n");
						vTaskDelay(1000);
						//连接蓝牙名称
						BLUETOOTH_SEND("AT+RNAME?0020,08,00400F\r\n");
						vTaskDelay(1000);
						//软件版本号
						 BLUETOOTH_SEND("AT+VERSION?\r\n");
						 vTaskDelay(1000);
						 //密码
						BLUETOOTH_SEND("AT+PSWD?\r\n");
						vTaskDelay(1000);
					}
					break;
					case 20://断开连接
					BLUETOOTH_SEND("AT+DISC\r\n");
					break;
					case 21://扫描设备
					{
						//AT模式
						BLUETOOTH_KEY=1;
						vTaskDelay(2000);
						//断开连接
						BLUETOOTH_SEND("AT+DISC\r\n");
						
						vTaskDelay(1500);
						memset( Sys_RT_Status.RT_IOT_Status.Recive_Buffer, 0, IOT_SENDANDRECEIVE_BUFFER_LEN );
						Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.p_Recive_Buffer =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
						Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.scan_num=0;
						//扫描设备
						BLUETOOTH_SEND("AT+INQ\r\n");
						//延时检测
						//存储数据到数组
						vTaskDelay(10000);
						puts("\r\n-->\r\n");
						puts( Sys_RT_Status.RT_IOT_Status.Recive_Buffer );
						string_handle();
						//停止扫描设备
						BLUETOOTH_SEND("AT+INQC\r\n");
					}
					break;
					case 23://选择连接蓝牙,并输入密码
					{
						BLUETOOTH_KEY=1; vTaskDelay(300);
						char array_send[60];
						memset( array_send,0,60 );
						strcpy( array_send,"AT+BIND=" );
						strcat( array_send, Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_connect_ip);
						strcat( array_send, "\r\n" );
						BLUETOOTH_SEND(array_send);
						
						vTaskDelay(1000);
						memset( array_send,0,60 );
						strcpy( array_send,"AT+PSWD=" );
						strcat( array_send,Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password );
						strcat( array_send, "\r\n" );
						BLUETOOTH_SEND(array_send);
						vTaskDelay(1000);
						BLUETOOTH_SEND("AT+RESET\r\n");
						vTaskDelay(50);
						BLUETOOTH_KEY=0;
					}
					break;
					case 90://设备复位
					{
						BLUETOOTH_KEY=1;
						vTaskDelay(3000);
						//断开连接
						BLUETOOTH_SEND("AT+DISC\r\n");
						vTaskDelay(1000);
						BLUETOOTH_KEY=0;
						BLUETOOTH_SEND("AT+RESET\r\n");
					}
					break;
				 }
					Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command=0;
				}
			 #endif
				break;
			}
			case ETHERNET_MODE:
			{
				
				break;
			}
		 }
	}
}
 


static void vEthernet_Task(void *pvParameters)
{
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		case BLUETOOTH_MODE:
		{ 
			return;
		}
		case ETHERNET_MODE:
		{ 
			
		  lwip_comm_init();
			break;
		}
	}
	#if 1
  err_t err,recv_err;
	u32 data_len = 0; 
	
  for(;;)
  {
		#if 1
		//puts("udp_thread_task link...\r\n");
		vTaskDelay(2000);
		//创建一个TCP链接
		Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn = netconn_new(NETCONN_TCP);  
		//连接服务器
		err = netconn_connect(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,
												 &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Server_IPAddr,
												  Sys_Params.IOT_Params.Server_Params.Sever_Port);
		//返回值不等于ERR_OK,删除tcp_clientconn连接
		if(err != ERR_OK)  netconn_delete(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
			//处理新连接的数据		
		else if (err == ERR_OK)   
		{ 
			struct netbuf *recvbuf;
			Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn->recv_timeout = 10;
			//获取本地IP主机IP地址和端口号
			netconn_getaddr(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,
									   &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Loca_IPAddr,
										 &Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Ethernet_Loca_Port,1); 
			//printf("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3],Struct_Tcp_Data0.loca_port);
			while(1)
			{
				if( Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l != 0&&Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_flag !=0) //有数据要发送
					{
            if( Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l >IOT_SENDANDRECEIVE_BUFFER_LEN )
              Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l =IOT_SENDANDRECEIVE_BUFFER_LEN;
						err = netconn_write(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn ,
                                Sys_RT_Status.RT_IOT_Status.Send_Buffer,
                                Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l,
                                NETCONN_COPY); //发送tcp_server_sendbuf中的数据
						if(err != ERR_OK)
						{
							//printf("发送失败\r\n");
						}
            Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l =0;
						Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_flag =0;
					}
					//接收到数据
					recv_err = netconn_recv(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn,&recvbuf);
					switch(recv_err)
					{
						case ERR_OK:
						{
							//数据接收缓冲区清零
							memset(Sys_RT_Status.RT_IOT_Status.Recive_Buffer,0,IOT_SENDANDRECEIVE_BUFFER_LEN);  
							taskENTER_CRITICAL(); 
							for(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q=recvbuf->p; 
							Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q!=NULL;
							Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q=Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->next)  //遍历完整个pbuf链表
							{
								//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
								//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
								if(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len > (IOT_SENDANDRECEIVE_BUFFER_LEN-data_len)) 
								{
									memcpy(Sys_RT_Status.RT_IOT_Status.Recive_Buffer+data_len,
												 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->payload,
												(IOT_SENDANDRECEIVE_BUFFER_LEN-data_len));
								}
								else
								{
										memcpy(Sys_RT_Status.RT_IOT_Status.Recive_Buffer+data_len,
													 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->payload,
													 Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len);
								}						
								data_len += Sys_RT_Status.RT_IOT_Status.Ethernet_Status.q->len;  	
								if(data_len > IOT_SENDANDRECEIVE_BUFFER_LEN) break; //超出TCP客户端接收数组,跳出	
							}
							taskEXIT_CRITICAL();
							
              Sys_RT_Status.RT_IOT_Status.Ethernet_Status.r_l = data_len;
							netbuf_delete(recvbuf);
              //printf("%s\r\n",Struct_Tcp_Data0.tcp_client_recvbuf);
              
              data_len=0;  //复制完成后data_len要清零。
						}
							break;
						case ERR_CLSD:
						{
							netconn_close(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
							netconn_delete(Sys_RT_Status.RT_IOT_Status.Ethernet_Status.tcp_clientconn);
							//printf("服务器%d.%d.%d.%d断开连接\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
							goto lable0;
						}
							break;
					}
			 
			}
			lable0: ;
		}
    #endif
  }
  #endif
	/* USER CODE END StartDefaultTask */
}
void AppTaskCreate(void)
{
		taskENTER_CRITICAL();           //进入临界区	
		//									
		xTaskCreate(   (TaskFunction_t )vGPIO_Task,         			/* 任务函数  */
								 (const char*    )"vGPIO_Task",        				/* 任务名    */
								 (uint16_t       )256,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )30,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */			
 
		xTaskCreate(   (TaskFunction_t )vUI_Task,          				/* 任务函数  */
								 (const char*    )"vUI_Task",        					/* 任务名    */
								 (uint16_t       )512,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )29,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		

		xTaskCreate(   (TaskFunction_t )vINF_Task,          			/* 任务函数  */
								 (const char*    )"vINF_Task",        				/* 任务名    */
								 (uint16_t       )256,              	 							/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )28,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */	
									
		xTaskCreate(   (TaskFunction_t )vStatus_Task,         		/* 任务函数  */
								 (const char*    )"vStatus_Task",        			/* 任务名    */
								 (uint16_t       )128,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )27,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		
									
		xTaskCreate(   (TaskFunction_t )vUSB_Task,         			/* 任务函数  */
								 (const char*    )"vUSB_Task",        				/* 任务名    */
								 (uint16_t       )512,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )26,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */				
									
		xTaskCreate(   (TaskFunction_t )vCommunicationResolving_Task,         			/* 任务函数  */
								 (const char*    )"vCommunicationResolving_Task",        				/* 任务名    */
								 (uint16_t       )256,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )25,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */		
			

		//									
		xTaskCreate(   (TaskFunction_t )vDispensing_Task,         			/* 任务函数  */
								 (const char*    )"vDispensing_Task",        				/* 任务名    */
								 (uint16_t       )128,               								/* 任务栈大小，单位word，也就是4字节 */
								 (void*          )NULL,               								/* 任务参数  */
								 (UBaseType_t    )23,                  						/* 任务优先级*/
								 (TaskHandle_t*  )NULL );             					/* 任务句柄  */									 

		switch(Sys_Params.IOT_Params.Commication_Mode)
		{
			case WIFI_MODE:
			case BLUETOOTH_MODE:
			{ 
				xTaskCreate(   (TaskFunction_t )vWifi_Bluetooth_Recive_Task,       /* 任务函数  */
											 (const char*    )"vWifi_Bluetooth_Recive_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,               			/* 任务参数  */
											 (UBaseType_t    )22,                  			/* 任务优先级*/
											 (TaskHandle_t*  )NULL );             			/* 任务句柄  */		
											 
				xTaskCreate(   (TaskFunction_t )vWifi_Bluetooth_Task,         			/* 任务函数  */
											 (const char*    )"vWifi_Bluetooth_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,               			/* 任务参数  */
											 (UBaseType_t    )21,                  			/* 任务优先级*/
											 (TaskHandle_t*  )NULL );             			/* 任务句柄  */		
				break;
			}
			case ETHERNET_MODE:
			{  
								 
				xTaskCreate(   (TaskFunction_t )vEthernet_Task,         			/* 任务函数  */
											 (const char*    )"vEthernet_Task",        			/* 任务名    */
											 (uint16_t       )256,               				/* 任务栈大小，单位word，也就是4字节 */
											 (void*          )NULL,               			/* 任务参数  */
											 (UBaseType_t    )22,                  			/* 任务优先级*/
											 (TaskHandle_t*)NULL );//创建UDP任务	
				break;
			}
		}
	/* 创建 Event_Handle */
  //Event_Handle = xEventGroupCreate();	 
	taskEXIT_CRITICAL();            //退出临界区
}
static SemaphoreHandle_t  xMutex = NULL;
void AppObjCreate (void)
{
	 /* 创建 Event_Handle */
  Event_Handle = xEventGroupCreate();	
	/* 创建互斥信号量 */
  xMutex = xSemaphoreCreateMutex();
	
	if(xMutex == NULL)
  {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
  }
}
 
