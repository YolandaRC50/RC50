/*******************************************************************************
** 文件名: 		IO_type.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM
** 作者: 		Adam
** 功能:		硬件接口定义
** 相关文件:		无
** 修改日志：	2018-03-12   创建文档
*******************************************************************************/
#ifndef __IO_TYPE_H
#define __IO_TYPE_H

/* 类型定义 ------------------------------------------------------------------*/
#include "stm32f4xx.h"

typedef struct 
{
	GPIO_TypeDef* port;
	uint16_t pin;
}IOPort_TypeDef;//接口参数

#define PORT_OUT_SET(xport)      GPIO_SetBits(xport.port, xport.pin)
#define PORT_OUT_RESET(xport)    GPIO_ResetBits(xport.port, xport.pin)
#define PORT_IN_READ(xport)     GPIO_ReadInputDataBit(xport.port, xport.pin)

#endif


