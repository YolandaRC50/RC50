#include "I2C_ADC.h"
#include "sys_timer.h"

void I2C_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_ADC_SDA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SCL_Pin;
	GPIO_Init(I2C_ADC_SCL_PORT, &GPIO_InitStructure);
	
	I2C_ADC_SDA_H;  
	I2C_ADC_SCL_H;  
}

void I2C_ADC_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_ADC_SDA_PORT, &GPIO_InitStructure);
}

void I2C_ADC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_ADC_SDA_PORT, &GPIO_InitStructure);
}

void I2C_ADC_I2CDELAY(void)
{
  delay_us(2);          // Quick delay
}

/**************起始信号***************/
void I2C_ADC_Start(void)
{       
  I2C_ADC_SDA_OUT();
	I2C_ADC_SDA_L;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SDA_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SDA_L;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_L;
	I2C_ADC_I2CDELAY();
}
/*************终止信号*************/
void I2C_ADC_Stop(void)
{       
	I2C_ADC_SDA_OUT();
	I2C_ADC_SDA_L;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SDA_H;
	I2C_ADC_I2CDELAY();
}

//产生ACK应答
void I2C_ADC_Ack(void)
{
	I2C_ADC_SCL_L;
	I2C_ADC_SDA_OUT();
	I2C_ADC_SDA_L;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_L;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SDA_H;
}
//不产生ACK应答		    
void I2C_ADC_NAck(void)
{
	I2C_ADC_SCL_L;
	I2C_ADC_SDA_OUT();
	I2C_ADC_SDA_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_H;
	I2C_ADC_I2CDELAY();
	I2C_ADC_SCL_L;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t I2C_ADC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	
	I2C_ADC_SDA_H;
	I2C_ADC_SDA_IN();      //SDA设置为输入  	
	I2C_ADC_I2CDELAY();	   
	I2C_ADC_SCL_H;
	I2C_ADC_I2CDELAY();	 
	while(I2C_ADC_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_ADC_Stop();
			return 1;
		}
	}
	I2C_ADC_SCL_L;//时钟输出0 	   
	return 0;  
} 
/*************向I2C总线发送一个字节************/
void I2C_ADC_Send_Byte(uint16_t byte)
{
	uint16_t i;
        
	SDA_OUT();
	for(i=0;i<8;i++)
	{
	   if((byte<<i) & 0x80)
		   I2C_ADC_SDA_H;
	   else
		   I2C_ADC_SDA_L;
		 I2C_ADC_I2CDELAY();
	   I2C_ADC_SCL_H;
		 I2C_ADC_I2CDELAY();
     I2C_ADC_SCL_L;
	 }
  I2C_ADC_SDA_H;
  I2C_ADC_SCL_H;
  I2C_ADC_SCL_L;
}
/**************读取一个字节**************/
uint16_t I2C_ADC_Read_Byte(void)
{
    uint16_t temp=0,i;
    SDA_IN();
    for(i=0;i<8;i++)
    {
    	temp=temp<<1;
    	I2C_ADC_SCL_L;
			I2C_ADC_I2CDELAY();
      I2C_ADC_SCL_H;
			I2C_ADC_I2CDELAY();
      if(I2C_ADC_READ_SDA)
      {
				temp|=0x01;
      }
    }

    I2C_ADC_Ack(); //发送ACK 
    return temp;
}
