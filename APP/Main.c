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
	/* �������� */
	AppTaskCreate();
	User_Shell_Init();
	/* ��������ͨ�Ż��� */
	AppObjCreate();	
  /* �������ȣ���ʼִ������ */
	vTaskStartScheduler();	
	while(1)
		{	
			uint32_t i=0x1fff000;
//			printf("123\r\n");
				while(i--);
		
		};     //�����������
}

#if DEBUG ==1
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
	  shellHandler(&shell, Res);
	} 
}
#endif
