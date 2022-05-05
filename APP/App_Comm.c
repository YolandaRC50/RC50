#include "App_Comm.h" 


#define COMMUNICATION_MODE 0//0:Ethernet 1: wifi 2:Bluetooth 
 #if 1 //参数初始化
  
 #endif
 
 int IPStrtoArr(const char* IPAddrStr,ip_addr_t* voIPAddrArr)
 {
	 //ip_addr_t _IPaddrARR;
	 int ref= ipaddr_aton(IPAddrStr,voIPAddrArr);
	 return ref;
 }
 
	#if 1 //Init 
	
void communication_cfg_to_device()
{
	ipaddr_aton(Sys_Params.IOT_Params.Ethernet_Params.IP_Address,&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Loca_IPAddr);
	ipaddr_aton(Sys_Params.IOT_Params.Server_Params.Server_IPAddress,&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Server_IPAddr);
	ipaddr_aton(Sys_Params.IOT_Params.Ethernet_Params.Gate_way,&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Gate_way);
	ipaddr_aton(Sys_Params.IOT_Params.Ethernet_Params.Net_Mask,&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Net_Mask);  

}
//通讯参数初始化
void Communication_Param_Init()
{
	communication_cfg_to_device();
	
	Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8 = Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
	Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8=Sys_RT_Status.RT_IOT_Status.Send_Buffer;
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{ 
		case WIFI_MODE: 
		case BLUETOOTH_MODE:
		{
			
			Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l =&Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Recive_Buffer_Len;
			Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =&Sys_RT_Status.RT_IOT_Status.WIFI_Bluetooth_Status.Send_Buffer_Len;
			break;
		}
		default:
		{  
			Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l =&Sys_RT_Status.RT_IOT_Status.Ethernet_Status.r_l;
			Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l =&Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_l;
			break;
		}
			
	}
}
//发送参数
void communication_send()
{
	WIFI_BLUETOOTH_DMA_UART *p_Struct_DMA_Uart=&Wifi_Bluetooth_DMA_Uart;
	
	switch(Sys_Params.IOT_Params.Commication_Mode )
	{ 
		case WIFI_MODE:
		{
			memcpy(p_Struct_DMA_Uart->Txd_Fifo,
			       Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, 
			      *Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l);
			
			p_Struct_DMA_Uart->Txd_L = *Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l; 
			Wifi_Send_Dma(p_Struct_DMA_Uart);
		break;
		}
		case BLUETOOTH_MODE:
		{
			memcpy(p_Struct_DMA_Uart->Txd_Fifo,
			       Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8, 
			      *Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l);
			
			p_Struct_DMA_Uart->Txd_L = *Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_out_u8_l; 
			Bluetooth_Send_Dma(p_Struct_DMA_Uart);
		break;
		}
		default: 
		{
			Sys_RT_Status.RT_IOT_Status.Ethernet_Status.t_flag =1;
			break;
		}
	}
}	
//-1:无数据返回 1有数据返回
int communication_back_poll()
{
	if( *Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.p_in_u8_l >0 )
	{
		//*p_Struct_Communication->struct_data.p_in_u8_l=0;
		return 1;
	}
	else
		return -1;
}

//解析蓝牙名称 和 IP
void string_handle()
{
	//Struct_Bluetooth *p_struct_blue =p_Struct_Bluetooth;
	char *p_array_char;
	char *p_u8;
	char array_char_buf[60];
	char array_char_buf1[60];
	uint16_t num= Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.scan_num;
	memset(array_char_buf,  0, 60);
	memset(array_char_buf1, 0, 60);
	uint16_t length;
	p_u8 =Sys_RT_Status.RT_IOT_Status.Recive_Buffer;
	{
		lable0:
		p_u8=strstr( p_u8, "+INQ" );
		if ( p_u8 !=NULL )
		{
				p_u8=strstr( p_u8, ":" ); p_u8++;
				length = strstr( p_u8, "," )
							          -p_u8;
				memcpy(array_char_buf,p_u8,length);
				strcat( array_char_buf,"\r\n\0" );
				array_char_buf[4]=',';
				array_char_buf[7]=',';
			
			  p_u8=strstr( p_u8, "," ); p_u8++;
        p_u8=strstr( p_u8, "," ); p_u8++;
				p_u8=strstr( p_u8, "," ); p_u8++;	
				length = strstr( p_u8, "\r\n" )
											  -p_u8;
				memcpy(array_char_buf1,p_u8,length);
				//strcat( array_char_buf,"\r\n" );
			
				num++;
			  strcpy( Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_ip[num],  array_char_buf);
			  strcpy( Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_name[num],  array_char_buf1);
			  memset(array_char_buf,  0, 60);
				memset(array_char_buf1, 0, 60);
			goto lable0;
		}
	}
	Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.scan_num =num;
	//puts( "\r\n<<array_data>>\r\n");
	//printf("num:%d\r\n", num);
	for( uint16_t i=1; i<=num; i++ )
	{
		puts( Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_ip[i] );
		puts( Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.array_name[i] );
	}
}


#endif
