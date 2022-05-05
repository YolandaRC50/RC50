#ifndef __USART3_H
#define __USART3_H	   

#include "stm32f4xx.h"

#define USART_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART_MAX_SEND_LEN		400					//最大发送缓存字节数

extern uint8_t  USART3_RX_BUF[USART_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern uint8_t  USART3_TX_BUF[USART_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern uint16_t USART3_RX_STA;   						//接收数据状态
extern uint8_t moedbus_busy;
void USART485_Init(uint32_t bound);				//串口3初始化 
void USART232_Init(uint32_t bound);
void USARTDIS_Init(uint32_t bound);

void RS485_Send_Data(uint8_t *buf,uint8_t len);
void TIM7_Int_Init(uint16_t arr,uint16_t psc);
void u3_printf(char* fmt, ...);
void ui_sendsinglechar(uint8_t dat);
void RS485_Receive_Data(uint8_t *buf,uint16_t viTagLen,uint16_t *len);
#endif

