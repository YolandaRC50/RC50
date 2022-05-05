#ifndef __APP_PARAMS_H
#define __APP_PARAMS_H 
#include "stm32f4xx.h" 
#include "lwip/api.h"
#define __PACKED_LEN 4

#define DEVICE_NAME_LEN 20
#define IP_ADDRESS_LEN  20
#define PASSWORD_LEN    20 
#define IOT_SENDANDRECEIVE_BUFFER_LEN 512
#define BLUETOOTH_MAX_CONNECT_NUM 9
#define LOGIN_PWD_LEN   20

#if(1) /*ENUM*/

/* 
要点提示: 
1. float和unsigned long具有相同的数据结构长度 
2. union据类型里的数据存放在相同的物理空间 
*/  
typedef union  __FLOATLONGTYPE
{  
    float fdata;  
    unsigned long ldata;  
}FloatLongType; 


 
	#if (1)  /*ENUM*/
typedef enum __IOT_COMMICATION_MODE_ENUM
	{
		ETHERNET_MODE=0,
		WIFI_MODE=1,
		BLUETOOTH_MODE=2
	}IOT_COMMICATION_MODE_ENUM;
		
	#endif	

/*--->
系统语言
-->*/
typedef enum __SYSTEM_LANGUAGE
{
	
	 LANG_ENGLISH=0,
	 LANG_CHINESE=1,
 } SYSTEM_LANGUAGE;
typedef enum __CH_DISPENSING_STEP
{
	DISPENSING_STOP=0,
	HEAD_DELAY=1,
	DISPENSING_STATUS=2,
	TAIL_DELAY=3
}CH_DISPENSING_STEP;
typedef enum __SYSTEM_CHANNEL
{
	
	 CH1=0,
	 CH2=1,
 } System_Channel;
typedef enum __PRESSURE_UNIT_MODE
{
	BAR=0,
	PSI=1,
} PRESSURE_UNIT;
 typedef enum __SYSTEM_OPTION_MODE
 {
	 STOP_MODE=0,
	 AUTORUN_MODE=1,
 }SYSTEM_OPTION_MODE;
typedef enum __POWER_STATUS
{
	POWER_ON=1,
	POWER_OFF=!POWER_ON,
}POWER_STATUS;
 
typedef enum __CARTRIDGE_STATUS
	{
	FULL=0,
	LOWER=1,
	EMPTY=2,
} Cartridge_Status;

typedef enum __SYSTEM_STATUS{
	READY=0,
	DISPENSING=1,
	STOP=2,
	ALM=3,
} System_Status;

typedef enum __LOG_TYPE{
	MACHINE_INFO_LOG=0,
	RUN_STOP_LOG=1,
	ERROR_MESSAGE_LOG=2,
	PARAMETER_LOG=3
}Log_Type;

typedef enum __RUN_MODE
{
	CONTINUE=0,
	TIMER=1, 
}RUN_MODE;

#endif

#if(1) /*Struct*/
		//typedef struct __LOG_PATH_ITEM{
		//	Log_Type Log_Type;
		//	char Log_Path[20];
		//}Log_Path_Item;

		//typedef struct __LOG_PATH_LIST{
		//	Log_Path_Item Machine_info_log;
		//	Log_Path_Item Run_stop_log;
		//	Log_Path_Item Error_message_log;
		//	Log_Path_Item Parameter_log;
		//} Log_Path_List;

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct _SYSTEM_RTC
		{
		float Analog_1_Value;
		float RT_1_Value;
		float Analog_2_Value;
		float RT_2_Value;

		} Calibration_Params;

		#pragma pack()

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct _CALIBRATION_PARAMS
		{
		uint8_t year;
		uint8_t month;
		uint8_t week;
		uint8_t day;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;

		} System_RTC;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __CHANNEL_PARAMS_ITEM
		{
		RUN_MODE Run_Mode_Params;
		float Timer;
		float Head_Time;
		float Tail_Time;

		} Channel_Params_Item;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __PRESSURE_PARAMS
		{
		Calibration_Params Calibration_RT_Param;
		Calibration_Params Calibration_Set_Param;
		PRESSURE_UNIT Pressure_Unit;
		float TargetPreessure;
		float TargetPreessure_Range;
		float Valve_Percent;//0..100%
		 
		} Pressure_Params; 


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __TEMPERATURE_HUMIDITY
		{ 
		Calibration_Params Temperature_Calibration_Param;
		Calibration_Params Humidity_Calibration_Param;

		} Temperature_Humidity;


		#pragma pack()
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __CH_DISPENSING_STATUS
		{
		CH_DISPENSING_STEP CH_Dispensing_Step; 
		uint64_t HEAD_COUNT_DOWN;
		uint64_t DISPENSING_COUNT_DOWN;
		uint64_t TAIL_COUNT_DOWN;
		} CH_DISPENSING_STATUS;
		#pragma pack() 
		#endif


		#if (1)  /*IOT_PARAMS*/
		 
		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_WIFI_PARAMS   //40
		{	 
			/*交换机名称*/
			char Wlan_SwitchHub_Name[DEVICE_NAME_LEN]; 		 //20
			/*登录密码*/ 
			char Wlan_Password[PASSWORD_LEN];  //20
		} IoT_WIFI_Params;


		#pragma pack()


		/*-------------------------------------*/

		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_BLUETOOTH_PARAMS //20
		{ 
			/*配对密码*/
			char Bluetooth_Match_Password[PASSWORD_LEN]; //20
		} IoT_Bluetooth_Params; 
		 
		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_ETHERNET_PARAMS //60
		{ 
			/*IP地址*/
			char IP_Address[IP_ADDRESS_LEN]; 	 //20
			/*网关*/
			char Gate_way[IP_ADDRESS_LEN];  //20
			/*子网掩码*/ //20
			char Net_Mask[IP_ADDRESS_LEN];   //20
			
			
			
		} IoT_Ethernet_Params;


		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_SERVER_PARAMS //24
		{
			/*服务器IP地址*/
			char Server_IPAddress[IP_ADDRESS_LEN]; //20
			/*服务器端口号*/
			uint16_t Sever_Port;  //2
			
		} IOT_Server_Params;
			

		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __IOT_LOCA_PARAMS //60
		{
			/*本机名称*/
			
			char Loca_Name[DEVICE_NAME_LEN]; //20
			char Loca_IDH[DEVICE_NAME_LEN]; //20
			char Loca_SN[DEVICE_NAME_LEN];  //20
			 
		} IOT_Loca_Params;



		#pragma pack()


		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_IOT_PARAMS
		{
			IOT_Loca_Params Loca_Params; //60
			IOT_Server_Params Server_Params;//24
			IoT_Bluetooth_Params Bluetooth_Params;
			IoT_WIFI_Params WIFI_Params;
			IoT_Ethernet_Params Ethernet_Params;
			IOT_COMMICATION_MODE_ENUM Commication_Mode;  //2
		} System_IOT_Params;


		#pragma pack() 


		#endif
		/*-------------------------------------*/
		
		#if(1) /*Login_PARAMS*/
		#pragma pack(__PACKED_LEN)
		typedef struct __LOGIN_PWD
		{
			 /*交换机名称*/
			char First_Password[LOGIN_PWD_LEN]; 		 //20
			/*登录密码*/ 
			char Second_Password[LOGIN_PWD_LEN];  //20
			
		} LOGIN_PWD;
		#pragma pack() 
		#endif

		#if(1)  /*IOT_STSTUS*/
		typedef struct __RT_WIFI_SEARCH
		{
			uint8_t Ecn; //模式
			char Ssid[DEVICE_NAME_LEN]; //热点名称
			int16_t Rssi;    //信号强度
		} RT_Wifi_Search;		
		 
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_WIFI_STATUS
		{ 
			RT_Wifi_Search Wifi_Search_List[5]; 
			int16_t Status_Step;
		} RT_WIFI_Status;
		
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_BLUETOOTH_STATUS
		{
			#if (1)
			char array_name[BLUETOOTH_MAX_CONNECT_NUM][DEVICE_NAME_LEN];
			char array_ip[BLUETOOTH_MAX_CONNECT_NUM][DEVICE_NAME_LEN];// 1-8 共8个
			uint8_t scan_num;//扫描到的蓝牙个数
			uint8_t scan_use_num;//使用的蓝牙 
			char array_connect_name[DEVICE_NAME_LEN];
			char array_connect_ip[IP_ADDRESS_LEN];// 1-8 共8个
			char array_password[PASSWORD_LEN];//配对密码
			#endif
			uint16_t Command;
			int16_t  Status;
		} RT_Bluetooth_Status;
		 
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_WIFI_BLUETOOTH_STATUS
		{
			char *p_Recive_Buffer;  
			uint16_t Recive_Buffer_Len; //接收缓存已用长度
			uint16_t Send_Buffer_Len;		//发送缓存已用长度
			
		}RT_WIFI_Bluetooth_Status;
		
		
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __RT_ETHERNET_STATUS
		{
				struct netconn *tcp_clientconn;	//TCP CLIENT网络连接结构体
				uint16_t r_l;
				uint16_t r_flag; 
				uint16_t t_l;
				uint16_t t_flag;
				char tcp_client_flag;		//TCP客户端数据发送标志位 
				struct pbuf *q;
			
				uint8_t flag_send;
			
			
			
				uint8_t mac[6];      //MAC地址
				uint8_t dhcpstatus;	//dhcp状态 
												//0,未获取DHCP地址;
												//1,进入DHCP获取状态
												//2,成功获取DHCP地址
												//0XFF,获取失败.
		} RT_Ethernet_Status;
		#pragma pack()
		
		#pragma pack(__PACKED_LEN)
		typedef	struct __RT_COMM_STATUS_DATA
		{
			char  			*p_in_u8;//接收到的数据
			char  			*p_out_u8;
			uint16_t 		*p_in_u8_l;//接收数据的长度
			uint16_t 		*p_out_u8_l;
			
			ip_addr_t   Server_IPAddr;
			ip_addr_t   Loca_IPAddr; 
			ip_addr_t 	Gate_way;  //网关
			ip_addr_t 	Net_Mask;   //掩码
			uint16_t  	Ethernet_Loca_Port,WIFI_Loca_Port;
		}RT_Comm_Status_Data;
		#pragma pack()
	
	
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_RT_IOT_STATUS
		{
			
			RT_WIFI_Status WIFI_Status;
			RT_Bluetooth_Status Bluetooth_Status;
			RT_Ethernet_Status Ethernet_Status; 
			POWER_STATUS Connect_Status;
			char Recive_Buffer[IOT_SENDANDRECEIVE_BUFFER_LEN];
			char Send_Buffer[IOT_SENDANDRECEIVE_BUFFER_LEN];		 
			RT_Comm_Status_Data Comm_Status_Data;
			
			RT_WIFI_Bluetooth_Status WIFI_Bluetooth_Status;
		} System_RT_IOT_Status;
		
	 
		#pragma pack() 
		#endif
 
 
/*-------------------------------------*/
		#pragma pack(__PACKED_LEN)
		typedef struct __SYSTEM_RT_STATUS
		{
			Cartridge_Status Cartridge_Status;
			System_Status System_Status;
			System_Status CH_Status[2];
			float Temperature_RT;
			float Humidity_RT;
			float RT_Pressure;
			float RT_AN_Pressure;
			POWER_STATUS Pressure_Set_Cal_0Bar_Enable;
			POWER_STATUS Pressure_Set_Cal_5Bar_Enable;
			uint16_t Lower_Empty_Counter;
			
			POWER_STATUS System_Run_Mode;
			POWER_STATUS CH_Manuan_Run_Mode[2]; 
			POWER_STATUS Data_Summary_Run_Mode;
			
			CH_DISPENSING_STATUS CH_Dispensing_Status[2];
			SYSTEM_OPTION_MODE System_Option_Mode;
			
			System_RTC System_RTC;	 
			System_RT_IOT_Status RT_IOT_Status;
			
			LOGIN_PWD Login_Pwd ;
		} System_RT_Status;

		#pragma pack()

		/*-------------------------------------*/
		#pragma pack(__PACKED_LEN) 
		typedef struct __SYSTEM_PARAMS
		{ 
			//下面是参数，需要保存的
			Channel_Params_Item Channel_Param[2];
			Pressure_Params Pressure_Param;
			Temperature_Humidity Temperature_Humidity_Params;
			int16_t Low_to_Empty_Counter;
			POWER_STATUS LowSensor_Enable; 
			System_IOT_Params IOT_Params;
			LOGIN_PWD Login_Pwd ;
		} System_Params;
		#pragma pack()
		/*-------------------------------------*/
 

extern System_RT_Status Sys_RT_Status;
extern System_Params Sys_Params;

void LoadSystemParams();
void SaveSystemParams();
POWER_STATUS Match_Pwd(uint8_t viLevel);
#endif