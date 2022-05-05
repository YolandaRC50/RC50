#include "I2C.h"
#include "sys_timer.h"

void EEPI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
	
	SDA_H;  
	SCL_H;  
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

void I2CDELAY(void)
{
  delay_us(2);          // Quick delay
}

/**************起始信号***************/
void IIC_Start(void)
{       
  SDA_OUT();
	SDA_L;
  SDA_OUT();
	SCL_H;
	I2CDELAY();
	SDA_H;
	I2CDELAY();
	SDA_L;
	I2CDELAY();
	SCL_L;
	I2CDELAY();
}
/*************终止信号*************/
void IIC_Stop(void)
{       
	SDA_OUT();
	SDA_L;
	I2CDELAY();
	SCL_H;
	I2CDELAY();
	SDA_H;
	I2CDELAY();
}

//产生ACK应答
void IIC_Ack(void)
{
	SCL_L;
	SDA_OUT();
	SDA_L;
	I2CDELAY();
	SCL_H;
	I2CDELAY();
	SCL_L;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	SCL_L;
	SDA_OUT();
	SDA_H;
	I2CDELAY();
	SCL_H;
	I2CDELAY();
	SCL_L;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	
	SDA_H;
	SDA_IN();      //SDA设置为输入  	
	I2CDELAY();	   
	SCL_H;
	I2CDELAY();	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;//时钟输出0 	   
	return 0;  
} 
/*************向I2C总线发送一个字节************/
void IIC_Send_Byte(uint16_t byte)
{
	uint16_t i;
        
	SDA_OUT();
	for(i=0;i<8;i++)
	{
	   if((byte<<i) & 0x80)
		   SDA_H;
	   else
		   SDA_L;
		 I2CDELAY();
	   SCL_H;
		 I2CDELAY();
     SCL_L;
	 }
  SDA_H;
  SCL_H;
  SCL_L;
}
/**************读取一个字节**************/
uint16_t IIC_Read_Byte(uint8_t ack)
{
    uint16_t temp=0,i;
    SDA_IN();
    for(i=0;i<8;i++)
    {
    	temp=temp<<1;
    	SCL_L;
			I2CDELAY();
      SCL_H;
			I2CDELAY();
      if(READ_SDA)
      {
				temp|=0x01;
      }
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK 
    return temp;
}
