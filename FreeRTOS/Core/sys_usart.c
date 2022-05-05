/*******************************************************************************
** 文件名:      sys_usart.c
** 版本：       1.0
** 工作环境:    RealView MDK-ARM
** 作者:        Adam
** 功能:        串口驱动程序
** 相关文件:    无
** 修改日志：   2017-09-09   创建文档
*******************************************************************************/
/* 包含的头文件 --------------------------------------------------------------*/
#include "sys_usart.h"

/* 接口定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/* 类型定义 ------------------------------------------------------------------*/
/* 变量定义  -----------------------------------------------------------------*/
/* 函数声明  -----------------------------------------------------------------*/

/******************************************************************************/
/*           		             串口配置                                     */
/******************************************************************************/

/******************************************************************************
- * @函数名称   usart_init
- * @函数说明   初始化串口
- * @输入参数   bound:波特率
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void usart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，
//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
 	USART_DeInit(USART2);  //复位串口1
	 //USART2_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PB6
   
    //USART2_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PB7


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
   //USART2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 

}

/*******************************************************************************
- * @函数名称	usart_send_nbytes
- * @函数说明   发送一定长的数据
- * @输入参数   send_buf：数据指针，num：长度
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void usart_send_nbytes(uint8_t* send_buf,uint16_t num)
{
    int i;
    for (i=0;i<num;i++)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
        USART_SendData(USART2,*send_buf++);
    }
}
/*******************************************************************************
- * @函数名称	usart_send_str
- * @函数说明   发送一定长的字符串
- * @输入参数   str：字符串指针
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void usart_send_str(char* str)
{
    while (*str)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
        USART_SendData(USART2,*str++);
    }
}
/*******************************************************************************
- * @函数名称	UART2_Handle
- * @函数说明   串口中断服务函数
- * @输入参数   无
- * @输出参数   无
- * @返回参数   无
*******************************************************************************/
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        USART_ReceiveData(USART2);
    }
}

/************************************文件结束**********************************/
