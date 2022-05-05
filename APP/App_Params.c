#include "APP_Params.h"
#include "w25qxx.h"
 
const uint16_t PARAMS_LEN = sizeof(Sys_Params)+4; 
const int PARAMS_START_ADDR=0;
const uint8_t BUFFER_HEADER=0x55;
	
uint8_t m_Params_Buffer[PARAMS_LEN];
System_RT_Status Sys_RT_Status;
System_Params Sys_Params=
{ 
	
	
	/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1) /*Commication_Mode*/  
		.IOT_Params.Commication_Mode= ETHERNET_MODE,
		#endif
		#if(1) /*Loca_Params*/  
		.IOT_Params.Loca_Params.Loca_Name="ISD II NAE",
		.IOT_Params.Loca_Params.Loca_IDH="ISD II IDH",
		.IOT_Params.Loca_Params.Loca_SN="ISD II SN", 
		#endif
		#if(1) /*Server_Params*/ 
	  .IOT_Params.Server_Params.Server_IPAddress="192.168.1.250",
		.IOT_Params.Server_Params.Sever_Port=8088,
		#endif
		#if(1) /*Ethernet_Params*/   
		.IOT_Params.Ethernet_Params.IP_Address="192.168.1.101",
		.IOT_Params.Ethernet_Params.Gate_way="192.168.1.1",
		.IOT_Params.Ethernet_Params.Net_Mask="255.255.255.0",
		#endif
		#if(1) /*WIFI_Params*/  
		.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name="Rody_PLC_LINK",
		.IOT_Params.WIFI_Params.Wlan_Password="18980928898", 
		#endif
		#if(1) /*Bluetooth_Params*/ 
		.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password="1234",
		#endif
		 
};
 
/* 
将浮点数f转化为4个字节数据存放在byte[4]中 
*/  
 void Float_to_Byte(float f,uint8_t byte[],uint16_t *index)  
{  
    FloatLongType fl;  
    fl.fdata=f;  
    byte[*index+0]=(unsigned char)fl.ldata;  
    byte[*index+1]=(unsigned char)(fl.ldata>>8);  
    byte[*index+2]=(unsigned char)(fl.ldata>>16);  
    byte[*index+3]=(unsigned char)(fl.ldata>>24);  
		*index+=4;
}  
/* 
将4个字节数据byte[4]转化为浮点数存放在*f中 
*/  
float Byte_to_Float(uint8_t byte[],uint16_t *index) 
{  
	float  *f;
  FloatLongType fl;  
	if(byte[*index+0]==0xFF
		&&byte[*index+1]==0xFF
		&&byte[*index+2]==0xFF
		&&byte[*index+3]==0xFF)
		{
			 *f=0.0f;
			 return *f;
		}
    fl.ldata=0;  
    fl.ldata=byte[*index+3];  
    fl.ldata=(fl.ldata<<8)|byte[*index+2];  
    fl.ldata=(fl.ldata<<8)|byte[*index+1];  
    fl.ldata=(fl.ldata<<8)|byte[*index+0];  
    *f=fl.fdata; 
		*index+=4;
		return *f;
}  


uint16_t Byte_to_int(uint8_t byte[],uint16_t *index)
{ 

	int16_t _u16= (byte[*index])+(byte[*index+1]<<8);
	*index+=2;
	return _u16; 
}
/* 
将Byte字节转换为字符数组
*/  
uint16_t Byte_to_String(uint8_t byte[],char strArr[],uint16_t *index,uint8_t ArrayLen)
{ 
	for(uint16_t Len=0;Len<ArrayLen;Len++)
	{
		strArr[Len]=  byte[*index+Len];
		if(strArr[Len]=='\0')
			break;
	}
	
	*index+=ArrayLen;
	return ArrayLen;
}


void Int_to_Byte(uint16_t viValue,uint8_t byte[],uint16_t *index)
{ 
	byte[*index]=viValue;
	byte[*index+1]=viValue>>8;
	*index+=2; 
}




int16_t String_to_Byte(char strArr[],uint8_t byte[],uint16_t *index,uint8_t ArrayLen)
{  
	for(uint16_t Len=0;Len<ArrayLen;Len++)
	{
		 byte[*index+Len]=strArr[Len];
		if(strArr[Len]=='\0')
			break;
	} 
	*index+=ArrayLen;
	return ArrayLen;
}


void ResetAllParams()
{
	Sys_Params.Channel_Param[CH1].Run_Mode_Params=CONTINUE;
	Sys_Params.Channel_Param[CH1].Timer=10.0f;
	Sys_Params.Channel_Param[CH1].Head_Time=0.0f;
	Sys_Params.Channel_Param[CH1].Tail_Time=0.0f;
	
	Sys_Params.Channel_Param[CH2].Run_Mode_Params=CONTINUE;
	Sys_Params.Channel_Param[CH2].Timer=10.0f; 
	Sys_Params.Channel_Param[CH2].Head_Time=0.0f;
	Sys_Params.Channel_Param[CH2].Tail_Time=0.0f;
	
	
	
	Sys_Params.Pressure_Param.TargetPreessure=0;
	Sys_Params.Pressure_Param.TargetPreessure_Range=0;
	
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=1024.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=4096.0f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=9.0f;
	
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=2048.0f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=0.001f;
	Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=5.0f;
	Sys_Params.Pressure_Param.Pressure_Unit= BAR; 
	
	
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=4096;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=50.0;
	
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=4096;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=0.0;
	Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=100.0;
	
	Sys_Params.Low_to_Empty_Counter=10;
	Sys_Params.LowSensor_Enable=POWER_ON;
	
	#if(1) /*IOT参数*/
		/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1) /*Commication_Mode*/ 
		Sys_Params.IOT_Params.Commication_Mode= ETHERNET_MODE;
		#endif
		#if(1) /*Loca_Params*/ 
		#endif
		#if(1) /*Server_Params*/ 
		Sys_Params.IOT_Params.Server_Params.Sever_Port=8088;
		#endif
		#if(1) /*Ethernet_Params*/ 
		#endif
		#if(1) /*WIFI_Params*/ 
		#endif
		#if(1) /*Bluetooth_Params*/  
		#endif
		
	#endif
}
void LoadSystemParams()
{
	W25QXX_Read(m_Params_Buffer,PARAMS_START_ADDR,PARAMS_LEN);
	
	if(m_Params_Buffer[0]==BUFFER_HEADER &&m_Params_Buffer[1]==BUFFER_HEADER)
	{
		uint16_t m_Index=2;
		
		Sys_Params.Channel_Param[CH1].Run_Mode_Params=Byte_to_int(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH1].Timer=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH1].Head_Time=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH1].Tail_Time=Byte_to_Float(m_Params_Buffer,&m_Index); 
		
		Sys_Params.Channel_Param[CH2].Run_Mode_Params=Byte_to_int(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH2].Timer=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH2].Head_Time=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Channel_Param[CH2].Tail_Time=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		
		Sys_Params.Pressure_Param.TargetPreessure=Byte_to_Float(m_Params_Buffer,&m_Index); 
		Sys_Params.Pressure_Param.TargetPreessure_Range=Byte_to_Float(m_Params_Buffer,&m_Index); 
		 
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		Sys_Params.Pressure_Param.Pressure_Unit= Byte_to_int(m_Params_Buffer,&m_Index) ; 
		
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value=Byte_to_Float(m_Params_Buffer,&m_Index);
		
		Sys_Params.Low_to_Empty_Counter=Byte_to_int(m_Params_Buffer,&m_Index); 
		Sys_Params.LowSensor_Enable=Byte_to_int(m_Params_Buffer,&m_Index);  
		
		
	#if (1==1) /*IOT参数*/
		/*
		IOT_COMMICATION_MODE_ENUM Commication_Mode;
		IOT_Loca_Params Loca_Params;
		IOT_Server_Params Server_Params;
		IoT_Ethernet_Params Ethernet_Params;
		IoT_WIFI_Params WIFI_Params;
		IoT_Bluetooth_Params Bluetooth_Params;
		*/
		
		#if(1==1) /*Commication_Mode*/ 
		Sys_Params.IOT_Params.Commication_Mode= Byte_to_int(m_Params_Buffer,&m_Index); 
		#endif
		
//		#define DEVICE_NAME_LEN 20
//		#define IP_ADDRESS_LEN  20
//		#define PASSWORD_LEN    20 

		#if (1==1) /*Loca_Params*/ 
		
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_Name ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_IDH ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Loca_Params.Loca_SN ,&m_Index,DEVICE_NAME_LEN);
		#endif
		#if (1==1) /*Server_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Server_Params.Server_IPAddress ,&m_Index,IP_ADDRESS_LEN);		
		Sys_Params.IOT_Params.Server_Params.Sever_Port=Byte_to_int(m_Params_Buffer,&m_Index);
		#endif
		#if (1==1) /*Ethernet_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.IP_Address ,&m_Index,IP_ADDRESS_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.Gate_way ,&m_Index,IP_ADDRESS_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Ethernet_Params.Net_Mask ,&m_Index,IP_ADDRESS_LEN);
		#endif
		#if (1==1) /*WIFI_Params*/ 
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name ,&m_Index,DEVICE_NAME_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.WIFI_Params.Wlan_Password ,&m_Index,PASSWORD_LEN);
		#endif
		#if (1==1) /*Bluetooth_Params*/  
		Byte_to_String(m_Params_Buffer,Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password ,&m_Index,PASSWORD_LEN);
		#endif
		
		#if (1==1) /*Login_Params*/  
		Byte_to_String(m_Params_Buffer,Sys_Params.Login_Pwd.First_Password ,&m_Index,LOGIN_PWD_LEN);
		Byte_to_String(m_Params_Buffer,Sys_Params.Login_Pwd.Second_Password ,&m_Index,LOGIN_PWD_LEN);
		#endif
	#endif
	}
	else
	{
		ResetAllParams();
	}
}
void SaveSystemParams()
{
	uint16_t m_Index=0;
	m_Params_Buffer[m_Index++]=BUFFER_HEADER;
	m_Params_Buffer[m_Index++]=BUFFER_HEADER;

	Int_to_Byte(Sys_Params.Channel_Param[CH1].Run_Mode_Params,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH1].Timer,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH1].Head_Time,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH1].Tail_Time,m_Params_Buffer,&m_Index); 
	
	
	Int_to_Byte(Sys_Params.Channel_Param[CH2].Run_Mode_Params,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH2].Timer,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH2].Head_Time,m_Params_Buffer,&m_Index);
	Float_to_Byte(Sys_Params.Channel_Param[CH2].Tail_Time,m_Params_Buffer,&m_Index);
	
	Float_to_Byte(Sys_Params.Pressure_Param.TargetPreessure,m_Params_Buffer,&m_Index);//9.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.TargetPreessure_Range,m_Params_Buffer,&m_Index);//9.0f;


	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value,m_Params_Buffer,&m_Index);//9.0f;


	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//1024.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value,m_Params_Buffer,&m_Index);//4096.0f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value,m_Params_Buffer,&m_Index);//0.001f;
	Float_to_Byte(Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value,m_Params_Buffer,&m_Index);//9.0f;

	
	Int_to_Byte(Sys_Params.Pressure_Param.Pressure_Unit,m_Params_Buffer,&m_Index);
	
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//4096;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Temperature_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//50.0;

	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//4096;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//0.0;
	Float_to_Byte(Sys_Params.Temperature_Humidity_Params.Humidity_Calibration_Param.Analog_1_Value,m_Params_Buffer,&m_Index);//100.0;
	
	
	Int_to_Byte(Sys_Params.Low_to_Empty_Counter,m_Params_Buffer,&m_Index);//100.0; 
	Int_to_Byte(Sys_Params.LowSensor_Enable,m_Params_Buffer,&m_Index);//100.0; 
	
	#if (1==1) /*IOT Paeams*/
		#if(1) /*Commication_Mode*/  
		Int_to_Byte(Sys_Params.IOT_Params.Commication_Mode,m_Params_Buffer,&m_Index);
		#endif
				
//		#define DEVICE_NAME_LEN 20
//		#define IP_ADDRESS_LEN  20
//		#define PASSWORD_LEN    20 
		#if (1==1) /*Loca_Params*/ 
		
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_Name ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_IDH ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);
		String_to_Byte (Sys_Params.IOT_Params.Loca_Params.Loca_SN ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN); 
		
		 
		#endif
		#if (1==1) /*Server_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.Server_Params.Server_IPAddress ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		Int_to_Byte(Sys_Params.IOT_Params.Server_Params.Sever_Port,m_Params_Buffer,&m_Index); 
		#endif
		#if (1==1) /*Ethernet_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.IP_Address ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.Gate_way ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.Ethernet_Params.Net_Mask ,m_Params_Buffer,&m_Index,IP_ADDRESS_LEN);  
		#endif
		#if (1==1) /*WIFI_Params*/  
		String_to_Byte (Sys_Params.IOT_Params.WIFI_Params.Wlan_SwitchHub_Name ,m_Params_Buffer,&m_Index,DEVICE_NAME_LEN);  
		String_to_Byte (Sys_Params.IOT_Params.WIFI_Params.Wlan_Password ,m_Params_Buffer,&m_Index,PASSWORD_LEN); 
		#endif
		#if (1==1) /*Bluetooth_Params*/   
		String_to_Byte (Sys_Params.IOT_Params.Bluetooth_Params.Bluetooth_Match_Password ,m_Params_Buffer,&m_Index,PASSWORD_LEN); 
		#endif
		
		#if (1==1) /*Login_Params*/  
		String_to_Byte (Sys_Params.Login_Pwd.First_Password ,m_Params_Buffer,&m_Index,LOGIN_PWD_LEN); 
		String_to_Byte (Sys_Params.Login_Pwd.Second_Password,m_Params_Buffer,&m_Index,LOGIN_PWD_LEN);  
		#endif
		
	#endif	
	  
	 
	 W25QXX_Write(m_Params_Buffer,PARAMS_START_ADDR,PARAMS_LEN);
	 
	 uint8_t __ReadBuffer[PARAMS_LEN];
	 W25QXX_Read(__ReadBuffer,PARAMS_START_ADDR,PARAMS_LEN);
	 POWER_STATUS __RT=POWER_ON;
	 for(uint16_t _Index=0;_Index<PARAMS_LEN;_Index++)
	 {
		 if(__ReadBuffer[_Index]==m_Params_Buffer[_Index])
		 {
			 continue;
		 }
		 else
		 {
			 __RT=POWER_OFF;
			 break;
		 }
	 }
	  
}

POWER_STATUS Match_Pwd(uint8_t viLevel)
{
	POWER_STATUS Login_Status=POWER_OFF;
	if(viLevel==0||viLevel==1)
	{
		for(int i=0;i<LOGIN_PWD_LEN;i++)
		{
			if(Sys_RT_Status.Login_Pwd.First_Password[i]!=Sys_Params.Login_Pwd.First_Password[i])
				break;
			if(i==LOGIN_PWD_LEN-1)
				Login_Status=POWER_ON;
		}
	}
	else if(viLevel==2)
	{
		for(int i=0;i<LOGIN_PWD_LEN;i++)
		{
			
			if(Sys_RT_Status.Login_Pwd.Second_Password[i]!=Sys_Params.Login_Pwd.Second_Password[i])
				break;
			if(i==LOGIN_PWD_LEN-1)
				Login_Status=POWER_ON;
		}
	} 
	
	return Login_Status;
		
}

