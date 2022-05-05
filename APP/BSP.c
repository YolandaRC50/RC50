
#include "BSP.h"

#include "malloc.h"
//FATFS fs;

void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	
	delay_init(168);
	//delay_ms(1000);
	TIMx_Init();
	//
	//Pressure Sensor
	PressureSensor_Init();
	//HMI
	HMI_USART_Init();
	  
	BELL_Init();
	LED_Init();
  Bluetoorh_io_init();
	//HARDWARE/FLASH_EEPROM
	AT24CXX_Init();
	W25QXX_Init();
	//ResetAllParams();
	LoadSystemParams();
	
	//HARDWARE/GPIO
	GOIO_Init();
	
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
//	my_mem_init(SRAMEX);		        //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		          //��ʼ��CCM�ڴ��
	
	//HARDWARE/SENSOR
	DHTxx_Init();
	
	Communication_Param_Init();
 
	
	//Files
	
	#if FILES_ENABLE == 1
	Load_Cread_LogFolder();
	file_write(RUN_STOP_LOG,"POWER ON");
	#endif
	
	#if WDG_ENABLE//ʹ�ܿ��Ź�����
	dwg_init();
	#endif
}