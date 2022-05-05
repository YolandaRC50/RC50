#ifndef __FILE_OPERATE_H__
#define __FILE_OPERATE_H__
#include "includes.h" 
#include "shell.h" 

//#define LOG "Log"

#define Log(str)  memset(File_Path,0,100);\
									strcat(File_Path,"/Log/" );\
									strcat(File_Path,str );   
extern char ReadBuffer[];        /* ¶Á»º³åÇø */
int Load_Cread_LogFolder();
int file_write(Log_Type viLogType, char *p_data );
int file_read(Log_Type viLogType,  char *p_dataFileName);
#endif