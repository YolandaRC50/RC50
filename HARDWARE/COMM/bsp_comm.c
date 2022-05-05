
#if(1==1) 
#include "bsp_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
//#include "ethernetif.h" 
#include "netif/ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "malloc.h"
#include "delay.h"
#include "usart.h"  
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include <stdio.h>
#include "bsp_ethernet.h"
#endif
//#include "lcd.h" 						  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
//__lwip_dev lwipdev;						//lwip控制结构体 
struct netif lwip_netif;				//定义一个全局的网络接口
 
extern u32 memp_get_memorysize(void);	//在memp.c里面定义
extern u8_t *memp_memory;				//在memp.c里面定义.
extern u8_t *ram_heap;					//在mem.c里面定义.
TaskHandle_t LWIP_DHCP_TaskHandler;
/////////////////////////////////////////////////////////////////////////////////


#if(1==1)  //WIFI_MODE//BLUETOOTH_MODE

WIFI_BLUETOOTH_DMA_UART Wifi_Bluetooth_DMA_Uart; 
 
	
uint8_t Wifi_Bluetooth_DMA_Buffer_RX[IOT_SENDANDRECEIVE_BUFFER_LEN];
uint8_t Wifi_Bluetooth_DMA_Buffer_TX[IOT_SENDANDRECEIVE_BUFFER_LEN]; 

#if(1==1) //Wfif
/*
1 AT+GMR 　　　　　!!启动查询版本信息
2 AT+CWMODE_DEF=1 　　　!!配置WiFi模组工作模式为单STA模式，并把配置保存在flash
3 AT+CWJAP_DEF=“newifi_F8A0”,“anxinke123” !!连接网络
4 AT+CWAUTOCONN=1 　　　!!使能上电自动连接AP
3,AT+CIPSTART=“TCP”,“192.168.99.217”,6001 　　!!连接TCP服务器,本实验用网络调试助手进行测试

5,AT+CIPMODE=1 　　　　　　　　　　　　　　　　　!!方式二：使用透传模式发送数据
6,AT+CIPSEND 　　　　　　　　　　　　　　　　　　!!发送数据（Data:Test2）,见上图1：
7,发送：+++ 　　　　　　　　　　　　　　　　　　!!退出透传发送三个连续的+++，不要勾选新行（\r\n）,见下图：
6,AT+CIPCLOSE 　
*/


//串口参数配置
void Wifi_Uart_Param(void)
{ 
	WIFI_BLUETOOTH_DMA_UART P_Struct_DMA_Uart;
	
	P_Struct_DMA_Uart.Dma_R_Flag=0;
	P_Struct_DMA_Uart.Dma_T_Flag=0; 
	 
	
	P_Struct_DMA_Uart.Rxd_Fifo =Wifi_Bluetooth_DMA_Buffer_RX;
	P_Struct_DMA_Uart.Txd_Fifo =Wifi_Bluetooth_DMA_Buffer_TX; 
	memcpy(&Wifi_Bluetooth_DMA_Uart,&P_Struct_DMA_Uart, sizeof(P_Struct_DMA_Uart) );
	 
}
//PA2 PA3
void Wifi_Init_Bound(u32 bound)
{
#if(1==0)
	#if 1 
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_GPIOx,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_USARTx,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(WIFI_RCC_AHBxPeriph_DMAx,ENABLE);  //DMA1时钟使能 
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(WIFI_GPIOx,WIFI_GPIO_PinSourceRX,WIFI_GPIO_AF_USARTx); //GPIOA2复用为USART1
	GPIO_PinAFConfig(WIFI_GPIOx,WIFI_GPIO_PinSourceTX,WIFI_GPIO_AF_USARTx); //GPIOA3复用为USART1
 
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = WIFI_GPIO_PINx_RX | WIFI_GPIO_PINx_TX; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(WIFI_GPIOx,&GPIO_InitStructure); //初始化PA2，PA3
 
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(WIFI_USARTx, &USART_InitStructure); //初始化串口1
 
	USART_Cmd(WIFI_USARTx, ENABLE);  //使能串口1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(WIFI_USARTx, USART_IT_IDLE, ENABLE);//开启相关中断
 
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_USARTx_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
 
	USART_DMACmd(WIFI_USARTx,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送   
	USART_DMACmd(WIFI_USARTx,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收
 #endif
//Dma_Tx();
	//****************************配置UART2发送
#if 1  //TX
	{
		DMA_DeInit(WIFI_DMAx_Streamx_TX);
		while (DMA_GetCmdStatus(WIFI_DMAx_Streamx_TX) != DISABLE);//等待DMA可配置 
		/* 配置 DMA Stream */
		DMA_InitStructure.DMA_Channel = WIFI_DMA_Channel_x;  //通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&WIFI_USARTx->DR;//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA 存储器0地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
		DMA_InitStructure.DMA_BufferSize =  IOT_SENDANDRECEIVE_BUFFER_LEN;//;//数据传输量 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
		
		DMA_Init(WIFI_DMAx_Streamx_TX, &DMA_InitStructure);//初始化DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = WIFI_DMAx_Streamx_IRQn_TX;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(WIFI_DMAx_Streamx_TX,DMA_IT_TC,ENABLE);
}
 #endif
	//****************************配置UART2接收
#if 1  //RT
{
	DMA_DeInit(WIFI_DMAx_Streamx_RX);
	while (DMA_GetCmdStatus(WIFI_DMAx_Streamx_RX) != DISABLE);//等待DMA可配置 
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = WIFI_DMA_Channel_x;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&WIFI_USARTx->DR;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(WIFI_DMAx_Streamx_RX, &DMA_InitStructure);//初始化DMA Stream
	
	
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_DMAx_Streamx_IRQn_RX;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);  //开启DMA传输 
	DMA_ITConfig(WIFI_DMAx_Streamx_RX,DMA_IT_TC,ENABLE);
}
	#endif
#endif
#if(1)
#if 1 
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);  //DMA1时钟使能 
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA2复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA3复用为USART3
 
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA2，PA3
 
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
 
	USART_Cmd(USART3, ENABLE);  //使能串口1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启相关中断
 
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
 
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送   
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收
 #endif
	//****************************配置UART3发送
#if 1
	{
		DMA_DeInit(DMA1_Stream3);
		while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);//等待DMA可配置 
		/* 配置 DMA Stream */
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA 存储器0地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
		DMA_InitStructure.DMA_BufferSize =  IOT_SENDANDRECEIVE_BUFFER_LEN;//;//数据传输量 
		
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
		
		DMA_Init(DMA1_Stream3, &DMA_InitStructure);//初始化DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);
}
 #endif
 



	//****************************配置UART3接收
#if 1
{
	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);//等待DMA可配置 
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//数据传输量 
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);//初始化DMA Stream
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(DMA1_Stream1, ENABLE);  //开启DMA传输 
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
}
	#endif
#endif
}
 
#if 2 //Uart3
//发送完成中断
void WIFI_DMAx_Streamx_IRQHandler_TX(void)
{
	//清除标志
	if(DMA_GetFlagStatus(WIFI_DMAx_Streamx_TX,WIFI_DMA_FLAG_TCIFx_TX)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(WIFI_DMAx_Streamx_TX,WIFI_DMA_FLAG_TCIFx_TX);//清除DMA2_Steam7传输完成标志
		USART_ITConfig(WIFI_USARTx, USART_IT_TC, ENABLE);//开启相关中断
	}
	//
}
//接收完成中断
void WIFI_DMAx_Streamx_IRQHandler_RX(void)
{
	//清除标志
	if(DMA_GetFlagStatus(WIFI_DMAx_Streamx_RX,WIFI_DMA_FLAG_TCIFx_RX)!=RESET)//等待DMA1_Steam5传输完成
	{ 
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(WIFI_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(WIFI_DMAx_Streamx_RX, DISABLE); //关闭DMA,防止处理其间有数据
		DMA_ClearFlag(WIFI_DMAx_Streamx_RX,WIFI_DMA_CLEARFLAG_RX);//清除DMA1_Steam5传输完成标志
		DMA_SetCurrDataCounter(WIFI_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);     //打开DMA,
		
    OS_UARTx_THREAD_GET();
//		osSignalSet( UART1_GET, 1 );
	}
}

//空闲中断
void WIFI_USARTx_IRQHandler(void)
{
	u16 data;
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_TC) != RESET)
	{
		USART_ClearFlag(WIFI_USARTx, USART_IT_TC);
		USART_ITConfig(WIFI_USARTx, USART_IT_TC, DISABLE);//关中断
		Wifi_Bluetooth_DMA_Uart.Dma_T_Flag =0;
	}
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_IDLE) != RESET)
	{
		data = WIFI_USARTx->SR;
		data = WIFI_USARTx->DR;
		
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(WIFI_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(WIFI_DMAx_Streamx_RX, DISABLE); //关闭DMA,防止处理其间有数据
		DMA_ClearFlag(WIFI_DMAx_Streamx_RX,WIFI_DMA_CLEARFLAG_RX);//清除DMA2_Steam7传输完成标志
		DMA_SetCurrDataCounter(WIFI_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(WIFI_DMAx_Streamx_RX, ENABLE);     //打开DMA,
		
    OS_UARTx_THREAD_GET();
//		os_Uart2_Thread_Get();
	}
} 
#endif
void Wifi_Init()
{
	
	Wifi_Uart_Param();
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case WIFI_MODE:
		{
			Wifi_Init_Bound(WIFI_MODE_BOUND);
			break;
		}
		case BLUETOOTH_MODE:
		case ETHERNET_MODE:
		{
			Wifi_Init_Bound(BLUETOOTH_MODE_BOUND);
			break;
		} 
	}
}

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void Wifi_DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	 
uint8_t Wifi_Send_Dma( WIFI_BLUETOOTH_DMA_UART *I_Struct_DMA_Uart )
{ 
	WIFI_BLUETOOTH_DMA_UART *P_DMA_Uart=I_Struct_DMA_Uart; 
	Wifi_DmaSendDataProc(WIFI_DMAx_Streamx_TX,P_DMA_Uart->Txd_L); //开始一次DMA传输  
	return 1;
}
int16_t Wifi_Send(char *str)
{ 
		strcpy((char *)Wifi_Bluetooth_DMA_Uart.Txd_Fifo, str);
		Wifi_Bluetooth_DMA_Uart.Txd_L =strlen(str);
    puts(str);
		Wifi_Send_Dma(&Wifi_Bluetooth_DMA_Uart);
 }


#endif
#endif 

#if(1==1) //Bluetooth
 int Bluetooth_Cmd(uint16_t num)
{
  Sys_RT_Status.RT_IOT_Status.Bluetooth_Status.Command=num;
	xEventGroupSetBits(Event_Handle, EVENT_HANDLE_BLUE);
	return 0;
}	
 //串口参数配置
void Bluetooth_Uart_Param(void)
{ 
	WIFI_BLUETOOTH_DMA_UART P_Struct_DMA_Uart;
	
	P_Struct_DMA_Uart.Dma_R_Flag=0;
	P_Struct_DMA_Uart.Dma_T_Flag=0; 
	 
	
	P_Struct_DMA_Uart.Rxd_Fifo =Wifi_Bluetooth_DMA_Buffer_RX;
	P_Struct_DMA_Uart.Txd_Fifo =Wifi_Bluetooth_DMA_Buffer_TX; 
	memcpy(&Wifi_Bluetooth_DMA_Uart,&P_Struct_DMA_Uart, sizeof(P_Struct_DMA_Uart) );
	 
}
//PA2 PA3
void Bluetooth_Init_Bound(u32 bound)
{
	#if(1==0)
	
	#if 1 
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(BLUETOOTH_RCC_AHBxPeriph_GPIOx,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(BLUETOOTH_RCC_AHBxPeriph_USARTx,ENABLE);//使能USART1时钟
	RCC_AHB1PeriphClockCmd(BLUETOOTH_RCC_AHBxPeriph_DMAx,ENABLE);  //DMA1时钟使能 
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(BLUETOOTH_GPIOx,BLUETOOTH_GPIO_PinSourceRX,BLUETOOTH_GPIO_AF_USARTx); //GPIOA2复用为USART1
	GPIO_PinAFConfig(BLUETOOTH_GPIOx,BLUETOOTH_GPIO_PinSourceTX,BLUETOOTH_GPIO_AF_USARTx); //GPIOA3复用为USART1
 
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = BLUETOOTH_GPIO_PINx_RX | BLUETOOTH_GPIO_PINx_TX; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(BLUETOOTH_GPIOx,&GPIO_InitStructure); //初始化PA2，PA3
 
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(BLUETOOTH_USARTx, &USART_InitStructure); //初始化串口1
 
	USART_Cmd(BLUETOOTH_USARTx, ENABLE);  //使能串口1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(BLUETOOTH_USARTx, USART_IT_IDLE, ENABLE);//开启相关中断
 
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_USARTx_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
 
	USART_DMACmd(BLUETOOTH_USARTx,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送   
	USART_DMACmd(BLUETOOTH_USARTx,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收
 #endif
//Dma_Tx();
	//****************************配置UART2发送
#if 1  //TX
	{
		DMA_DeInit(BLUETOOTH_DMAx_Streamx_TX);
		while (DMA_GetCmdStatus(BLUETOOTH_DMAx_Streamx_TX) != DISABLE);//等待DMA可配置 
		/* 配置 DMA Stream */
		DMA_InitStructure.DMA_Channel = BLUETOOTH_DMA_Channel_x;  //通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&BLUETOOTH_USARTx->DR;//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA 存储器0地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
		DMA_InitStructure.DMA_BufferSize =  IOT_SENDANDRECEIVE_BUFFER_LEN;//;//数据传输量 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
		
		DMA_Init(BLUETOOTH_DMAx_Streamx_TX, &DMA_InitStructure);//初始化DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_DMAx_Streamx_IRQn_TX;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(BLUETOOTH_DMAx_Streamx_TX,DMA_IT_TC,ENABLE);
}
 #endif
	//****************************配置UART2接收
#if 1  //RT
{
	DMA_DeInit(BLUETOOTH_DMAx_Streamx_RX);
	while (DMA_GetCmdStatus(BLUETOOTH_DMAx_Streamx_RX) != DISABLE);//等待DMA可配置 
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = BLUETOOTH_DMA_Channel_x;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&BLUETOOTH_USARTx->DR;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(BLUETOOTH_DMAx_Streamx_RX, &DMA_InitStructure);//初始化DMA Stream
	
	
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_DMAx_Streamx_IRQn_RX;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(BLUETOOTH_DMAx_Streamx_RX, ENABLE);  //开启DMA传输 
	DMA_ITConfig(BLUETOOTH_DMAx_Streamx_RX,DMA_IT_TC,ENABLE);
}
#endif
#endif


	#if(1)

	#if 1 
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
 
	RCC_AHB1PeriphClockCmd(BLUETOOTH_RCC_AHBxPeriph_GPIOx,ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);  //DMA1时钟使能 
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC5复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC6复用为USART6
 
	//USART6端口配置
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; //GPIOC6与GPIOC7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC5，PC6
 
	//USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
 
	USART_Cmd(USART6, ENABLE);  //使能串口1 
 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//开启相关中断
 
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
 
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送   
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);  //使能串口1的DMA接收
 #endif
	//****************************配置UART6发送
#if 1
	{
		DMA_DeInit(DMA2_Stream6);
		while (DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);//等待DMA可配置 
		/* 配置 DMA Stream */
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;  //通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART6->DR;//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr =   (u32)Wifi_Bluetooth_DMA_Uart.Txd_Fifo;//DMA 存储器0地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
		DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//;//数据传输量 
		
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
		
		DMA_Init(DMA2_Stream6, &DMA_InitStructure);//初始化DMA Stream
		//DMA NVIC  
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure);  
		DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);
}
 #endif
 



	//****************************配置UART6接收
#if 1
{
	DMA_DeInit(DMA2_Stream2);
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);//等待DMA可配置 
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART6->DR;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Wifi_Bluetooth_DMA_Uart.Rxd_Fifo;//(u32)ReceiveBuff;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = IOT_SENDANDRECEIVE_BUFFER_LEN;//RECEIVE_BUF_SIZE;//数据传输量 
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);//初始化DMA Stream
	//DMA NVIC  
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	DMA_Cmd(DMA2_Stream2, ENABLE);  //开启DMA传输 
	DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
}
	#endif


	#endif
}
 
#if 2 //Uart2
//发送完成中断
void BLUETOOTH_DMAx_Streamx_IRQHandler_TX(void)
{
	//清除标志
	if(DMA_GetFlagStatus(BLUETOOTH_DMAx_Streamx_TX,BLUETOOTH_DMA_FLAG_TCIFx_TX)!=RESET)//等待DMA2_Steam7传输完成
	{ 
		DMA_ClearFlag(BLUETOOTH_DMAx_Streamx_TX,BLUETOOTH_DMA_FLAG_TCIFx_TX);//清除DMA2_Steam7传输完成标志
		USART_ITConfig(BLUETOOTH_USARTx, USART_IT_TC, ENABLE);//开启相关中断
	}
	//
}
//接收完成中断
void BLUETOOTH_DMAx_Streamx_IRQHandler_RX(void)
{
	//清除标志
	if(DMA_GetFlagStatus(BLUETOOTH_DMAx_Streamx_RX,BLUETOOTH_DMA_FLAG_TCIFx_RX)!=RESET)//等待DMA1_Steam5传输完成
	{ 
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(BLUETOOTH_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(BLUETOOTH_DMAx_Streamx_RX, DISABLE); //关闭DMA,防止处理其间有数据
		DMA_ClearFlag(BLUETOOTH_DMAx_Streamx_RX,BLUETOOTH_DMA_CLEARFLAG_RX);//清除DMA1_Steam5传输完成标志
		DMA_SetCurrDataCounter(BLUETOOTH_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(BLUETOOTH_DMAx_Streamx_RX, ENABLE);     //打开DMA,
		
    OS_UARTx_THREAD_GET();
//		osSignalSet( UART1_GET, 1 );
	}
}

//空闲中断
void BLUETOOTH_USARTx_IRQHandler(void)
{
	u16 data;
	if(USART_GetITStatus(BLUETOOTH_USARTx,USART_IT_TC) != RESET)
	{
		USART_ClearFlag(BLUETOOTH_USARTx, USART_IT_TC);
		USART_ITConfig(BLUETOOTH_USARTx, USART_IT_TC, DISABLE);//关中断
		Wifi_Bluetooth_DMA_Uart.Dma_T_Flag =0;
	}
	if(USART_GetITStatus(BLUETOOTH_USARTx,USART_IT_IDLE) != RESET)
	{
		data = BLUETOOTH_USARTx->SR;
		data = BLUETOOTH_USARTx->DR;
		
		Wifi_Bluetooth_DMA_Uart.Rxd_L =IOT_SENDANDRECEIVE_BUFFER_LEN - DMA_GetCurrDataCounter(BLUETOOTH_DMAx_Streamx_RX);
		Wifi_Bluetooth_DMA_Uart.Dma_R_Flag =1;
		
		DMA_Cmd(BLUETOOTH_DMAx_Streamx_RX, DISABLE); //关闭DMA,防止处理其间有数据
		DMA_ClearFlag(BLUETOOTH_DMAx_Streamx_RX,BLUETOOTH_DMA_CLEARFLAG_RX);//清除DMA2_Steam7传输完成标志
		DMA_SetCurrDataCounter(BLUETOOTH_DMAx_Streamx_RX, IOT_SENDANDRECEIVE_BUFFER_LEN);
		DMA_Cmd(BLUETOOTH_DMAx_Streamx_RX, ENABLE);     //打开DMA,
		
    OS_UARTx_THREAD_GET();
//		os_Uart2_Thread_Get();
	}
} 
#endif
void Bluetooth_Init()
{
	
	Bluetooth_Uart_Param();
	switch(Sys_Params.IOT_Params.Commication_Mode)
	{
		case BLUETOOTH_MODE:
		{
			Bluetooth_Init_Bound(BLUETOOTH_MODE_BOUND);
			break;
		}
		case WIFI_MODE:
		case ETHERNET_MODE:
		{
			Wifi_Init_Bound(WIFI_MODE_BOUND);
			break;
		} 
	}
}

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void Bluetooth_DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
}	 
uint8_t Bluetooth_Send_Dma( WIFI_BLUETOOTH_DMA_UART *I_Struct_DMA_Uart )
{ 
	WIFI_BLUETOOTH_DMA_UART *P_DMA_Uart=I_Struct_DMA_Uart; 
	Bluetooth_DmaSendDataProc(BLUETOOTH_DMAx_Streamx_TX,P_DMA_Uart->Txd_L); //开始一次DMA传输  
	return 1;
}
int16_t Bluetooth_Send(char *str)
{ 
		strcpy((char *)Wifi_Bluetooth_DMA_Uart.Txd_Fifo, str);
		Wifi_Bluetooth_DMA_Uart.Txd_L =strlen(str);
    puts(str);
		Bluetooth_Send_Dma(&Wifi_Bluetooth_DMA_Uart);
 }


#endif
#if(1==1)  //ETHERNET_MODE
//lwip两个任务定义(内核任务和DHCP任务)
 
//lwip DHCP任务
//设置任务优先级
#define LWIP_DHCP_TASK_PRIO       		4
//设置任务堆栈大小
#define LWIP_DHCP_STK_SIZE  		   	512
//任务函数
void lwip_dhcp_task(void *pdata); 
 
//用于以太网中断调用
void lwip_pkt_handle(void)
{
	ethernetif_input(&lwip_netif);
}
 
//lwip中mem和memp的内存申请
//返回值:0,成功;
//    其他,失败
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
    INTX_DISABLE();//关中断
	mempsize=memp_get_memorysize();			//得到memp_memory数组大小
	memp_memory=mymalloc(SRAMIN,mempsize);	//为memp_memory申请内存
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//得到ram heap大小
	ram_heap=mymalloc(SRAMIN,ramheapsize);	//为ram_heap申请内存   
	//TCPIP_THREAD_TASK_STK=mymalloc(SRAMIN,TCPIP_THREAD_STACKSIZE*4);//给内核任务申请堆栈,这里FreeRTOS创建任务的时候使用了动态创建，如果是静态创建就需要申请堆栈
	//LWIP_DHCP_TASK_STK=mymalloc(SRAMIN,LWIP_DHCP_STK_SIZE*4);		  //给dhcp任务堆栈申请内存空间
    INTX_ENABLE();//开中断
	if(!memp_memory||!ram_heap)//有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip中mem和memp内存释放
void lwip_comm_mem_free(void)
{ 	
	myfree(SRAMIN,memp_memory);
	myfree(SRAMIN,ram_heap);
}
//lwip 默认IP设置
//lwipx:lwip控制结构体指针
void lwip_comm_default_ip_set()
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字节 
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[1]=0;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[2]=0;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[4]=(sn0>>8)&0XFFF;
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.mac[5]=sn0&0XFF;  
	Sys_RT_Status.RT_IOT_Status.Ethernet_Status.dhcpstatus=0;//没有DHCP	 
} 
 
//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
u8 lwip_comm_init(void)
{
    u8 retry=0;
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功 
 
	if(ETH_Mem_Malloc())return 1;		//内存申请失败
	if(lwip_comm_mem_malloc())return 2;	//内存申请失败
	puts("1");
	  
	if(LAN8720_Init())		        //初始化LAN8720,如果失败的话就重试5次
    {
//			puts("2");
//        retry++;
//        if(retry>5) {retry=0;return 3;} //LAN8720初始化失败
			return 3;
    }
	puts("2");
	tcpip_init(NULL,NULL);				//初始化tcp ip内核,该函数里面会创建tcpip_thread内核任务
	lwip_comm_default_ip_set();	//设置默认IP等信息
 
	taskENTER_CRITICAL();
#if LWIP_DHCP		//使用动态IP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else				//使用静态IP
	Netif_Init_Flag=netif_add(
		&lwip_netif,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Loca_IPAddr,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Net_Mask,
		&Sys_RT_Status.RT_IOT_Status.Comm_Status_Data.Gate_way,
		NULL,
		&ethernetif_init,
		&tcpip_input
		);//向网卡列表中添加一个网口 
#endif
	taskEXIT_CRITICAL();  //退出临界区
	if(Netif_Init_Flag==NULL)return 4;//网卡添加失败 
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default(&lwip_netif); //设置netif为默认网口
		netif_set_up(&lwip_netif);		//打开netif网口
	} 
	return 0;//操作OK.
}   
//如果使能了DHCP 

#endif
