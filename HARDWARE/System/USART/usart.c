#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "cmd_queue.h"
#include "sys_timer.h"
#include "bell.h"  

uint8_t moedbus_busy = 0;
//串口发送缓存区 	
__align(8) uint8_t USART3_TX_BUF[USART_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
__align(8) uint8_t USART6_TX_BUF[USART_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节  
//串口接收缓存区 	
uint8_t USART3_RX_BUF[USART_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
uint8_t USART6_RX_BUF[USART_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.

uint16_t USART3_RX_STA=0;   	 
//void USART3_IRQHandler(void)
//{	   
//	uint8_t res = 0;
//	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
//	{  
//		USART_ClearFlag(USART3, USART_FLAG_PE);  
//	}  

//	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
//	{   
//		USART_ClearFlag(USART3, USART_FLAG_ORE);  
//	}  

//	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
//	{  
//		USART_ClearFlag(USART3, USART_FLAG_FE);  
//	}

//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
//	{
//		res =USART_ReceiveData(USART3);//;读取接收到的数据USART2->DR
//		if(USART3_RX_STA< USART_MAX_RECV_LEN)
//		{
//			USART3_RX_BUF[USART3_RX_STA]=res;		//记录接收到的值
//			USART3_RX_STA++;						//接收数据增加1 
//		}
//		USART_ClearFlag(USART3, USART_FLAG_RXNE);  
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		
//	}										 
//}  
	 
void USART3_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
	{  
		USART_ClearFlag(USART3, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
	{  
		USART_ClearFlag(USART3, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
	{   
		USART_ClearFlag(USART3, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		uint8_t data = USART_ReceiveData(USART3);
		queue_push(data);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		
	}										 
}

uint16_t USART6_RX_STA=0;   	 
void USART6_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET)  
	{  
	USART_ClearFlag(USART6, USART_FLAG_PE);  
	}  

	if(USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)  
	{  
	USART_ClearFlag(USART6, USART_FLAG_ORE);  
	}  

	if (USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET)  
	{   
	USART_ClearFlag(USART6, USART_FLAG_FE);  
	}

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		
		USART_ClearFlag(USART6, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);		
		
		uint8_t data = USART_ReceiveData(USART6); 
		/*Start*/ 
		/*End*/
		 
	}										 
} 

//初始化IO 串口3
//bound:波特率	  
void USART485_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

 	USART_DeInit(USART3);  //复位串口2
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB10复用为USART3	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10和GPIOB11初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	
	USART_InitStructure.USART_BaudRate = bound;//波特率 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//2个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口2
 
	USART_Cmd(USART3, ENABLE);               //使能串口 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOB10和GPIOB11初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);//接收数据模式
	USART3_RX_STA=0;		//清零
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]); 	 //发送数据到串口3 
	}
}

void RS485_Send_Data(uint8_t *buf,uint8_t len)
{
	
	uint8_t t;

	GPIO_SetBits(GPIOA, GPIO_Pin_3);//发送数据模式
	for(t=0;t<len;t++)		//循环发送数据
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //等待发送结束	
			USART_SendData(USART3,buf[t]); //发送数据
	}	 
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //等待发送结束		
		USART3_RX_STA=0;
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);//接收数据模式
	moedbus_busy=0;
}
 
void USART232_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART3时钟

 	USART_DeInit(USART6);  //复位串口5
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOG9
	
	
	USART_InitStructure.USART_BaudRate = bound;//波特率 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART6, &USART_InitStructure); //初始化串口2
 
	USART_Cmd(USART6, ENABLE);               //使能串口 
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	USART6_RX_STA=0;		//清零
} 


//串口6,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u6_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART6_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART6_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USART6,(uint8_t)USART6_TX_BUF[j]); 	 //发送数据到串口3 
	}
}



void USARTDIS_Init(uint32_t bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟

 	USART_DeInit(USART3);  //复位串口2
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD6复用为USART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD5复用为USART2	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOD6GPIOD5初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOD6，GPIOD5
	
	USART_InitStructure.USART_BaudRate = bound;//波特率 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口2
 
	USART_Cmd(USART3, ENABLE);               //使能串口 
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//抢占优先级8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}


void ui_sendsinglechar(uint8_t dat)
{
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);  //等待上次传输完成 
//	USART_SendData(USART2,dat); 	 //发送数据到串口1
	
	   USART_SendData(USART3,dat);
	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET));//等待串口发送完毕
}

//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
//uint8_t modbus_t = 0;
void RS485_Receive_Data(uint8_t *buf,uint16_t viTagLen,uint16_t *len)
{
	
	moedbus_busy = 1;
	uint16_t rxlen=0;
	uint8_t i=0;
	while(viTagLen!=USART3_RX_STA && i++<50)
	{
		delay_ms(1);	
	}
	rxlen=USART3_RX_STA;
	*len=0;				  //默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	
	if(rxlen==USART3_RX_STA && rxlen==viTagLen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];	
		}		
		*len=USART3_RX_STA;	//记录本次数据长度
	}
		USART3_RX_STA=0;		//清零
//	modbus_t++;
//	
//	if(modbus_t > 10)
//	{
//		modbus_t = 0;
	moedbus_busy = 0;
//	}
}

/*!
*   \brief  发送1个字节
*   \param  t 发送的字节
*/
void  Custom_RS232_SendChar(unsigned char  t)
{
	    USART_SendData(USART6,t);
	    while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
	    while((USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET));//等待串口发送完毕
	    //DelayMS(1);
}