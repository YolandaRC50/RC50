#include "HMI_App.h" 
#include "includes.h" 

#define UI_STYLE_BW			0//��ɫ
uint16_t current_screen_id=0;
uint8_t Pump_AccButtonView=0;
	
uint16_t BackICONIndex=40;
uint16_t StatusICONIndex=50;

uint8_t cmd_buffer[CMD_MAX_SIZE];//ָ���
static int32_t test_value = 0;//����ֵ
  
static uint8_t Level_Vol_Enable_Counter=0;	

uint8_t Auto_AT=0;
uint8_t Temp_Alm_Counter_Trigger=0;

uint8_t CHx_Manual_Dispensing[2]={POWER_OFF,POWER_OFF};
POWER_STATUS Pressure_Display_Enable=POWER_OFF;

uint16_t _Debug_UI_Open_Count =0;
 
IOT_COMMICATION_MODE_ENUM Comm_Mode=ETHERNET_MODE;
	//SetScreen()
uint8_t Level_To_Screen_ID=2;
uint8_t Prev_Screen=2;
char New_Level_1_pwd[PASSWORD_LEN];
char New_Level_2_pwd[PASSWORD_LEN];
char Input_Pwd[PASSWORD_LEN];
uint8_t New_Level_1_pwd_Update=0;
uint8_t New_Level_2_pwd_Update=0;
char Super_Level_1_pwd[PASSWORD_LEN]="888888";
char Super_Level_2_pwd[PASSWORD_LEN]="654321";
void SetTextValueint32_t(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%d",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue0Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%0.0lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.1lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValue2Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.2lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue3Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.3lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
}
void SetTextValue4Float(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.4lf",value);//�Ѹ�����ת��Ϊ�ַ���(����2λС��)
	SetTextValue(screen_id,control_id,buffer);
} 
 
/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16_t screen_id)
{
	
	//TODO: ����û�����
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID
	Clearscreen(0);
	Clearscreen(1);
	
	if(screen_id!=_SCREEN_SETTING_MORE_4)
	{
		Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_OFF;
		Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_OFF;
	}
	if(screen_id!=_SCREEN_DEBUG_10)
	{
		FORCE_ENABLE=POWER_OFF;
	}
	if(screen_id!=_SCREEN_MAIN_2)
	{
		_Debug_UI_Open_Count=0;
	}
	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{ 
			AnimationPlayFrame(current_screen_id,21,Sys_Params.Pressure_Param.Pressure_Unit);
			AnimationPlayFrame(current_screen_id,22,Sys_Params.Pressure_Param.Pressure_Unit);
			break;
		} 
		case _SCREEN_SETTING_3:
		{ 
			
			AnimationPlayFrame(current_screen_id,22,Sys_Params.Pressure_Param.Pressure_Unit); 
			
			SetButtonValue(current_screen_id,4,Sys_Params.Channel_Param[0].Run_Mode_Params==TIMER);
			SetButtonValue(current_screen_id,5,Sys_Params.Channel_Param[0].Run_Mode_Params!=TIMER);
			 
			SetButtonValue(current_screen_id,6,Sys_Params.Channel_Param[1].Run_Mode_Params==TIMER);
			SetButtonValue(current_screen_id,7,Sys_Params.Channel_Param[1].Run_Mode_Params!=TIMER);
			
			SetTextValueFloat(current_screen_id,10,Sys_Params.Channel_Param[0].Timer);
			SetTextValueFloat(current_screen_id,13,Sys_Params.Channel_Param[1].Timer);
			
			
//			SetTextValueFloat(current_screen_id,20,Sys_Params.Channel_Param[0].Head_Time);
//			SetTextValueFloat(current_screen_id,21,Sys_Params.Channel_Param[1].Head_Time);
//			
//			 
//			SetTextValueFloat(current_screen_id,26,Sys_Params.Channel_Param[0].Tail_Time);
//			SetTextValueFloat(current_screen_id,27,Sys_Params.Channel_Param[1].Tail_Time);
			 
			if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
			{
				
				SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure);
				SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range);
			}
			else
			{
				
				SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
				SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range*BAR_PSI_TR);
			}
			
			
			if(Sys_Params.LowSensor_Enable==POWER_ON)
			{
				SetButtonValue(current_screen_id,27,1);
				SetButtonValue(current_screen_id,28,0);
				
				SetControlEnable(current_screen_id,27,0);
				SetControlEnable(current_screen_id,28,1);
			}
			else
			{
				SetButtonValue(current_screen_id,27,0);
				SetButtonValue(current_screen_id,28,1);
				
				SetControlEnable(current_screen_id,27,1);
				SetControlEnable(current_screen_id,28,0);
			}
			
			if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
			{
				SetButtonValue(current_screen_id,29,1);
				SetButtonValue(current_screen_id,30,0);
			}
			else
			{
				SetButtonValue(current_screen_id,29,0);
				SetButtonValue(current_screen_id,30,1);
			}
			
			
			SetTextValueint32_t(current_screen_id,24,Sys_Params.Low_to_Empty_Counter);	
			break;
		} 
		case _SCREEN_SETTING_MORE_4:
		{
			 
			AnimationPlayFrame(current_screen_id,22,Sys_Params.Pressure_Param.Pressure_Unit); 		
			SetTextValue0Float(current_screen_id,19,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value);
			SetTextValue0Float(current_screen_id,20,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value);
			break;
		} 
		case _SCREEN_SYSTEMINI_5:
		{
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_6:
		{
			break;
		}
		case _SCREEN_SETTING_MORE_SAVE_DLG_7:
		{
			break;
		}
		case _SCREEN_PRESSURE_ALM_DLG_9:
		{
			break;
		}
		case _SCREEN_DEBUG_10:
		{ 
			break;
		}
		case _SCREEN_DSETTINGCOMM_11:
		{ 
			
			Comm_Mode=Sys_Params.IOT_Params.Commication_Mode;
			if(Sys_Params.IOT_Params.Commication_Mode==WIFI_MODE)
			{
				SetButtonValue(current_screen_id,4,0);
				SetButtonValue(current_screen_id,5,1);
				SetButtonValue(current_screen_id,6,0);
			}
			else if(Sys_Params.IOT_Params.Commication_Mode==BLUETOOTH_MODE)
			{
				SetButtonValue(current_screen_id,4,0);
				SetButtonValue(current_screen_id,5,0);
				SetButtonValue(current_screen_id,6,1);
			}
			else
			{
				SetButtonValue(current_screen_id,4,1);
				SetButtonValue(current_screen_id,5,0);
				SetButtonValue(current_screen_id,6,0);
			}
			//uint8_t buffer[20] = {0};
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);				
			SetTextValue(current_screen_id,7,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.IP_Address); 
			 
			//sprintf((char *)buffer,"%s","255.255.255.0");				
			SetTextValue(current_screen_id,8,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.Net_Mask); 
			
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Ethernet_Params.Gate_way);			
			SetTextValue(current_screen_id,9,(uint8_t *)Sys_Params.IOT_Params.Ethernet_Params.Gate_way); 
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Ethernet_Params.IP_Address);					
			SetTextValue(current_screen_id,10,(uint8_t *)Sys_Params.IOT_Params.Server_Params.Server_IPAddress); 
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Server_Params.Server_IPAddress);				
			SetTextValue(current_screen_id,11,(uint8_t *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);				
			SetTextValue(current_screen_id,12,(uint8_t *)Sys_Params.IOT_Params.WIFI_Params.Wlan_Password);
			
			//sprintf((char *)buffer,"%s",Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password);				
			SetTextValue(current_screen_id,13,(uint8_t *)Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password);			 
			 
			SetTextValueint32_t(current_screen_id,14,Sys_Params.IOT_Params.Server_Params.Sever_Port); 
			break; 
		}
		case _SCREEN_DSETTINGCOMM_SAVE_DLG_12:
		{
			break;
		}
		case _SCREEN_DSETTINGCOMM_WIFI_SELECT_13:
		{
			break;
		}
		case _SCREEN_CHANGE_PWD_14:
		{
			break;
		}
	}
	
	UpdateUI();
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: ����û�����
}

void UI_Init(void)
{ 
//	SetTextSpeed(0,2,10);
//	UpdateUI();
	
//	if(System_Language==LANG_ENGLISH)
//	{
//		//��������ΪEnglish
//		Setlanguage_EN();
//	}
//	else
//	{
//		//��������Ϊ����
//		Setlanguage_CH();
//	} 
	SetScreen(_SCREEN_LOAD_0);
	
} 
 
void UpdateUI(void)
{  
	
	 if(m_Get_HMI_RTC_MARK==1)
	 {
		 m_Get_HMI_RTC_MARK=POWER_OFF;
		 GetRTCTime();
	 }
	 
	if(Pressure_Display_Enable==POWER_ON)
	{
		SetScreen(_SCREEN_PRESSURE_ALM_DLG_9);
		current_screen_id=_SCREEN_PRESSURE_ALM_DLG_9;
	}
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{
			SetControlVisiable(current_screen_id,50,0);
			SetControlVisiable(current_screen_id,51,1);
			SetControlVisiable(current_screen_id,52,0);
			
			if(Sys_RT_Status.RT_IOT_Status.Connect_Status==POWER_ON)
			{ 
				AnimationPlayFrame(current_screen_id,51,2);
			}
			else
			{
				AnimationPlayFrame(current_screen_id,51,1);
			}
			break;
		}
		case BLUETOOTH_MODE:
		{
			SetControlVisiable(current_screen_id,50,0);
			SetControlVisiable(current_screen_id,51,0);
			SetControlVisiable(current_screen_id,52,1);
			if(Sys_RT_Status.RT_IOT_Status.Connect_Status==POWER_ON)
			{ 
				AnimationPlayFrame(current_screen_id,52,2);
			}
			else
			{
				AnimationPlayFrame(current_screen_id,52,1);
			}
			break;
		}
		default:
		{
		SetControlVisiable(current_screen_id,50,1);
		SetControlVisiable(current_screen_id,51,0);
		SetControlVisiable(current_screen_id,52,0);
		
		if(Sys_RT_Status.RT_IOT_Status.Connect_Status==POWER_ON)
		{ 
			AnimationPlayFrame(current_screen_id,50,2);
		}
		else
		{
			AnimationPlayFrame(current_screen_id,50,1);
		}
		break;
	}
	}
	switch(current_screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			uint8_t buffer[15] = {0};
			sprintf((char *)buffer,"%s",VERSION);				
			SetTextValue(_SCREEN_LOAD_0,2,buffer);
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
			//BatchBegin(current_screen_id);
				if(!Sys_RT_Status.System_Run_Mode)
				{
					Sys_RT_Status.System_Option_Mode=STOP_MODE; 
				} 
				
				SetControlEnable(current_screen_id,20,Sys_RT_Status.System_Run_Mode);
			
				SetButtonValue(current_screen_id,4,Sys_RT_Status.System_Run_Mode);
				SetButtonValue(current_screen_id,20,Sys_RT_Status.System_Option_Mode);
			
				
				if(Sys_RT_Status.System_Option_Mode==AUTORUN_MODE)
				{
					SetControlEnable(current_screen_id,5,POWER_OFF);
					SetControlEnable(current_screen_id,6,POWER_OFF);
					
					Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF;
					Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF;
					
					
					SetControlEnable(current_screen_id,25,POWER_ON);
					SetControlEnable(current_screen_id,26,POWER_ON);
				}
				else
				{
					SetControlEnable(current_screen_id,5,POWER_ON);
					SetControlEnable(current_screen_id,6,POWER_ON);
					
					SetControlEnable(current_screen_id,25,POWER_OFF);
					SetControlEnable(current_screen_id,26,POWER_OFF);
				}
				
				SetButtonValue(current_screen_id,5,Sys_RT_Status.CH_Manuan_Run_Mode[CH1]);
				SetButtonValue(current_screen_id,6,Sys_RT_Status.CH_Manuan_Run_Mode[CH2]);
			//CHx_Manual_Dispensing[CH1]
				SetButtonValue(current_screen_id,7,Sys_RT_Status.Data_Summary_Run_Mode);
			 
				SetButtonValue(current_screen_id,25,CHx_Manual_Dispensing[CH1]);
				SetButtonValue(current_screen_id,26,CHx_Manual_Dispensing[CH2]);
				
				if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)    
				{
					
				SetTextValue2Float(current_screen_id,8, Sys_Params.Pressure_Param.TargetPreessure);
				SetTextValue2Float(current_screen_id,9, Sys_RT_Status.RT_Pressure);
				}
				else
				{
				SetTextValue2Float(current_screen_id,8, Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
				SetTextValue2Float(current_screen_id,9, Sys_RT_Status.RT_Pressure*BAR_PSI_TR);
				}
				
				
				AnimationPlayFrame(current_screen_id,21,Sys_Params.Pressure_Param.Pressure_Unit);
				AnimationPlayFrame(current_screen_id,22,Sys_Params.Pressure_Param.Pressure_Unit);
			
				
				SetTextValueFloat(current_screen_id,10, Sys_RT_Status.Temperature_RT);
				SetTextValueFloat(current_screen_id,11, Sys_RT_Status.Humidity_RT);
			
				SetTextValueFloat(current_screen_id,23, (Sys_RT_Status.Temperature_RT*1.8f+32.0f));
			
				AnimationPlayFrame(current_screen_id,13, Sys_Params.Channel_Param[0].Run_Mode_Params);
				AnimationPlayFrame(current_screen_id,14, Sys_Params.Channel_Param[1].Run_Mode_Params);
			
			
				AnimationPlayFrame(current_screen_id,15, Sys_RT_Status.CH_Status[CH1]);
				AnimationPlayFrame(current_screen_id,16, Sys_RT_Status.CH_Status[CH2]);			
			
				
				AnimationPlayFrame(current_screen_id,19, Sys_RT_Status.System_Status);
					
				if(Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY)
				{
					AnimationPlayFrame(current_screen_id,17, LOWER);
				}
				else
				{
					AnimationPlayFrame(current_screen_id,17, FULL);
				}
				if(Sys_Params.LowSensor_Enable==POWER_ON)
					SetControlVisiable(current_screen_id,18,Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY);
				else
					SetControlVisiable(current_screen_id,18,POWER_OFF);
					
				
				if(Sys_Params.LowSensor_Enable==POWER_ON 
					&& Sys_RT_Status.Cartridge_Status!=FULL)
				{ 
					SetTextValueint32_t(current_screen_id,18,Sys_RT_Status.Lower_Empty_Counter);
				}
				
				if(Sys_Params.Channel_Param[CH1].Run_Mode_Params==TIMER)
				{
					if(Sys_RT_Status.CH_Dispensing_Status[CH1].CH_Dispensing_Step==DISPENSING_STATUS)
					{
						
					}
				}
				//SetTextValueint32_t(current_screen_id,18,m_Timer_1S);
				
				
			
			break;
		} 
		case _SCREEN_SETTING_3:
		{
			
				AnimationPlayFrame(current_screen_id,23, Sys_RT_Status.System_Status);
					
				if(Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY)
				{
					AnimationPlayFrame(current_screen_id,20, LOWER);
				}
				else
				{
					AnimationPlayFrame(current_screen_id,20, FULL);
				}
				if(Sys_Params.LowSensor_Enable==POWER_ON)
					SetControlVisiable(current_screen_id,21,Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY);
				else
					SetControlVisiable(current_screen_id,21,POWER_OFF);
					
				
				if(Sys_Params.LowSensor_Enable==POWER_ON 
					&& Sys_RT_Status.Cartridge_Status!=FULL)
				{ 
					SetTextValueint32_t(current_screen_id,21,Sys_RT_Status.Lower_Empty_Counter);
				}
			
			
				
			break;
		} 
		case _SCREEN_SETTING_MORE_4:
		{
			
				AnimationPlayFrame(current_screen_id,25, Sys_RT_Status.System_Status);
					
				if(Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY)
				{
					AnimationPlayFrame(current_screen_id,23, LOWER);
				}
				else
				{
					AnimationPlayFrame(current_screen_id,23, FULL);
				}
				if(Sys_Params.LowSensor_Enable==POWER_ON)
					SetControlVisiable(current_screen_id,24,Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY);
				else
					SetControlVisiable(current_screen_id,24,POWER_OFF);
					
				
				if(Sys_Params.LowSensor_Enable==POWER_ON 
					&& Sys_RT_Status.Cartridge_Status!=FULL)
				{ 
					SetTextValueint32_t(current_screen_id,24,Sys_RT_Status.Lower_Empty_Counter);
				}
				
			
				SetTextValue2Float(current_screen_id,9, Sys_RT_Status.RT_AN_Pressure);
				if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
				{
					SetTextValue2Float(current_screen_id,10, Sys_RT_Status.RT_Pressure);
				}
				else
				{
					SetTextValue2Float(current_screen_id,10, Sys_RT_Status.RT_Pressure*BAR_PSI_TR);
				}
			
			break;
		} 
		case _SCREEN_SYSTEMINI_5:
		{
				//			int8_t buffer_1[15] = {0}; //
				//			int8_t buffer_2[15] = {0}; //
				//			int8_t buffer_3[15] = {0}; //
				//			int8_t buffer_4[15] = {0}; //
				//			int8_t buffer_5[15] = {0}; //
				//			
				//			sprintf((char *)buffer_1,"%s","EQ Integrated Dispenser");			
				//			sprintf((char *)buffer_2,"%s",VERSION);			
				//			//sprintf((char *)buffer_3,"%s","xxxxxxxx");			
				//			sprintf((char *)buffer_4,"%s","xxxxxxxx");			
				//			sprintf((char *)buffer_5,"%s","20GTCA-001");		
				//			
				//			SetTextValue(_SCREEN_LOAD_0,1,buffer_1);	
				//			SetTextValue(_SCREEN_LOAD_0,2,buffer_2);		
				//			//SetTextValue(_SCREEN_LOAD_0,3,buffer_1);	
				//			SetTextValue(_SCREEN_LOAD_0,4,buffer_4);		
				//			SetTextValue(_SCREEN_LOAD_0,5,buffer_5);
			
			
				AnimationPlayFrame(current_screen_id,23, Sys_RT_Status.Cartridge_Status);
				AnimationPlayFrame(current_screen_id,25, Sys_RT_Status.System_Status);
				
				SetControlVisiable(current_screen_id,24,Sys_RT_Status.Cartridge_Status==LOWER);
				
				if(Sys_RT_Status.Cartridge_Status==LOWER)
				{ 
					SetTextValueint32_t(current_screen_id,24,Sys_RT_Status.Lower_Empty_Counter);
				}
				
				
			uint8_t buffer[15] = {0};
			sprintf((char *)buffer,"%s",VERSION);				
			SetTextValue(current_screen_id,2,buffer);
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_6:
		{
			break;
		}
		case _SCREEN_SETTING_MORE_SAVE_DLG_7:
		{
			break;
		}
		case _SCREEN_PRESSURE_ALM_DLG_9:
		{
			break;
		}
		case _SCREEN_DEBUG_10:
		{
				AnimationPlayFrame(current_screen_id,25, Sys_RT_Status.System_Status);
					
				if(Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY)
				{
					AnimationPlayFrame(current_screen_id,23, LOWER);
				}
				else
				{
					AnimationPlayFrame(current_screen_id,23, FULL);
				}
				if(Sys_Params.LowSensor_Enable==POWER_ON)
					SetControlVisiable(current_screen_id,24,Sys_RT_Status.Cartridge_Status==LOWER||Sys_RT_Status.Cartridge_Status==EMPTY);
				else
					SetControlVisiable(current_screen_id,24,POWER_OFF);
					
				
				if(Sys_Params.LowSensor_Enable==POWER_ON 
					&& Sys_RT_Status.Cartridge_Status!=FULL)
				{ 
					SetTextValueint32_t(current_screen_id,24,Sys_RT_Status.Lower_Empty_Counter);
				}
				
			
				
			/*
				#define DO_CHANNEL_1_VALVE 			0
				#define DO_CHANNEL_2_VALVE 			1
				#define DO_LOW_LEVEL_EMPTY 			2
				#define DO_LOW_LEVEL_LOWER 			3
				#define DO_CHANNEL_1_DISPSING 	4
				#define DO_CHANNEL_2_DISPSING 	5 
				#define DO_PRESSURE_ALARM 			6
				#define DO_RUN_STOP  						7
				#define DO_BACK_VALVE 					8
				#define DO_PRESSURE_POWER 			9
				extern GPIO GPIO_OUT[GP_O_LEN];
				*/
				
				SetButtonValue(current_screen_id,15,FORCE_ENABLE);			
				
				SetControlEnable(current_screen_id,16,FORCE_ENABLE);
				SetControlEnable(current_screen_id,17,FORCE_ENABLE);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				SetControlEnable(current_screen_id,19,FORCE_ENABLE);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				SetControlEnable(current_screen_id,20,FORCE_ENABLE);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				SetControlEnable(current_screen_id,21,FORCE_ENABLE);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				SetControlEnable(current_screen_id,22,FORCE_ENABLE);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				
				if(FORCE_ENABLE==POWER_ON)
				{	
//					#define DO_LOW_LEVEL_EMPTY_FORCE_INDEX 0 
//					#define	DO_LOW_LEVEL_LOWER_FORCE_INDEX	1
//					#define	DO_CHANNEL_1_DISPSING_FORCE_INDEX	2
//					#define	DO_CHANNEL_2_DISPSING_FORCE_INDEX	3
//					#define	DO_PRESSURE_ALARM_FORCE_INDEX	4
//					#define	DO_RUN_STOP_FORCE_INDEX	5
					 					
					
					SetButtonValue(current_screen_id,16,Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status);
					SetButtonValue(current_screen_id,17,Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status);
					SetButtonValue(current_screen_id,19,Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status);
					SetButtonValue(current_screen_id,20,Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status);
					SetButtonValue(current_screen_id,21,Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status);
					SetButtonValue(current_screen_id,22,Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status);
				}
				else
				{
					SetButtonValue(current_screen_id,16,POWER_OFF);
					SetButtonValue(current_screen_id,17,POWER_OFF);
					SetButtonValue(current_screen_id,19,POWER_OFF);
					SetButtonValue(current_screen_id,20,POWER_OFF);
					SetButtonValue(current_screen_id,21,POWER_OFF);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
					SetButtonValue(current_screen_id,22,POWER_OFF);//Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
					
					
					Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status=POWER_OFF;
					Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status=POWER_OFF;
					Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status=POWER_OFF;
					Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status=POWER_OFF;
					Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status=POWER_OFF;
					Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status=POWER_OFF;
				}
				
				SetButtonValue(current_screen_id,14,GPIO_OUT[DO_SYS_RUN].GPIO_Value);
				SetButtonValue(current_screen_id,13,GPIO_OUT[DO_LOW_LEVEL_LOWER].GPIO_Value);
				SetButtonValue(current_screen_id,12,GPIO_OUT[DO_CHANNEL_2_DISPSING].GPIO_Value);
				SetButtonValue(current_screen_id,11,GPIO_OUT[DO_CHANNEL_1_DISPSING].GPIO_Value);
				SetButtonValue(current_screen_id,10,GPIO_OUT[DO_PRESSURE_ALARM].GPIO_Value);
				SetButtonValue(current_screen_id,9,GPIO_OUT[DO_PRE_ENA].GPIO_Value);
				
				
					/*
				#define DI_FOOT_SWITCH_1 				0
				#define DI_FOOT_SWITCH_2 				1
				#define DI_LOW_LEVEL_SENSOR_1 	2
				#define DI_LOW_LEVEL_SENSOR_2 	3
				#define DI_CHANNEL_SWITCH_1 		4
				#define DI_CHANNEL_SWITCH_2 		5
				#define DI_PURGE_SWITCH_1 			6
				#define DI_PURGE_SWITCH_2 			7
				#define DI_PRESSURE_ENABLE			8
				#define DI_RUN_STOP 						9
				#define DI_BACKUP_1 						10
				#define DI_BACKUP_2 						11
				extern GPIO GPIO_IN[GP_I_LEN];
				*/ 
				
				SetButtonValue(current_screen_id,18,GPIO_IN[DI_CHANNEL_SWITCH_1].GPIO_Value);
				SetButtonValue(current_screen_id,4,GPIO_IN[DI_CHANNEL_SWITCH_2].GPIO_Value);
				SetButtonValue(current_screen_id,5,GPIO_IN[DI_PURGE_SWITCH_1].GPIO_Value);
				SetButtonValue(current_screen_id,6,GPIO_IN[DI_PURGE_SWITCH_2].GPIO_Value);
				SetButtonValue(current_screen_id,7,GPIO_IN[DI_PRESSURE_ENABLE].GPIO_Value);
				SetButtonValue(current_screen_id,8,GPIO_IN[DI_RUN_STOP].GPIO_Value);
				 
				
				
			break;
		}
		case _SCREEN_DSETTINGCOMM_11:
		{ 
			break;
		}
		case _SCREEN_DSETTINGCOMM_SAVE_DLG_12:
		{
			break;
		}
		
	  case _SCREEN_DSETTINGCOMM_WIFI_SELECT_13:
		{
					
					SetTextValue(current_screen_id,1,(uint8_t *)Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[0].Ssid);		
					SetTextValue(current_screen_id,2,(uint8_t *)Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[1].Ssid);		
					SetTextValue(current_screen_id,3,(uint8_t *)Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[2].Ssid);		
					SetTextValue(current_screen_id,4,(uint8_t *)Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[3].Ssid);
			
					SetTextValue(current_screen_id,11,(uint8_t *)m_WIFI_Ecn[Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[0].Ecn]);		
					SetTextValue(current_screen_id,12,(uint8_t *)m_WIFI_Ecn[Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[1].Ecn]);		
					SetTextValue(current_screen_id,13,(uint8_t *)m_WIFI_Ecn[Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[2].Ecn]);		
					SetTextValue(current_screen_id,14,(uint8_t *)m_WIFI_Ecn[Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[3].Ecn]);
					
					AnimationPlayFrame(current_screen_id,21,Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[0].Rssi);
					AnimationPlayFrame(current_screen_id,22,Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[1].Rssi);
					AnimationPlayFrame(current_screen_id,23,Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[2].Rssi);
					AnimationPlayFrame(current_screen_id,24,Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[3].Rssi);
			
			break;
		}
		 
		case _SCREEN_CHANGE_PWD_14:
		{
			
			break;
		}
	}
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t state)
{ 
	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
			switch(control_id)
			{
				case 1:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_3;
					Prev_Screen=_SCREEN_MAIN_2;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}
				case 2:
				{
					Level_To_Screen_ID=_SCREEN_DSETTINGCOMM_11;
					Prev_Screen=_SCREEN_MAIN_2;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}
				case 3:
				{ 
					SetScreen(_SCREEN_SYSTEMINI_5);
					break;
				}
				case 4:
				{
					if(state)
					{
						Sys_RT_Status.System_Run_Mode=POWER_ON;
					}
					else
					{
						Sys_RT_Status.System_Run_Mode=POWER_OFF;
					}
					break;
				}
				case 5:
				{
					if(state && Sys_RT_Status.System_Option_Mode== STOP_MODE)
					{
						Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_ON;
					}
					else
					{
						Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF;
					}
					break;
				}
				case 6:
				{
					if(state && Sys_RT_Status.System_Option_Mode== STOP_MODE)
					{
						Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_ON;
					}
					else
					{
						Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF;
					}
					break;
				}
				case 7:
				{
					if(state)
					{
						Sys_RT_Status.Data_Summary_Run_Mode=POWER_ON;
					}
					else
					{
						Sys_RT_Status.Data_Summary_Run_Mode=POWER_OFF;
					}
					break;
				}
				case 20:
				{
					if(state)
					{
						Sys_RT_Status.System_Option_Mode = AUTORUN_MODE;
						Sys_RT_Status.CH_Manuan_Run_Mode[CH1]=POWER_OFF;
						Sys_RT_Status.CH_Manuan_Run_Mode[CH2]=POWER_OFF;
					}
					else
					{
						Sys_RT_Status.System_Option_Mode=STOP_MODE;
					}
					break;
				}
				case 25:
				{
					if(state)
					{ 
						if(Sys_RT_Status.Cartridge_Status!=EMPTY)
							CHx_Manual_Dispensing[CH1]= POWER_ON;
						else
							CHx_Manual_Dispensing[CH1]= POWER_OFF;
							
					}
					else
					{ 
							CHx_Manual_Dispensing[CH1]= POWER_OFF;
					}
					break;
				}
				case 26:
				{
					if(state)
					{
						if(Sys_RT_Status.Cartridge_Status!=EMPTY)
							CHx_Manual_Dispensing[CH2]= POWER_ON;
						else
							CHx_Manual_Dispensing[CH2]= POWER_OFF;
					}
					else
					{
						CHx_Manual_Dispensing[CH2]= POWER_OFF;
					}
					break;
				}
				case 27:
				{
					if(state)
					{
						_Debug_UI_Open_Count++;
						if(_Debug_UI_Open_Count>=10)
						{
							SetScreen(_SCREEN_DEBUG_10);
							current_screen_id=_SCREEN_DEBUG_10;
							_Debug_UI_Open_Count=0;
							SetControlEnable(current_screen_id,4,0);
							SetControlEnable(current_screen_id,5,0);
							SetControlEnable(current_screen_id,6,0);
							SetControlEnable(current_screen_id,7,0);
							SetControlEnable(current_screen_id,8,0);
							SetControlEnable(current_screen_id,9,0);
							SetControlEnable(current_screen_id,10,0);
							SetControlEnable(current_screen_id,11,0);
							SetControlEnable(current_screen_id,12,0);
							SetControlEnable(current_screen_id,13,0);
							SetControlEnable(current_screen_id,14,0);
							SetControlEnable(current_screen_id,18,0);
						}
					}
					break;
				}
			}
			
			break;
		} 
		case _SCREEN_SETTING_3:
		{
			switch(control_id)
			{
				case 2:
				{
					Level_To_Screen_ID=_SCREEN_DSETTINGCOMM_11;
					Prev_Screen=_SCREEN_SETTING_3;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}
				case 8:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_MORE_4;
					Prev_Screen=_SCREEN_SETTING_3;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				} 
				case 3:
				{
					if(state) 
					{
						LoadSystemParams();
					}
					//�ָ�����
					break;
				}
				case 4:
				{
					if(state) 
						Sys_Params.Channel_Param[0].Run_Mode_Params=TIMER;
					SetButtonValue(current_screen_id,4,Sys_Params.Channel_Param[0].Run_Mode_Params==TIMER);
					SetButtonValue(current_screen_id,5,Sys_Params.Channel_Param[0].Run_Mode_Params!=TIMER);
			 
					break;
				}
				case 5:
				{
				if(state) Sys_Params.Channel_Param[0].Run_Mode_Params=CONTINUE;
					SetButtonValue(current_screen_id,4,Sys_Params.Channel_Param[0].Run_Mode_Params==TIMER);
					SetButtonValue(current_screen_id,5,Sys_Params.Channel_Param[0].Run_Mode_Params!=TIMER);
					break;
				}
				case 6:
				{
					if(state) Sys_Params.Channel_Param[1].Run_Mode_Params=TIMER;
					SetButtonValue(current_screen_id,6,Sys_Params.Channel_Param[1].Run_Mode_Params==TIMER);
					SetButtonValue(current_screen_id,7,Sys_Params.Channel_Param[1].Run_Mode_Params!=TIMER);
					break;
				}
				case 7:
				{
				if(state) Sys_Params.Channel_Param[1].Run_Mode_Params=CONTINUE;
				SetButtonValue(current_screen_id,6,Sys_Params.Channel_Param[1].Run_Mode_Params==TIMER);
				SetButtonValue(current_screen_id,7,Sys_Params.Channel_Param[1].Run_Mode_Params!=TIMER);
					break;
				}
				case 14:
				{ 
					if(state && Sys_Params.Channel_Param[0].Timer>1) 
					{
						Sys_Params.Channel_Param[0].Timer-=1;
						SetTextValueFloat(current_screen_id,10,Sys_Params.Channel_Param[0].Timer);
					}
					break;
				}
				case 16:
				{ 
					if(state ) 
					{
						Sys_Params.Channel_Param[0].Timer+=1;
						SetTextValueFloat(current_screen_id,10,Sys_Params.Channel_Param[0].Timer);
					}
					break;
				}
				case 18:
				{ 
					if(state && Sys_Params.Channel_Param[1].Timer>1) 
					{
						Sys_Params.Channel_Param[1].Timer-=1;
						SetTextValueFloat(current_screen_id,13,Sys_Params.Channel_Param[1].Timer);
					}
					break;
				}
				case 19:
				{ 
					if(state) 
					{
						Sys_Params.Channel_Param[1].Timer+=1;
						SetTextValueFloat(current_screen_id,13,Sys_Params.Channel_Param[1].Timer);
					}
					break;
				}
				
				case 25:
				{
					if(state && Sys_Params.Low_to_Empty_Counter>1) 
					{
						Sys_Params.Low_to_Empty_Counter-=1;
						SetTextValueint32_t(current_screen_id,24,Sys_Params.Low_to_Empty_Counter);
					}
					break; 
				}
				case 26:
				{
					if(state) 
					{
						Sys_Params.Low_to_Empty_Counter+=1;
						SetTextValueint32_t(current_screen_id,24,Sys_Params.Low_to_Empty_Counter);
					}
					break;
				}
				case 27:
				{
					if(state) 
					{
						Sys_Params.LowSensor_Enable=POWER_ON;		

						SetButtonValue(current_screen_id,27,POWER_ON);						
						SetButtonValue(current_screen_id,28,POWER_OFF);		

						SetControlEnable(current_screen_id,27,POWER_OFF);	
						SetControlEnable(current_screen_id,28,POWER_ON);				
					} 
					
					break;
				}
				case 28:
				{
					if(state) 
					{
						Sys_Params.LowSensor_Enable=POWER_OFF; 	

						SetButtonValue(current_screen_id,27,POWER_OFF);						
						SetButtonValue(current_screen_id,28,POWER_ON);		

						SetControlEnable(current_screen_id,27,POWER_ON);	
						SetControlEnable(current_screen_id,28,POWER_OFF);					
					}
					break;
				}
				
				 
				case 29:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Pressure_Unit= BAR;		
					}				
						SetButtonValue(current_screen_id,29,Sys_Params.Pressure_Param.Pressure_Unit== BAR);						
						SetButtonValue(current_screen_id,30,Sys_Params.Pressure_Param.Pressure_Unit!= BAR);
					break;
				}
				case 30:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Pressure_Unit= PSI;			
					}			
						SetButtonValue(current_screen_id,29,Sys_Params.Pressure_Param.Pressure_Unit== BAR);						
						SetButtonValue(current_screen_id,30,Sys_Params.Pressure_Param.Pressure_Unit!= BAR);
					break;
				}
				case 15:
				{ 
					
					if(state && Sys_Params.Pressure_Param.TargetPreessure>0.1) 
					{ 
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure=
							Sys_Params.Pressure_Param.TargetPreessure-0.05; 
						SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure);
						}
						else
						{
								Sys_Params.Pressure_Param.TargetPreessure=
							Sys_Params.Pressure_Param.TargetPreessure-(1.0f/BAR_PSI_TR);
						SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
						}
						
					}
					break;
				}
				case 17:
				{
					if(state) 
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure=
							Sys_Params.Pressure_Param.TargetPreessure+0.05; 
						SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure);
						}
						else
						{
								Sys_Params.Pressure_Param.TargetPreessure=
							Sys_Params.Pressure_Param.TargetPreessure+(1.0f/BAR_PSI_TR);
						SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
							
						}
						//Sys_Params.Pressure_Param.TargetPreessure+=0.1;
					}
					break;
				}
				case 33:
				{ 
					
					if(state && Sys_Params.Pressure_Param.TargetPreessure_Range>0.1) 
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure_Range
							=Sys_Params.Pressure_Param.TargetPreessure_Range-0.05f; 
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range);
						}
						else
						{
								Sys_Params.Pressure_Param.TargetPreessure_Range
							=Sys_Params.Pressure_Param.TargetPreessure_Range-(1.0f/BAR_PSI_TR);
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range*BAR_PSI_TR);
						}
						
					}
					break;
				}
				case 34:
				{
					if(state) 
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							Sys_Params.Pressure_Param.TargetPreessure_Range
							=Sys_Params.Pressure_Param.TargetPreessure_Range+0.05f; 
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range);
						}
						else
						{
								Sys_Params.Pressure_Param.TargetPreessure_Range
							=Sys_Params.Pressure_Param.TargetPreessure_Range+(1.0f/BAR_PSI_TR);
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range*BAR_PSI_TR);
						} 
					}
					break;
				}
				//----------------------------
			}
			break;
		} 
		case _SCREEN_SETTING_MORE_4:
		{
			
			switch(control_id)
			{
				case 2:
				{
					Level_To_Screen_ID=_SCREEN_DSETTINGCOMM_11;
					Prev_Screen=_SCREEN_SETTING_MORE_4;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				} 
				case 6:
				{
					if(state) 
					{
						LoadSystemParams();
					}
					break;
				}
				case 4:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=Sys_RT_Status.RT_AN_Pressure;
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=0;
					}
					break;
				}
				case 5:
				{
					if(state) 
					{
						Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=Sys_RT_Status.RT_AN_Pressure;
						Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=5;
					}
					break;
				} 
				case 8:
				{ 
					if(state && Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value--;
						SetTextValue0Float(current_screen_id,19,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value); 
					}
					break;
				} 
				case 11:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value++;
						SetTextValue0Float(current_screen_id,19,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value); 
					}
					break;
				} 
				case 13:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value--;
						SetTextValue0Float(current_screen_id,20,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value); 
					}
					break;
				} 
				case 14:
				{
					if(state && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON) 
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value++;
						SetTextValue0Float(current_screen_id,20,Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value); 
					}
					break;
				}
				case 15:
				{
					Level_To_Screen_ID=_SCREEN_CHANGE_PWD_14;
					Prev_Screen=_SCREEN_SETTING_MORE_4;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}  
				case 18:
				{ 
					if(state) 
					{
						Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_ON;		
						Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_OFF;		
						SetButtonValue(current_screen_id,18,1);						
						SetButtonValue(current_screen_id,21,0);
					}
					break;
				}
				case 21:
				{ 
					if(state) 
					{
						Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable=POWER_OFF;		
						Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable=POWER_ON;		
						SetButtonValue(current_screen_id,18,0);						
						SetButtonValue(current_screen_id,21,1);
					}
				}
			}
			break;
		} 
		case _SCREEN_SYSTEMINI_5:
		{
						
			switch(control_id)
			{
				case 9:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_3;
					Prev_Screen=_SCREEN_SYSTEMINI_5;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				} 
				case 7:
				{
					Level_To_Screen_ID=_SCREEN_DSETTINGCOMM_11;
					Prev_Screen=_SCREEN_SYSTEMINI_5;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				} 
			} 
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_6:
		{ 
					
			switch(control_id)
			{
				case 1:
				case 2:
				{
					if(state) 
					{
						SaveSystemParams();
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{
						LoadSystemParams();
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_SETTING_MORE_SAVE_DLG_7:
		{		
			switch(control_id)
			{
				case 1:
				case 2:
				{
					if(state) 
					{
						SaveSystemParams();
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{
						LoadSystemParams();
					}
					break;
				}
			}
			break; 
		}
		case _SCREEN_PRESSURE_ALM_DLG_9:
		{
			switch(control_id)
			{
				case 1:
				case 2:
				{ 
					break;
				}
				case 3: 
				{
					if(state) 
					{
						Pressure_Display_Enable=POWER_OFF;
						SetScreen(_SCREEN_MAIN_2);
					}
					break;
				}
			}
			break;
		}
		case _SCREEN_DEBUG_10:
		{
			switch(control_id)
			{	
				case 2:
				{
					Level_To_Screen_ID=_SCREEN_DSETTINGCOMM_11;
					Prev_Screen=_SCREEN_DEBUG_10;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				} 
				case 28:
				{
					Level_To_Screen_ID=_SCREEN_CHANGE_PWD_14;
					Prev_Screen=_SCREEN_DEBUG_10;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}  
				case 3:
				{
					FORCE_ENABLE=POWER_OFF;					
					_Debug_UI_Open_Count=0;
					break;
				}
				/*
					SetControlVisiable(current_screen_id,15,FORCE_ENABLE);
					SetControlVisiable(current_screen_id,16,Force_List[DO_RUN_STOP].Forced_Status);
					SetControlVisiable(current_screen_id,17,Force_List[DO_PRESSURE_ALARM].Forced_Status);
					SetControlVisiable(current_screen_id,19,Force_List[DO_CHANNEL_2_DISPSING].Forced_Status);
					SetControlVisiable(current_screen_id,20,Force_List[DO_CHANNEL_1_DISPSING].Forced_Status);
					SetControlVisiable(current_screen_id,21,Force_List[DO_LOW_LEVEL_LOWER].Forced_Status);
					SetControlVisiable(current_screen_id,22,Force_List[DO_LOW_LEVEL_EMPTY].Forced_Status);
				*/
				case 15:
				{
					if(state)
					{
						FORCE_ENABLE=POWER_ON;
					}
					else
					{
						FORCE_ENABLE=POWER_OFF;
					}
					SetButtonValue(current_screen_id,15,FORCE_ENABLE);
					break;
				}
				case 16:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,16,Force_List[DO_PRE_ENA_FORCE_INDEX].Forced_Status);
					break;
				}
				case 17:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,17,Force_List[DO_PRESSURE_ALARM_FORCE_INDEX].Forced_Status);
					break;
				}
				case 19:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,19,Force_List[DO_CHANNEL_1_DISPSING_FORCE_INDEX].Forced_Status);
					break;
				}
				case 20:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,20,Force_List[DO_CHANNEL_2_DISPSING_FORCE_INDEX].Forced_Status);
					break;
				}
				case 21:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,21,Force_List[DO_LOW_LEVEL_LOWER_FORCE_INDEX].Forced_Status);
					break;
				}
				case 22:
				{
					if(state && FORCE_ENABLE)
					{
						Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status=POWER_ON;
					}
					else
					{
						Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status=POWER_OFF;
					}
					SetButtonValue(current_screen_id,22,Force_List[DO_SYS_RUN_FORCE_INDEX].Forced_Status);
					break;
				}
				case 26:
				{
					
					FORCE_ENABLE=POWER_OFF;					
					_Debug_UI_Open_Count=0;
					break;
				}
			}
			break;
		}
	
		case _SCREEN_DSETTINGCOMM_11:
		{
			
			switch(control_id)
			{
				case 1:
				{
					break;
				}
				case 2:
				{
					Level_To_Screen_ID=_SCREEN_SETTING_3;
					Prev_Screen=_SCREEN_DSETTINGCOMM_11;
					SetScreen(_SCREEN_INPUT_PWD_15);
					break;
				}  
				case 3:
				{
					if(state)
					{
					}
					break;
				}
				case 4:
				{
					
					if(Sys_Params.IOT_Params.Commication_Mode!=ETHERNET_MODE && state)
					{
						Sys_Params.IOT_Params.Commication_Mode=ETHERNET_MODE;
						SetButtonValue(screen_id,4,1);
						SetButtonValue(screen_id,5,0);
						SetButtonValue(screen_id,6,0);
					} 
					break;
				}
				case 5:
				{
					
					if(Sys_Params.IOT_Params.Commication_Mode!=WIFI_MODE &&state)
					{
						Sys_Params.IOT_Params.Commication_Mode=WIFI_MODE;
						SetButtonValue(screen_id,4,0);
						SetButtonValue(screen_id,5,1);
						SetButtonValue(screen_id,6,0);
					}
					 
					break;
				}
				case 6:
				{
					
					if(Sys_Params.IOT_Params.Commication_Mode!=BLUETOOTH_MODE && state)
					{
						Sys_Params.IOT_Params.Commication_Mode=BLUETOOTH_MODE;
						SetButtonValue(screen_id,4,0);
						SetButtonValue(screen_id,5,0);
						SetButtonValue(screen_id,6,1);
					}
				 
					break;
				}
				
				case 15:
				{
					//_SCREEN_DSETTINGCOMM_WIFI_SELECT_13 
					//Sys_RT_Status.RT_IOT_Status.WIFI_Status.Status_Step=20;
					break;
				}
			}
			break;
		}
		case _SCREEN_DSETTINGCOMM_SAVE_DLG_12:
		{
					
			switch(control_id)
			{
				case 1:
				case 2:
				{
					if(state) 
					{
						SaveSystemParams();
						
					  if(Comm_Mode!=Sys_Params.IOT_Params.Commication_Mode)
						{
							__disable_fault_irq();                                                   
							NVIC_SystemReset();  
						}
					}
					break;
				}
				case 3: 
				{
					if(state) 
					{
						LoadSystemParams();
					}
					break;
				}
			}
			break; 
		}
		
	  case _SCREEN_DSETTINGCOMM_WIFI_SELECT_13:
		{
			switch(control_id)
			{
				case 31:
				{
					
					memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, 0, DEVICE_NAME_LEN);
					strcat(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[0].Ssid); 
					break;
				}
				case 32:
				{
					memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, 0, DEVICE_NAME_LEN);
					strcat(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[1].Ssid); 
					break;
				}
				case 33:
				{
					memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, 0, DEVICE_NAME_LEN);
					strcat(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[2].Ssid); 
					break;
				}
				case 34:
				{
					memset(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, 0, DEVICE_NAME_LEN);
					strcat(Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name, Sys_RT_Status.RT_IOT_Status.WIFI_Status.Wifi_Search_List[3].Ssid); 
					break;
				}
			}
			break;
		}
		 
		case _SCREEN_CHANGE_PWD_14:
		{
			switch(control_id)
			{ 
				case 1:
				{  
					if(state) 
					{  
//							uint8_t New_Level_1_pwd_Update=0;
//							uint8_t New_Level_2_pwd_Update=0;
						if(New_Level_1_pwd_Update==1)
						{
							memset(Sys_Params.Login_Pwd.First_Password, 0, PASSWORD_LEN); 
							sprintf((char *)Sys_Params.Login_Pwd.First_Password,"%s",New_Level_1_pwd);	
						}
						if(New_Level_2_pwd_Update==1)
						{
							memset(Sys_Params.Login_Pwd.Second_Password, 0, PASSWORD_LEN); 
							sprintf((char *)Sys_Params.Login_Pwd.Second_Password,"%s",New_Level_2_pwd);	
						}
							SaveSystemParams();
						  New_Level_1_pwd_Update=0;
							New_Level_2_pwd_Update=0;
							memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
							memset(New_Level_2_pwd, 0, PASSWORD_LEN);   
						  SetScreen(_SCREEN_MAIN_2);	
					}
					break;
				}
				case 4:
				case 5:
				{
					if(state) 
					{
						New_Level_1_pwd_Update=0;
						New_Level_2_pwd_Update=0;
						memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
						memset(New_Level_2_pwd, 0, PASSWORD_LEN);   
					}
				}
			}
			break;
		}
		case _SCREEN_INPUT_PWD_15:
		{
			switch(control_id)
			{ 
				case 3:
				{ 
					if(state) 
					{ 
						uint8_t login_Success=0;
//					Level_To_Screen_ID=_SCREEN_SETTING_3;
//					Prev_Screen=_SCREEN_DSETTINGCOMM_11;
						
						if(Level_To_Screen_ID==_SCREEN_CHANGE_PWD_14||Level_To_Screen_ID==_SCREEN_SETTING_MORE_4)
						{
							if(strcmp(Input_Pwd,Sys_Params.Login_Pwd.Second_Password)==0||strcmp(Input_Pwd,Super_Level_2_pwd)==0)
							{
								login_Success=1;
							}
						}
						else
						{
							if(strcmp(Input_Pwd,Sys_Params.Login_Pwd.First_Password)==0||strcmp(Input_Pwd,Super_Level_1_pwd)==0)
							{
								login_Success=1;
							}
						}
						 if(login_Success==1)
						 {
							 SetScreen(Level_To_Screen_ID);		
							 NotifyScreen(Level_To_Screen_ID);
						 }	
						 else
						 {
							 SetScreen(Prev_Screen);	
							 NotifyScreen(Prev_Screen);
						 }
							uint8_t buffer[15] = {0};
							sprintf((char *)buffer,"%s","");	 
						 SetTextValue(_SCREEN_INPUT_PWD_15,2,buffer);
						 Level_To_Screen_ID=Prev_Screen;
						 memset(Input_Pwd, 0, PASSWORD_LEN); 
					}
					break;
				}
				case 4: 
				{
					if(state) 
					{ 
						 SetScreen(Prev_Screen);
						 Level_To_Screen_ID=Prev_Screen;
						 memset(Input_Pwd, 0, PASSWORD_LEN); 
					}
					break;
				}
			}
			break;
		}
	}
		

}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	//TODO: ����û�����
//	int32_t value = 0; 
	float value_double = 0;
	
	switch(screen_id)
	{ 
		case _SCREEN_LOAD_0:
		{
			break;
		} 
		case _SCREEN_SOFTUPDATE_1:
		{
			break;
		} 
		case _SCREEN_MAIN_2:
		{
			break;
		} 
		case _SCREEN_SETTING_3:
		{
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
			switch (control_id)
			{
				case 10:
				{
					Sys_Params.Channel_Param[0].Timer=value_double;
					break;
				}
				case 13:
				{
					Sys_Params.Channel_Param[1].Timer=value_double;
					break;
				}
				case 20:
				{
					Sys_Params.Channel_Param[0].Head_Time=value_double;
					break;
				}
				case 21:
				{
					Sys_Params.Channel_Param[1].Head_Time=value_double;
					break;
				}
				
				case 26:
				{
					Sys_Params.Channel_Param[0].Tail_Time=value_double;
					break;
				}
				case 27:
				{
					
					Sys_Params.Channel_Param[1].Tail_Time=value_double;
					break;
				}
				
				case 24:
				{
					Sys_Params.Low_to_Empty_Counter=(uint16_t)(value_double+(0.5));
					break;
				}
				
				case 11:
				{
					if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
					{
							Sys_Params.Pressure_Param.TargetPreessure=value_double;
					}
					else
					{
						Sys_Params.Pressure_Param.TargetPreessure=(value_double/BAR_PSI_TR);
					}
					if(Sys_Params.Pressure_Param.TargetPreessure>9)
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure);
						}
						else
						{
							SetTextValue2Float(current_screen_id,11,Sys_Params.Pressure_Param.TargetPreessure*BAR_PSI_TR);
						}
					}
					break;
				}
				case 32:
				{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
					{
							Sys_Params.Pressure_Param.TargetPreessure_Range=value_double;
					}
					else
					{
						Sys_Params.Pressure_Param.TargetPreessure_Range=(value_double/BAR_PSI_TR);
					}
					 
					if(Sys_Params.Pressure_Param.TargetPreessure_Range>9)
					{
						if(Sys_Params.Pressure_Param.Pressure_Unit==BAR)
						{
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range);
						}
						else
						{
							SetTextValue2Float(current_screen_id,32,Sys_Params.Pressure_Param.TargetPreessure_Range*BAR_PSI_TR);
						}
					}
					break;
				}
			}
			break;
		} 
		case _SCREEN_SETTING_MORE_4:
		{
			
			sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ��
			switch(control_id)
			{
				case 19:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=0;
					if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=(value_double+(0.005));
					}
					break;
				}
				case 20:
				{
					Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=5.0;
					if(Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON)
					{
						Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=(value_double+(0.005));
					}
					break;
				}
			}
			break;
		} 
		case _SCREEN_SYSTEMINI_5:
		{
			break;
		} 
		case _SCREEN_SETTING_SAVE_DLG_6:
		{
			break;
		}
		case _SCREEN_SETTING_MORE_SAVE_DLG_7:
		{
			break;
		}
		case _SCREEN_PRESSURE_ALM_DLG_9:
		{
			break;
		}
		case _SCREEN_DEBUG_10:
		{
			break;
		}
		case _SCREEN_DSETTINGCOMM_11:
		{ 
			
			//sscanf((char *),"%f",&value_double);//���ַ���ת��Ϊ��
			switch (control_id)
			{
				case 7:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.IP_Address,"%s",str);			
					break;
				}
				case 8:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.Net_Mask,"%s",str);	
					sprintf((char *)str,"%s",(char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name);		
					break;
				}
				case 9:
				{
					sprintf((char *)Sys_Params.IOT_Params.Ethernet_Params.Gate_way,"%s",str);		
					break;
				}
				case 10:
				{
					sprintf((char *)Sys_Params.IOT_Params.Server_Params.Server_IPAddress,"%s",str);	
					break;
				}
				case 11:
				{
					sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name,"%s",str);		
					break;
				}
				case 12:
				{
					sprintf((char *)Sys_Params.IOT_Params.WIFI_Params.Wlan_Password,"%s",str);		
					break;
				}
				case 13:
				{
					sprintf((char *)Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password,"%s",str);		
					break;
				}
				case 14:
				{
				  sscanf((char *)str,"%f",&value_double);//���ַ���ת��Ϊ�� 
					Sys_Params.IOT_Params.Server_Params.Sever_Port=value_double;
					break;
				}
			}
			break;
			   
		}
		case _SCREEN_DSETTINGCOMM_SAVE_DLG_12:
		{
			break;
		}
	  case _SCREEN_DSETTINGCOMM_WIFI_SELECT_13:
		{
			
			break;
		} 
		case _SCREEN_CHANGE_PWD_14:
		{
//				char New_Level_1_pwd[PASSWORD_LEN];
//				char New_Level_2_pwd[PASSWORD_LEN];
//				char Input_Pwd[PASSWORD_LEN];
			switch (control_id)
			{ 
				case 2:
				{
					New_Level_1_pwd_Update=1;
					memset(New_Level_1_pwd, 0, PASSWORD_LEN); 
					sprintf((char *)New_Level_1_pwd,"%s",str);		
				}
				case 3:
				{
					
					New_Level_2_pwd_Update=1;
					memset(New_Level_2_pwd, 0, PASSWORD_LEN); 
					sprintf((char *)New_Level_2_pwd,"%s",str);		
				}
			}
			break;
		}
		case _SCREEN_INPUT_PWD_15:
		{
				switch (control_id)
			{ 
				case 2:
				{
					
					memset(Input_Pwd, 0, PASSWORD_LEN); 
					sprintf((char *)Input_Pwd,"%s",str);		
				}
				default:
					break;
			}
			break;
		}
		
	}
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
	if(screen_id==5&&control_id==2)//�������
	{
		//test_value = value;

		//SetProgressValue(5,1,test_value); //���½�������ֵ
	}
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
	Sys_RT_Status.System_RTC.year=RTC_BCD_TO_TEN(year);
	Sys_RT_Status.System_RTC.month=RTC_BCD_TO_TEN(month);
	Sys_RT_Status.System_RTC.week=RTC_BCD_TO_TEN(week);
	Sys_RT_Status.System_RTC.day=RTC_BCD_TO_TEN(day);
	Sys_RT_Status.System_RTC.hour=RTC_BCD_TO_TEN(hour);
	Sys_RT_Status.System_RTC.minute=RTC_BCD_TO_TEN(minute);
	Sys_RT_Status.System_RTC.second=RTC_BCD_TO_TEN(second)+1; 
}

/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//ָ������
	uint8_t ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8_t control_type = msg->control_type;//�ؼ�����
	uint16_t screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16_t control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	uint32_t value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://����ʱ�ؼ�
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}
