#include "BSP.h"
#include "Bell.h"
USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;


void GOIO_OUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_ResetBits(GPIOA,GPIO_Pin_4);  //蜂鸣器对应引脚GPIOF8拉低， 
}

void BSP_Init(void)
{
	 
	__enable_irq() ;
	
	 NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x8000000); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //初始化中断分组
	delay_init(168);	
	
	delay_ms(50); 
	GOIO_OUT_Init();
	
	
	HMI_USART_Init();
	delay_ms(5*100); 
	//USART_DEBUG_Init(115200);
	delay_ms(1*100); 
	printf("调试串口初始化完成...\r\n");
	u8 sta=0;
	delay_ms(1*100); 
	//LED_Init();
	
	//BELL_Init();
	printf("my_mem_init...\r\n");
	my_mem_init(SRAMIN);	//初始化内部内存池	
	printf("exfuns_init...\r\n");
 	sta=exfuns_init();			  //为fatfs相关变量申请内存 
	
	delay_ms(1*100); 
	
	printf("开始初始化USB设备...\r\n");
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	printf("USB设备初始化完成...\r\n");
	//USBH_Process(&USB_OTG_Core, &USB_Host);   
	//f_mount(fs[2],"2:",1); 	//挂载U盘   
	//W25QXX_Init();
	printf("USARTDIS_Init...\r\n");
	queue_reset();
	printf("queue_reset...\r\n");
	delay_ms(1*100); 
	
	//USBH_Process(&USB_OTG_Core, &USB_Host);
	
}
