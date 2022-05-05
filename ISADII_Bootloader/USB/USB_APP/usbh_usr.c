#include "usbh_usr.h" 
#include "ff.h" 
#include "usart.h" 
#include "exfuns.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "string.h"
#include "delay.h"
#include "iap.h"
#include "HMI_App.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "sys.h"
#include "Bell.h"
#include "hmi_driver.h"  

#define IMAGE_INDEX_GRAIDX				0x90 //下载图片索引档
#define	FONT_FBIN									0x91 //下载字库
#define	IMAGE_GWB									0x92 //下载图片数据
#define	FONT_FIDX 								0x93 //下载字库索引档 
#define SYSTEM_CONFIG_SI					0x97 //下载系统配置文件
#define SYSTEM_RESOURCES_SBIN 		0x98 //下载系统资源文件
#define BUF_LEN  (uint16_t)2048
uint8_t  Update_HMI_Status=0;
uint8_t  HMI_Update_Index=0;
u8 Detect_USB=0;
uint8_t jump_flag = 0;
static u8 AppState;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;
uint8_t answer = 0;
   
u8 update_buf[BUF_LEN]={0};
u8 updateRead_buf[BUF_LEN]={0};
u8 LoadImageIndex=0;
HMI_IMG_IDX hmi_Image_Index_Data[50]={0};
uint16_t MAX_INDEX=0;
char buffer[30] ;


u8 CheckHMIUpdateFiles(uint8_t viUpdateIndex)
{
	 
		FRESULT OpenRT;
		switch(viUpdateIndex)
		{
			case IMAGE_INDEX_GRAIDX:
			{
				//memcpy(warn_view, Path_str_List[0], 13); 
				OpenRT=f_open(file,"2:image.idx",FA_READ) ;
				//res = find_volume(&dj.fs, &path, (BYTE)(mode & ~FA_READ));
				break;
			}
			case FONT_FBIN:
			{
				OpenRT=f_open(file,"2:font.bin",FA_READ) ;
				break;
			}
			case IMAGE_GWB:
			{
				OpenRT=f_open(file,"2:image.bin",FA_READ) ;
				break;
			}
			case FONT_FIDX:
			{
				OpenRT=f_open(file,"2:font.idx",FA_READ) ;
				break;
			}
			case SYSTEM_CONFIG_SI:
			{
				OpenRT=f_open(file,"2:screen.bin",FA_READ) ;
				break;
			}
			case SYSTEM_RESOURCES_SBIN:
			{
				
				OpenRT=f_open(file,"2:system.bin",FA_READ) ;
				break;
			}
			defult:
			return 0;
			break;
		}
		
		if(OpenRT == FR_OK)
		{ 
			
			f_close(file);
			
			delay_ms(10);
			return 1;
		}
		
			return 0;
}

uint8_t  UpdateHMIByFileIndex(uint8_t viUpdateIndex)
{
		//#define IMAGE_INDEX_GRAIDX	0x90 //下载图片索引档
		//#define FONT_FBIN				0x91 //下载字库
		//#define IMAGE_GWD				0x92 //下载图片数据
		//#define FONT_FIDX 				0x93 //下载字库索引档 
		//#define SYSTEM_CONFIG_SI		0x97 //下载系统配置文件
		//#define SYSTEM_RESOURCES 		0x98 //下载系统资源文件
		 
		HMI_Update_Index=viUpdateIndex;
		
		FRESULT OpenRT;
		OpenRT=f_mount(fs[2],"2:",1); 	//挂载U盘  
		if(OpenRT!=FR_OK ) 
			 return 0;
		
		 
		switch(viUpdateIndex)
		{
			case IMAGE_INDEX_GRAIDX:
			{
				//memcpy(warn_view, Path_str_List[0], 13); 
				OpenRT=f_open(file,"2:image.idx",FA_READ) ;
				break;
			}
			case FONT_FBIN:
			{
				OpenRT=f_open(file,"2:font.bin",FA_READ) ;
				break;
			}
			case IMAGE_GWB:
			{
				OpenRT=f_open(file,"2:image.bin",FA_READ) ;
				break;
			}
			case FONT_FIDX:
			{
				OpenRT=f_open(file,"2:font.idx",FA_READ) ;
				break;
			}
			case SYSTEM_CONFIG_SI:
			{
				OpenRT=f_open(file,"2:screen.bin",FA_READ) ;
				break;
			}
			case SYSTEM_RESOURCES_SBIN:
			{
				
				OpenRT=f_open(file,"2:system.bin",FA_READ) ;
				break;
			}
			defult:
			return 0;
			break;
		}
	 
		
		delay_ms(10);
		
		uint8_t Update_Success=0; 
		if(OpenRT == FR_OK )
		{
			
			
			if(BeginUpdateHMI()!=1) //发送更新标志并检测回应
				Update_Success= 1;

			
			uint8_t cmd[] = {viUpdateIndex};
			if(Update_Success==0 )
				if( SendFiledData(cmd,1,1)==0)
				{
					f_close(file);
					
					delay_ms(1000); 
					Update_Success= 2;
				}
			
			uint8_t res=0;
			uint8_t precent; 
			uint32_t pos = 0;
			uint32_t  size = file->fsize; 
				
			
			 UINT OutBytesRead = BUF_LEN; 
			if(Update_Success==0)
			{ 
				
				uint8_t Finish_Mark=0;
				while(OutBytesRead==BUF_LEN)
				{
					u8 HeaterTX[2]={0};
					 OutBytesRead=0;
 
					unsigned int  len=sizeof(update_buf);
					
					if(Finish_Mark==0)
					{
						//读取数据
						FRESULT ReadRT ;  
						ReadRT= f_read(file, update_buf, len, (void *)&OutBytesRead); 
						
						if(ReadRT!=FR_OK )
						{   
							delay_ms(50); 
							Update_Success=3;
							break; 
						} 
					}
					else
					{
						OutBytesRead=0;
					}
					//设置读取到的文件数据长度
					HeaterTX[0]=(uint8_t)(OutBytesRead%256);
					HeaterTX[1]=(uint8_t)((OutBytesRead/256));
					
					 
					if(OutBytesRead<BUF_LEN)
					{
						for(int i=OutBytesRead;i<BUF_LEN;i++)
						{
							update_buf[i]=0x00;
						}
					}  
					
					SendFiledData(HeaterTX,2,0);
					if(SendFiledData(update_buf,BUF_LEN,1)!=1)
					{
						
						Update_Success=4;
						break;
					}
					 
					
					
					//delay_us(50);
					
					pos += OutBytesRead;
					precent = pos * 100 / file->fsize;  
					 
					//printf(" 文件升级已完成 %dKB, %4.2f%%...\r\n",pos/1024,(pos * 100.0f / file->fsize));
					printf("文件共%ldkb,已执行%dkb,已完成  %4.2f%%...\r\n", file->fsize/1024,pos/1024,(pos * 100.0f / file->fsize));
					if(Finish_Mark==1)
					{ 
						break;
					}
					if(pos>=size)
					{
						Finish_Mark=1;
						OutBytesRead=BUF_LEN;
					}
				}  
			}
			
			if( Update_Success==0)
			{
				if(EndUpdateHMI()!=1) //发送更新标志并检测回应
				{	
					Update_Success=5;
				}
			}
			 
			f_close(file); 
			
			
			delay_ms(10);
		}
		
		if(Update_Success==0)
		{ 
			printf("升级成功...\r\n");
		}
		else
		{
			printf("升级失败,失败代码%d...\r\n",Update_Success);
		}
		return (Update_Success==0);
}
 


uint8_t UpdateHMI_Image(uint16_t viImageIndex)
{
	FRESULT OpenRT,ReadRT;
	OpenRT=f_mount(fs[2],"2:",1); 	//挂载U盘  
	if(OpenRT!=FR_OK ) 
		 return 0;
	
	sprintf((char *)buffer,"2:%d.bin",viImageIndex);
	OpenRT=f_open(file,buffer,FA_READ) ;  
	if(OpenRT!=FR_OK ) 
		 return 0;
	
	
	printf("检索到图片文件%d.bin 文件...\r\n",viImageIndex);
	uint32_t Index_Size
	=(hmi_Image_Index_Data[viImageIndex].Img_Size_3<<16)
	+(hmi_Image_Index_Data[viImageIndex].Img_Size_2<<8)
	+hmi_Image_Index_Data[viImageIndex].Img_Size_1;
	
	uint32_t start_addr =hmi_Image_Index_Data[viImageIndex].Img_Start_Addr;
	
	
	uint16_t frist_pack_size=BUF_LEN-(start_addr%BUF_LEN);
	uint16_t last_pack_size=(Index_Size-frist_pack_size)%BUF_LEN;
	
	printf("开始升级%d.bin 文件...\r\n",viImageIndex);
	if(BeginUpdateHMI()!=1) //发送更新标志并检测回应  
	{
		
		f_close(file); 
		delay_ms(1000); 
		return 0;
	}   
 
	uint8_t cmd[3] = {0x96,viImageIndex%256,viImageIndex/256}; 
	if( SendFiledData(cmd,3,1)==0)
	{
		
		f_close(file); 
		delay_ms(1000); 
		return 0;
	}   
	
	
	uint8_t res=0;
	uint8_t precent; 
	uint32_t pos = 0;
	uint32_t  size = file->fsize; 
	 UINT OutBytesRead = BUF_LEN; 
	
	 u8 HeaterTX[2]={0};
	 OutBytesRead=0; 
	
	 #if (0==1) /*Send frist_pack*/
	ReadRT= f_read(file, updateRead_buf, frist_pack_size, (void *)&OutBytesRead); 
	 for(uint16_t index=0;index<BUF_LEN;index++)
	 {
		 if(index<frist_pack_size)
			update_buf[index]=updateRead_buf[index];
		 else
			update_buf[index]=0; 
	 } 
	
	//设置读取到的文件数据长度
	HeaterTX[0]=(uint8_t)(OutBytesRead%256);
	HeaterTX[1]=(uint8_t)((OutBytesRead/256));
	
	SendFiledData(HeaterTX,2,0);
	if(SendFiledData(update_buf,BUF_LEN,1)!=1)
	{
		
		f_close(file); 
		delay_ms(1000); 
		return 0;
	}
	pos+=OutBytesRead;
	#endif
	
	uint8_t Update_Success=0; 
	OutBytesRead = BUF_LEN; 
	uint8_t frist_pack_Read=0;
	
	uint8_t Finish_Mark=0;
	while(OutBytesRead == BUF_LEN)
	{
		
		unsigned int  len=sizeof(update_buf);
		if(frist_pack_Read==0)
		{
			len=frist_pack_size;
		}
		
		if(Finish_Mark==0)
		{
			//读取数据 
			ReadRT= f_read(file, update_buf, len, (void *)&OutBytesRead); 
			 
			if(ReadRT!=FR_OK )
			{   
				delay_ms(50); 
				Update_Success=3;
				break; 
			} 
		}
		else
		{
			OutBytesRead=0;
		}
		//设置读取到的文件数据长度
		HeaterTX[0]=(uint8_t)(OutBytesRead%256);
		HeaterTX[1]=(uint8_t)((OutBytesRead/256));
		
		  
		if(OutBytesRead<BUF_LEN)
		{
			for(int i=OutBytesRead;i<BUF_LEN;i++)
			{
				update_buf[i]=0x00;
			}
		}  
		
		SendFiledData(HeaterTX,2,0);
		if(SendFiledData(update_buf,BUF_LEN,1)!=1)
		{
			
			Update_Success=4;
			break;
		}
		 
		
		
		//delay_us(50);
		
		pos += OutBytesRead;
		precent = pos * 100 / file->fsize; 
		 
		printf("%d.bin 文件共%ldkb,已执行%dkb,已完成  %4.2f%%...\r\n",viImageIndex, file->fsize/1024,pos/1024,(pos * 100.0f / file->fsize));
		if(frist_pack_Read==0)
		{
			OutBytesRead=BUF_LEN;
			frist_pack_Read=1;
		}
		 
		if(Finish_Mark==1)
		{ 
			break;
		}
		if(pos>=size)
		{
			OutBytesRead=BUF_LEN;
			Finish_Mark=1;
		}
	} 
	if( Update_Success==0)
	{
		if(EndUpdateHMI()!=1) //发送更新标志并检测回应
		{	
			Update_Success=5;
		}
	}
	 
	f_close(file); 
	
	
	delay_ms(10);
	if(Update_Success==0)
	{ 
		printf("%d.bin 升级成功...\r\n",viImageIndex);
	}
	else
	{
		printf("%d.bin 升级失败,失败代码%d...\r\n",viImageIndex,Update_Success);
	}
	return  (Update_Success==0);
}

uint8_t Check_HMI_Image_Index()
{
	
	ReadImgIdx();
	qsize  size = 0;
	uint16_t Wait_Loop=0;
	while(1)
	{
		
		size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //从缓冲区中获取一条指令        
		if(size>0&&cmd_buffer[1]==0x05)           
		{
			 P_IMG_DATA Images_Data= (P_IMG_DATA)cmd_buffer;	
			MAX_INDEX=Images_Data->Img_Count;
			for(int index=0;index<MAX_INDEX;index++)
			{ 
				hmi_Image_Index_Data[index].Img_Idx=(Images_Data->HMI_IMG_IDX[index]).Img_Idx;
				hmi_Image_Index_Data[index].Img_Start_Addr=(Images_Data->HMI_IMG_IDX[index]).Img_Start_Addr;
				hmi_Image_Index_Data[index].Img_X_Len=(Images_Data->HMI_IMG_IDX[index]).Img_X_Len;
				hmi_Image_Index_Data[index].Img_Y_Len=(Images_Data->HMI_IMG_IDX[index]).Img_Y_Len;
				hmi_Image_Index_Data[index].Img_Size_1=(Images_Data->HMI_IMG_IDX[index]).Img_Size_1;
				hmi_Image_Index_Data[index].Img_Size_2=(Images_Data->HMI_IMG_IDX[index]).Img_Size_2;
				hmi_Image_Index_Data[index].Img_Size_3=(Images_Data->HMI_IMG_IDX[index]).Img_Size_3;
			}
			
			LoadImageIndex=1;
			break;
		}
		else
		{
			if(Wait_Loop++>=5000)
				return 0;
			else
				delay_ms(1);
		}
	}
}
uint8_t UpdateHMI(void)
{
//	if(0==0) 
//	{
//		if( 
//			
//			CheckHMIUpdateFiles(IMAGE_INDEX_GRAIDX)!=1
//			&&CheckHMIUpdateFiles(SYSTEM_CONFIG_SI)!=1
//			&&CheckHMIUpdateFiles(SYSTEM_RESOURCES_SBIN)!=1
//		) //检查更新文件是否完整
//		{	
//			return 0;
//		}
//		else
//		{
//			Update_HMI_Status=30;
//		}
//	}
	uint8_t __StopUpdateFlag=1;
//	if(StartDownLoad()!=1)
//	{
//		__StopUpdateFlag=0;
//	}
	
	 if(20==20) 
	{
		
//		SetScreen(12);
//		SetProgressValue(12,4,0); 
//		delay_ms(1*1000);
		if(CheckHMIUpdateFiles(IMAGE_INDEX_GRAIDX)==1)
		{
				
			printf("检测到 图片索引文件...\r\n");
			printf("开始升级 图片索引文件...\r\n");
			__StopUpdateFlag=1;
				if(__StopUpdateFlag && UpdateHMIByFileIndex(SYSTEM_CONFIG_SI)!=1)//下载系统配置文件
					__StopUpdateFlag=0;
				
				Update_HMI_Status=40;
				
			printf("图片索引文件升级完成...\r\n");
		}
		
//		SetScreen(12);
//		SetProgressValue(12,4,100); 
//		delay_ms(1*1000);
	}
	//系统配置更新
	if(30==30) 
	{
		if(CheckHMIUpdateFiles(SYSTEM_CONFIG_SI)==1)
		{
				
			printf("检测到 系统配置文件...\r\n");
			printf("开始升级 系统配置文件...\r\n");
//			SetScreen(12);
//			SetProgressValue(12,4,0); 
//			delay_ms(1*1000);
			__StopUpdateFlag=1;
			if(__StopUpdateFlag && UpdateHMIByFileIndex(SYSTEM_CONFIG_SI)!=1)//下载系统配置文件
			{
				__StopUpdateFlag=0; 
				Update_HMI_Status=40;
				
				printf("图片索引文件 升级完成...\r\n");
			}
			
//			SetScreen(12);
//			SetProgressValue(12,4,100); 
//			delay_ms(1*1000);
				
		}
	}
		//更新系统资源
	if(40==40) 
	{
		if(CheckHMIUpdateFiles(SYSTEM_RESOURCES_SBIN)==1)
		{
			
			printf("检测到 系统资源文件...\r\n");
			printf("开始升级 系统资源文件...\r\n");
//			SetScreen(12);
//			SetProgressValue(12,4,0); 
//			delay_ms(1*1000);
			
			__StopUpdateFlag=1;
			if(__StopUpdateFlag && UpdateHMIByFileIndex(SYSTEM_RESOURCES_SBIN)!=1)//下载系统资源文件
				__StopUpdateFlag=0;
			
				Update_HMI_Status=50;
				printf("图片索引文件 系统资源文件...\r\n");
			
			
//			SetScreen(12);
//			SetProgressValue(12,4,100); 
//			delay_ms(1*1000);
		}
	}
	
	if(50==50)
	{
		if(Check_HMI_Image_Index()==0)
			return 0;
		
		for(uint16_t idx=0; idx<MAX_INDEX;idx++)
		{ 
			
//			SetScreen(12);
//			SetProgressValue(12,4,0); 
//			delay_ms(1*1000);
			
			UpdateHMI_Image(idx);
			
			
//			SetScreen(12);
//			SetProgressValue(12,4,100); 
//			delay_ms(1*1000);
		}
	}
}




u8 HUSB_BootLoader(void)
{
	
}
u8 USH_User_App(void)
{ 
	Detect_USB=1;
	
	printf("检测到U盘...\r\n");
	u8 res=0;
	uint8_t precent;
	uint16_t bytesRead = BUF_LEN, bytesToRead; 
	//FIL file;
	uint32_t pos = 0;
	qsize  size = 0;
	FRESULT MT_RT;
	


	
	
	
	MT_RT=f_mount(fs[2],"2:",1); 	//挂载U盘  
	precent=0;
	SetScreen(HMI_UPGRAD_INDEX); 
	SetProgressValue(HMI_UPGRAD_INDEX,HMI_PROGRAS_CONTROL_INDEX,precent);
	while(answer == 0)
	{
		size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //从缓冲区中获取一条指令        
		if(size>0)//接收到指令
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
		}		
		if(HCD_IsDeviceConnected(&USB_OTG_Core) == 0)
		{
			jump_flag = 1;
			return res;
		}
	}
		
	if(f_open(file,"2:ISADIIMCU.bin",FA_READ)==FR_OK )
	{ 
 
		printf("检测到\"ISADIIMCU.bin\"文件，开始升级MCU...\r\n");
		if(answer == 1)
		{
			while(bytesRead == 2048)
			{
				bytesToRead = sizeof(update_buf);
				f_read(file, update_buf, bytesToRead, (void *)&bytesRead);
				iap_write_appbin(FLASH_APP1_ADDR + pos, update_buf, bytesRead);
				pos += bytesRead;
				precent = pos * 100 / file->fsize;
				SetProgressValue(HMI_UPGRAD_INDEX,HMI_PROGRAS_CONTROL_INDEX,precent%100);
				
				printf("MCU升级共%ldkb,已执行%dKB,已完成%4.2f%%...\r\n", file->fsize/1024,pos/1024,(pos * 100.0f / file->fsize));
				delay_ms(20);
			}
			f_close(file);
			SetProgressValue(HMI_UPGRAD_INDEX,HMI_PROGRAS_CONTROL_INDEX,precent);
			
			delay_ms(2*1000);
		}
	}
	

	if(answer==1)
	{ 
		
		SetScreen(HMI_MAIN_INDEX);
		if(UpdateHMI()==1)
		{ 
			BELL_Work(1);
			delay_ms(500);
			BELL_Work(0);
		}
		 
		
	} 
	
//	
//	uint8_t Cycle_unit=0;
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
		;
	
	SetScreen(0);
//	{	 
//		if(Cycle_unit<=5)
//		{ 
//			Cycle_unit++; 
//			delay_ms(1000);
//		}
//		else
//			break;
//	}
	jump_flag = 1;
} 


//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks=
{
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_MSC_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};
/////////////////////////////////////////////////////////////////////////////////
//以下为各回调函数实现.

//USB HOST 初始化 
void USBH_USR_Init(void)
{
//	printf("USB OTG HS MSC Host\r\n");
//	printf("> USB Host library started.\r\n");
//	printf("  USB Host Library v2.1.0\r\n\r\n");
	
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{
//	LED1=1;
//	printf("检测到USB设备插入!\r\n");

}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{
//	LED1=0;
//	printf("USB设备拔出!\r\n");
}  
//复位从机
void USBH_USR_ResetDevice(void)
{
//	printf("复位设备...\r\n");
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
//		printf("高速(HS)USB设备!\r\n");
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
//		printf("全速(FS)USB设备!\r\n"); 
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
//		printf("低速(LS)USB设备!\r\n");  
	}
	else
	{
//		printf("设备错误!\r\n");  
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
//	USBH_DevDesc_TypeDef *hs;
//	hs=DeviceDesc;   
//	printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
//	printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
//	printf("从机地址分配成功!\r\n");   
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
//		printf("可移动存储器设备!\r\n"); 
	}else if((*id).bInterfaceClass==0x03)
	{
//		printf("HID 设备!\r\n"); 
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
//	printf("Product: %s\r\n",(char *)ProductString);  
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{ 
	printf("设备枚举完成!\r\n\r\n");    
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{
//	printf("无法识别的USB设备!\r\n\r\n");    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
//	printf("跳过用户确认步骤!\r\n");
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
//	printf("端口电流过大!!!\r\n");
} 

extern u8 USH_User_App(void);		//用户测试主程序
//USB HOST MSC类用户应用程序
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://初始化文件系统 

				AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//执行USB OTG 测试主程序
				res=USH_User_App(); //用户主程序
     		res=0;
			if(res)AppState=USH_USR_FS_INIT;
      		break;
    	default:break;
  	} 
	return res;
}
//用户要求重新初始化设备
void USBH_USR_DeInit(void)
{
  	AppState=USH_USR_FS_INIT;
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{

}
////////////////////////////////////////////////////////////////////////////////////////
//用户定义函数,实现fatfs diskio的接口函数 
extern USBH_HOST              USB_Host;

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//返回U盘状态
}

//读U盘
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

//写U盘
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}


/**
* @brief  Explore_Disk 
*         Displays disk content
* @param  path: pointer to root path
* @retval None
*/
u8 Explore_Disk (char* path , uint8_t recu_level)
{

  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];
	int line = 60;
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    while(HCD_IsDeviceConnected(&USB_OTG_Core)) 
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;
      strcpy(tmp, fn); 
      
      if(recu_level == 1)
      {
      }
      else if(recu_level == 2)
      {
      }
      if((fno.fattrib & AM_MASK) == AM_DIR)
      {
      }
      else
      {
      }

      if(((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1))
      {
        Explore_Disk(fn, 2);
      }
			line +=20;
    }
  }
  return res;
}

void File_Read(char* path)
{
//	u8 i = 0;
//	u8 view = 0;
//	int x = 30,y = 300;
	char ReadTextBuff[128]="\0";
	FIL file;
  uint16_t bytesRead, bytesToRead;

	if(f_open(&file,path,FA_READ) == FR_OK)
	{ 
			/* Write buffer to file */
		bytesToRead = sizeof(ReadTextBuff);
		f_read(&file, ReadTextBuff, bytesToRead, (void *)&bytesRead);
	}
//	for(i = 0; i < 64; i++)
//	{
//		view = ReadTextBuff[i];
//		if(ReadTextBuff[i] == 0x0d)
//		{
//			view = ' ';
//			LCD_ShowChar(x,y,view,16,0);
//			i++;
//			y += 20;
//			x = 30;
//			continue;
//		}
//		LCD_ShowChar(x,y,view,16,0);
//		x += 8;
//	}
	
	/*close file and filesystem*/
	f_close(&file);
}
