#include "DHTxx.h"  

    
//复位DHTxx
void DHTxx_Rst(void)	   
{                 
	DHTxx_IO_OUT(); 	//SET OUTPUT
  DHTxx_OUT_RESET; 	//拉低DQ
  delay_ms(20);    	//拉低至少18ms
  DHTxx_OUT_SET; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}
//等待DHTxx的回应
//返回1:未检测到DHTxx的存在
//返回0:存在
u8 DHTxx_Check(void) 	   
{   
	u8 retry=0;
	DHTxx_IO_IN();//SET INPUT	 
    while (DHTxx_DQ_IN&&retry<100)//DHTxx会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHTxx_DQ_IN&&retry<100)//DHTxx拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//从DHTxx读取一个位
//返回值：1/0
u8 DHTxx_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHTxx_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHTxx_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHTxx_DQ_IN)return 1;
	else return 0;		   
}
//从DHTxx读取一个字节
//返回值：读到的数据
u8 DHTxx_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHTxx_Read_Bit();
    }						    
    return dat;
}
//从DHTxx读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint16_t _ReadCount=0;
uint16_t _MaxReadCount=100;
u8 DHTxx_Read_Data()    
{        
 	u8 buf[5];
	u8 i;
	DHTxx_Rst();
	if(DHTxx_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHTxx_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{ 
			Sys_RT_Status.Temperature_RT=buf[2]*1.001f;
			Sys_RT_Status.Humidity_RT=buf[0]*1.001f;
			_ReadCount=0;
		} 
	}
	else 
	{
			if(_ReadCount<_MaxReadCount)
			{
				_ReadCount++;
			}
			else
			{
//				Sys_RT_Status.Temperature_RT=-1;
//				Sys_RT_Status.Humidity_RT=-1;
			}
		return 1;
	}
	return 0;	    
}
//初始化DHTxx的IO口 DQ 同时检测DHTxx的存在
//返回1:不存在
//返回0:存在    	 
u8 DHTxx_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(DHTxx_RCC_AHB1Periph_GPIOx, ENABLE);//使能GPIOG时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = DHTxx_WIRE_GPIO_Pin ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(DHTxx_GPIOx_Port, &GPIO_InitStructure);//初始化
	DHTxx_Rst();
	return DHTxx_Check();
}
