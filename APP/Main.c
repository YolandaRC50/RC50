#include "Includes.h"
#include "shell.h" 
 
#define DEBUG   0

Shell shell;
	#if DEBUG ==1
char shellBuffer[512];
	#endif	

void userShellWrite(char data)
{ 
	#if DEBUG ==1
		HMI_SendSingleChar(data); 
	#endif	
}

signed char userShellRead(char *data)
{
	return 0;
}
void User_Shell_Init(void)
{
	#if DEBUG ==1
	shell.write = userShellWrite;
	shellInit(&shell, shellBuffer, 512);
	#endif	
}

int fputc(int ch, FILE *f)
{ 	
	#if DEBUG ==1
		HMI_SendSingleChar(ch); 
	#endif	
	return ch;
}
int main(void)
{ 
	
	  __enable_irq();
		NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x8040000);
//	  __enable_irq();
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x8040000);
	
	
	BSP_Init(); 		
	/* 创建任务 */
	AppTaskCreate();
	User_Shell_Init();
	/* 创建任务通信机制 */
	AppObjCreate();	
  /* 启动调度，开始执行任务 */
	vTaskStartScheduler();	
	while(1)
		{	
			uint32_t i=0x1fff000;
//			printf("123\r\n");
				while(i--);
		
		};     //开启任务调度
}

#if DEBUG ==1
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
	  shellHandler(&shell, Res);
	} 
}
#endif
