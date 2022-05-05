 
#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "cmd_queue.h"
#include "sys_timer.h"

uint8_t cmd_Updata_Mark=0x00;
uint8_t HMI_MAIN_INDEX=0;
uint8_t HMI_UPGRAD_INDEX=1;
uint8_t HMI_PROGRAS_CONTROL_INDEX=4;
uint8_t HMI_ENTRY_BUTTON_INDEX=1;
uint8_t HMI_CANCEL_BUTTON_INDEX=2;

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void  _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
//	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
//	USART3->DR = (u8) ch;      
//	return ch;
}
#endif

void HMI_USART_Init()
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(HMI_RCC_AHB1Periph_GPIOx,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(HMI_RCC_APB2Periph_USARTx,ENABLE);//使能USART2时钟

 	USART_DeInit(HMI_USARTx);  //复位串口2
	
	GPIO_PinAFConfig(HMI_GPIOx,HMI_GPIO_PinSource_TX,HMI_GPIO_AF_USARTx); //GPIOD6复用为USART2
	GPIO_PinAFConfig(HMI_GPIOx,HMI_GPIO_PinSource_RX,HMI_GPIO_AF_USARTx); //GPIOD5复用为USART2	
	
  GPIO_InitStructure.GPIO_Pin = HMI_GPIO_Pin_TX | HMI_GPIO_Pin_RX; //GPIOD6GPIOD5初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(HMI_GPIOx,&GPIO_InitStructure); //初始化GPIOD6，GPIOD5
	
	USART_InitStructure.USART_BaudRate = HMI_BOUND;//波特率 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(HMI_USARTx, &USART_InitStructure); //初始化串口2
 
	USART_Cmd(HMI_USARTx, ENABLE);               //使能串口 
	
  USART_ITConfig(HMI_USARTx, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = HMI_USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}

void HMI_USARTx_IRQHandler(void)
{
	if (USART_GetFlagStatus(HMI_USARTx, USART_FLAG_PE) != RESET)  
	{  
		USART_ClearFlag(HMI_USARTx, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(HMI_USARTx, USART_FLAG_ORE) != RESET)  
	{  
		USART_ClearFlag(HMI_USARTx, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(HMI_USARTx, USART_FLAG_FE) != RESET)  
	{   
		USART_ClearFlag(HMI_USARTx, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(HMI_USARTx, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		uint8_t data = USART_ReceiveData(HMI_USARTx);
		queue_push(data);
		USART_ClearFlag(HMI_USARTx, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(HMI_USARTx, USART_IT_RXNE);		
	}									 
}
void HMI_SendSingleChar(uint8_t dat)
{ 
	   USART_SendData(HMI_USARTx,dat);
	    while(USART_GetFlagStatus(HMI_USARTx, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(HMI_USARTx, USART_FLAG_TC) == RESET));//等待串口发送完毕
}
