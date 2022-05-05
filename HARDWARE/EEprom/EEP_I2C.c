#include "EEP_I2C.h"
#include "sys_timer.h"

void EEP_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(EEP_I2C_RCC_AHB1Periph_GPIOx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = EEP_I2C_SDA_Pin | EEP_I2C_SCL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(EEP_I2C_PORT, &GPIO_InitStructure);
	
	EEP_I2C_SDA_H;  
	EEP_I2C_SCL_H;  
}

void EEP_I2C_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = EEP_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(EEP_I2C_PORT, &GPIO_InitStructure);
}

void EEP_I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = EEP_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(EEP_I2C_PORT, &GPIO_InitStructure);
}

void I2CDELAY(void)
{
  delay_us(2);          // Quick delay
}

/**************起始信号***************/
void EEP_I2C_Start(void)
{       
  EEP_I2C_SDA_OUT();
	SEEP_I2C_SDA_L;
  EEP_I2C_SDA_OUT();
	EEP_I2C_SCL_H;
	I2CDELAY();
	EEP_I2C_SDA_H;
	I2CDELAY();
	SEEP_I2C_SDA_L;
	I2CDELAY();
	EEP_I2C_SCL_L;
	I2CDELAY();
}
/*************终止信号*************/
void EEP_I2C_Stop(void)
{       
	EEP_I2C_SDA_OUT();
	SEEP_I2C_SDA_L;
	I2CDELAY();
	EEP_I2C_SCL_H;
	I2CDELAY();
	EEP_I2C_SDA_H;
	I2CDELAY();
}

//产生ACK应答
void EEP_I2C_Ack(void)
{
	EEP_I2C_SCL_L;
	EEP_I2C_SDA_OUT();
	SEEP_I2C_SDA_L;
	I2CDELAY();
	EEP_I2C_SCL_H;
	I2CDELAY();
	EEP_I2C_SCL_L;
}
//不产生ACK应答		    
void EEP_I2C_NAck(void)
{
	EEP_I2C_SCL_L;
	EEP_I2C_SDA_OUT();
	EEP_I2C_SDA_H;
	I2CDELAY();
	EEP_I2C_SCL_H;
	I2CDELAY();
	EEP_I2C_SCL_L;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t EEP_I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	
	EEP_I2C_SDA_H;
	EEP_I2C_SDA_IN();      //SDA设置为输入  	
	I2CDELAY();	   
	EEP_I2C_SCL_H;
	I2CDELAY();	 
	while(EEP_I2C_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			EEP_I2C_Stop();
			return 1;
		}
	}
	EEP_I2C_SCL_L;//时钟输出0 	   
	return 0;  
} 
/*************向I2C总线发送一个字节************/
void EEP_I2C_Send_Byte(uint16_t byte)
{
	uint16_t i;
        
	EEP_I2C_SDA_OUT();
	for(i=0;i<8;i++)
	{
	   if((byte<<i) & 0x80)
		   EEP_I2C_SDA_H;
	   else
		   SEEP_I2C_SDA_L;
		 I2CDELAY();
	   EEP_I2C_SCL_H;
		 I2CDELAY();
     EEP_I2C_SCL_L;
	 }
  EEP_I2C_SDA_H;
  EEP_I2C_SCL_H;
  EEP_I2C_SCL_L;
}
/**************读取一个字节**************/
uint16_t EEP_I2C_Read_Byte(uint8_t ack)
{
    uint16_t temp=0,i;
    EEP_I2C_SDA_IN();
    for(i=0;i<8;i++)
    {
    	temp=temp<<1;
    	EEP_I2C_SCL_L;
			I2CDELAY();
      EEP_I2C_SCL_H;
			I2CDELAY();
      if(EEP_I2C_READ_SDA)
      {
				temp|=0x01;
      }
    }
    if (!ack)
        EEP_I2C_NAck();//发送nACK
    else
        EEP_I2C_Ack(); //发送ACK 
    return temp;
}
