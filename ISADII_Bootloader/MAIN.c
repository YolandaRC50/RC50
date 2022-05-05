#include "BSP.h"

uint32_t Start_Loop = 0; 
int main(void)
{
	
	BSP_Init();	 
	
	delay_ms(100);
	
	//printf("正在循环扫描RCC_GetFlagStatus,当状态为SET时自动跳过...\r\n");
	
	//while ( RCC_GetFlagStatus(RCC_FLAG_SFTRST) == RESET);
	
	printf("所有驱动初始化完成，正在开始循环扫描USB设备...\r\n");
	while (1)
	{
		if(!jump_flag &&Detect_USB==0)
		{
			
			printf("第%d次U盘扫描...\r\n",Start_Loop);
			
			USBH_Process(&USB_OTG_Core, &USB_Host);   
		}  
		if(jump_flag)
		{ 
		 
			if(Detect_USB==1) 
			{ 
				USBH_DeInit(&USB_OTG_Core,&USB_Host);
			}	
			__disable_irq();
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
		}
		delay_ms(1);
		if(Detect_USB==0)
		{
			if(Start_Loop > 500)
			{
				Start_Loop=0;
				jump_flag = 1;
				
			}
			Start_Loop++;
		}
	}
}

