#include "file_operate.h"

//char string[1024];
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
FILINFO fileinfo;								//文件信息
char ReadBuffer[1024*1]="";        /* 读缓冲区 */
char Log_Path_List[4][20]={"MACHINE_INFO_LOG","RUN_STOP_LOG","ERROR_MESSAGE_LOG","PARAMETER_LOG"};
 
FRESULT FR_Result=FR_INT_ERR;
int Sd_f_mount(void)
{
		res_sd = f_mount(&fs,"0:",1);
		/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		//printf("-->SD dont have file system ，will format to file system...\r\n");
    /* 格式化 */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			//printf("-->SD format to file system success\r\n");
      /* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_sd = f_mount(&fs,"0:",1);
			FR_Result=FR_OK;
			return 0;
		}
		else
		{
			//printf("-->SD format to file system fail\r\n");
			return -1;
		}
	}
  else if(res_sd!=FR_OK)
  {
//    printf("!!SD mount file system fail(%d)\r\n",res_sd);
//    printf("！！may due to ：SD initialzation fail。\r\n");
		return  -1;
  }
  else
  {
    //printf("SD mount file system success,can read and write\r\n");
		return 0;
  }
}

int Sd_f_unmount(void)
{
	FRESULT res=f_mount(NULL,"0:",1);
	switch(res)
	{
		case 0:
			return  0;
			break;
		default  :
			return  1;
			break;
	}
}


void Sd_f_dir(s8 *path)
{
//		memset(string,0, 1024);
		static FILINFO FilInfo; //定义静态文件信息结构对象
	
	DIR DirInfo; //定义目录对象
// FRESULT res; /* FatFs 函数通用结果代码 */
#if _USE_LFN//如果使能支持长文件名 先对下面两项初始化
	  uint8_t P_Data[_MAX_LFN * 2 + 1];
    FilInfo.lfsize=_MAX_LFN * 2 + 1;//
    FilInfo.lfname=(TCHAR*)P_Data;//
#endif
    if(f_opendir(&DirInfo,(const TCHAR*)path) == FR_OK)/* 打开文件夹目录成功，目录信息已经在dir结构体中保存 */
    {
        while(f_readdir(&DirInfo, &FilInfo) == FR_OK)  /* 读文件信息到文件状态结构体中 */
        {
            if(!FilInfo.fname[0]) break; /* 如果文件名为‘\0'，说明读取完成结束 */
            //printf("%s/",path);//打印路径
#if _USE_LFN
            //printf(" %s\r\n", FilInfo.fname);//

#endif
        }
    }
}
int Sd_f_creat_folder(char *path)
{
	FRESULT res= f_mkdir(path);
	switch(res)
	{
		case 	FR_OK: 
			puts( "success\r\n");
			return 0;
			break;
		default:
//			printf("ERR code: %d", res);
			return -1;
			break;
	}
}
int Sd_f_delete_folder( char *path)
{
	FRESULT res= f_unlink(path);
	switch(res)
	{
		case 	FR_OK: 
			puts( "success\r\n");
			return 0;
			break;
		default:
			printf("ERR code: %d", res);
			return -1;
			break;
	}
}
		
void Sd_f_write(char *path, char *p_data)
{
	int num;
	res_sd = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE );
	
	if ( res_sd == FR_OK )
	{
		f_lseek(&fnew,f_size(&fnew));
	
    /* 将指定存储区内容写入到文件内 */
//		res_sd=f_write(&fnew,p_data,sizeof(p_data),&fnum);
		
				char buffer[256] = {0};
			sprintf((char *)buffer
				,"\r\n%02d:%02d:%02d  %s" 
			,Sys_RT_Status.System_RTC.hour
			,Sys_RT_Status.System_RTC.minute
			,Sys_RT_Status.System_RTC.second
			,p_data
			);		
			 
		num =f_puts (buffer, &fnew);
    if(num!=-1)
    {
      printf("Write Number: %d\n",num);
      printf("Write Data：\r\n%s\r\n",p_data);
    }
    else
    {
      printf("Write fail\r\n");
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		printf("Creat file fail。\r\n");
	}
}

void Sd_f_read(char *path)
{
	

	res_sd = f_open(&fnew, path, FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{		
		printf("file open success\r\n");
		memset(ReadBuffer, 0, sizeof(ReadBuffer) );
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_sd==FR_OK)
    {
      printf("read data number: %d\r\n",fnum);
      printf("read data is :\r\n");
			for( uint16_t i=0; i<fnum; i++ )
				putchar(ReadBuffer[i]);
			puts("-->\r\n");
    }
    else
    {
      printf("read file fail: (%d)\n",res_sd);
    }		
	}
	else
	{
		printf("!! file open fail\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
}

//
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_mount, Sd_f_mount,  Sd_f_mount);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_unmount, Sd_f_unmount,  Sd_f_unmount);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_dir,   Sd_f_dir,    Sd_f_dir);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_write, Sd_f_write,  Sd_f_write);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_read,  Sd_f_read,   Sd_f_read);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_creat_folder,  Sd_f_creat_folder,   Sd_f_creat_folder);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Sd_f_delete_folder, Sd_f_delete_folder,   Sd_f_delete_folder);

char File_Path[100];

//先创建需要的文件夹
int Load_Cread_LogFolder()
{  
	Sd_f_mount();
	
	if(FR_Result!=FR_OK) return 0;
	for(uint16_t i=0; i<4; i++)
		Sd_f_creat_folder(&Log_Path_List[i][0]);
}
//0 正常 -1文件系统挂载失败 -2文件写入失败
int file_write(Log_Type viLogType, char *p_data )
{
	if(FR_Result!=FR_OK) return 0;
		memset(File_Path,0,100);
		//char buffer[100] = {0};
		sprintf((char *)File_Path
			,"/%s/20%02d-%02d-%02d.txt"
		,Log_Path_List[viLogType]
		,Sys_RT_Status.System_RTC.year
		,Sys_RT_Status.System_RTC.month
		,Sys_RT_Status.System_RTC.day);		
		 
		Sd_f_mount();
		//Log(path);
		
		Sd_f_write(File_Path,p_data);
}
//0 正常 -1文件系统挂载失败 -2文件写入失败
int file_read(Log_Type viLogType,  char *p_dataFileName)
{
	if(FR_Result!=FR_OK) return 0;
	 memset(File_Path,0,100);
		sprintf((char *)File_Path
			,"/%s/%s.txt"
		,Log_Path_List[viLogType]
		,p_dataFileName);
		
	Sd_f_mount(); 
	Sd_f_read(File_Path);
}
 
//
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), file_write, file_write,  file_write);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), Load_Cread_LogFolder, Load_Cread_LogFolder,  Load_Cread_LogFolder);

