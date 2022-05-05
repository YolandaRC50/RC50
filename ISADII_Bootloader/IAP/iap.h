#ifndef __IAP_H__
#define __IAP_H__  

#include "stm32f4xx.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

void MSR_MSP(u32 addr);	//设置堆栈地址 
extern uint8_t jump_flag;
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
#define FLASH_APP1_ADDR		0x08040000  	//第一个应用程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为Bootloader使用(共64KB)	   
void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
#endif







































