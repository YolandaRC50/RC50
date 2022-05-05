/*******************************************************************************
** 文件名: 		sys_debug.h
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM
** 作者: 		Adam
** 功能:		debug相关程序
** 相关文件:		无
** 修改日志：	2017-02-12   创建文档
*******************************************************************************/
/* 防止重定义-----------------------------------------------------------------*/
#ifndef _CORE_DEBUG_H
#define _CORE_DEBUG_H
/* 包含的头文件---------------------------------------------------------------*/
#include "sys_config.h"

#if DEBUG_ENABLE
    void dbg_print (const char *format, ...);
//    #define DEBUG(format, ...) Debug_Printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)
    //警告是因为 __FILE__ 中有汉字
//    #define DEBUG(format, ...) debug_printf("FILE: %s, LINE: %d: "format"\n", __FILE__, __LINE__, ##__VA_ARGS__)
//    #define DEBUG(format, ...) dbg_print(""format"\n", ##__VA_ARGS__)
#else
//    #define debug_printf(format, ...);
    #define dbg_print(format, ...);
#endif

#endif

/************************************文件结束**********************************/
