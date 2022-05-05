/*******************************************************************************
** 文件名:      timer.c
** 版本：       1.0
** 工作环境:    RealView MDK-ARM
** 作者:        Adam
** 功能:        时间相关程序
** 相关文件:    无
** 修改日志：   2017-12-19   创建文档
*******************************************************************************/
/* 包含的头文件---------------------------------------------------------------*/
#include "sys_core.h"
#include "sys_wdg.h"
/* 变量定义 ------------------------------------------------------------------*/
volatile __int64 run_time_ms=0;/*设备运行时间（ms）*/
volatile __int64 time_ms=0;/*世界时间（ms）*/


#if RTOS_ENABLE //使用操作系统

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数

extern void xPortSysTickHandler(void);
extern void LED_shine_handle(void);
/*******************************************************************************
- * @函数名称	SysTick_Handler
- * @函数说明   SysTick中断发生的中断处理函数
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
//void SysTick_Handler(void)
//{
//    run_time_ms++;
//    time_ms++;
//		LED_shine_handle();
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
//    {
//        xPortSysTickHandler();
//    }
//}
void SysTick_Handler(void)
{	
    run_time_ms++;
    time_ms++;
//	  LED_shine_handle();
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
///*******************************************************************************
//- * @函数名称	delay_init
//- * @函数说明   延时函数初始化
//- * @输入参数   无
//- * @输出参数   无
//- * @返回参数   无
//*******************************************************************************/
//void delay_init(void)
//{
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置中断优先级分组为组4
//	fac_us=SystemCoreClock/1000000;                 //
//	fac_ms=1000/configTICK_RATE_HZ;                 //代表OS可以延时的最少单位	   
//	SysTick_Config(SystemCoreClock / 1000);
//}
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;							//每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;		//根据configTICK_RATE_HZ设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
}		
///*******************************************************************************
//- * @函数名称	delay_us
//- * @函数说明   延时n微妙
//- * @输入参数   nus:要延时的微妙数
//- * @输出参数   无
//- * @返回参数   无
//*******************************************************************************/
//void delay_us(u32 nus)
//{
//	u32 ticks;
//	u32 told,tnow,tcnt=0;
//	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
//	ticks=nus*fac_us; 						//需要的节拍数 
//	told=SysTick->VAL;        				//刚进入时的计数器值
//	while(1)
//	{
//		tnow=SysTick->VAL;	
//		if(tnow!=told)
//		{
//			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
//			else tcnt+=reload-tnow+told;
//			told=tnow;
//			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
//		}
//	}
//}
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
} 
///*******************************************************************************
//- * @函数名称	delay_ms
//- * @函数说明   延时n毫秒
//- * @输入参数   nus:要延时的毫秒数
//- * @输出参数   无
//- * @返回参数   无
//*******************************************************************************/
//void delay_ms(u32 nms)
//{
//	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
//	{
//		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
//		{
//   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
//		}
//		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
//	}
//	delay_us((u32)(nms*1000));				//普通方式延时
//}
void delay_ms(u32 nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));				//普通方式延时
}
#else //不使用操作系统
#include "stm32f10x.h"
volatile u32 timing_delay=0;/*延时变量*/
/*******************************************************************************
- * @函数名称	delay_init
- * @函数说明   延时函数初始化
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void delay_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置中断优先级分组为组4
    SysTick_Config(SystemCoreClock / 1000);
}

/*******************************************************************************
- * @函数名称	SysTick_Handler
- * @函数说明   SysTick中断发生的中断处理函数
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void SysTick_Handler(void)
{
    run_time_ms++;
    time_ms++;
	if(timing_delay)
		timing_delay--;
}

/*******************************************************************************
  * @函数名称	delay_ms
  * @函数说明   开关接口初始化
  * @输入参数   nms：延时时间，单位：毫秒
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void delay_ms(u32 nms)
{
	timing_delay = nms;
	while(timing_delay != 0)
        dwg_feed();
}
#endif

/************************************文件结束*********************************/
