#include "ADS1115.h"
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

/**************************************
起始信号
**************************************/
void ADS1115_Start()
{
    I2C_ADC_SDA_OUT();
    delay_us(5);

    I2C_ADC_SDA_H; //拉高数据线
    delay_us(5);
    I2C_ADC_SCL_H; //拉高时钟线
    delay_us(5); //延时
    I2C_ADC_SDA_L; //产生下降沿
    delay_us(5); //延时
    I2C_ADC_SCL_L; //拉低时钟线
}

/**************************************
停止信号
**************************************/
void ADS1115_Stop()
{
    I2C_ADC_SDA_OUT();
    delay_us(5);
    I2C_ADC_SDA_L; //拉低数据线
    delay_us(5);
    I2C_ADC_SCL_H; //拉高时钟线
    delay_us(5); //延时
    I2C_ADC_SDA_H; //产生上升沿
    delay_us(5); //延时
}

//**************************************
//向IIC总线发送一个字节数据
//**************************************
void ADS1115_SendByte(char dat)
{
    unsigned char i;

    I2C_ADC_SDA_OUT();
    delay_us(5);
    for (i=0; i<8; i++) //8位计数器
    {
			if((dat&0x80) >> 7)
				I2C_ADC_SDA_H;
			else
				I2C_ADC_SDA_L;
      delay_us(5);

        dat <<= 1; //移出数据的最高位
        I2C_ADC_SCL_H; //拉高时钟线
        delay_us(5); //延时
        I2C_ADC_SCL_L; //拉低时钟线
        delay_us(5); //延时
    }
}

/**************************************
//从IIC总线接收一个字节数据
**************************************/
unsigned char ADS1115_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;

    I2C_ADC_SDA_IN();

    for (i=0; i<8; i++) //8位计数器
    {
        dat <<= 1;

        I2C_ADC_SCL_H; //拉高时钟线
        delay_us(5); //延时

        dat |= I2C_ADC_READ_SDA; //读数据

        I2C_ADC_SCL_L; //拉低时钟线
        delay_us(5); //延时
    }
    return dat;
}

char ADS1115_RecvACK(void)
{
    uint8_t WaitTime = 0;

    I2C_ADC_SDA_OUT();
    delay_us(5);
    I2C_ADC_SDA_H;
    delay_us(5); //延时

    I2C_ADC_SDA_IN();
    delay_us(5);
    I2C_ADC_SCL_H;
    delay_us(5); //延时
    while(I2C_ADC_READ_SDA)
    {
        WaitTime++;
        delay_ms(1);
        if(WaitTime > 200)
        {
            ADS1115_Stop();
            return 1;
        }
    }
    I2C_ADC_SCL_L;
    return 0;
}

void ADS1115_Confige(char channel)
{
    unsigned char i = 0;
    char Initdata[4];
    switch(channel)
    {	
	case 0: Initdata[2] = 0xC2; break;
	case 1: Initdata[2] = 0xD2; break;
	case 2: Initdata[2] = 0xE2; break;
	case 3: Initdata[2] = 0xF2; break;

	case 10:  Initdata[2] = 0x82; break;
	case 20:  Initdata[2] = 0xB2; break;
    }

    Initdata[0] = 0x90;
    Initdata[1] = 0x01;
    Initdata[3] = 0xE2;
    I2C_ADC_SCL_H;
    delay_us(5);
    ADS1115_Start();
    delay_us(5);

    for(i = 0;i < 4;i++)
    {
        ADS1115_SendByte(Initdata[i]);
	delay_us(5);
        ADS1115_RecvACK();
	delay_us(5);
    }
    ADS1115_Stop();
    delay_us(5);
}

void ADS1115_PointReg(void)
{
    unsigned char i = 0;
    char Initdata[2];

    Initdata[0] = 0x90;
    Initdata[1] = 0x00;

    I2C_ADC_SDA_OUT();
    delay_us(5);
    I2C_ADC_SCL_H;
    delay_us(5);
    ADS1115_Start();
    delay_us(5);
    for(i = 0;i < 2;i++)
    {
        ADS1115_SendByte(Initdata[i]);
	delay_us(5);
        ADS1115_RecvACK();
        delay_us(5);
    }
    ADS1115_Stop();
    delay_us(5);
}

unsigned int ADS1115_Getresult(void)
{
    unsigned char Result_L,Result_H;
    unsigned int  Result;

    I2C_ADC_SCL_H;
    delay_us(5);
    ADS1115_Start();
    delay_us(5);
    ADS1115_SendByte(0x91);
    delay_us(5);
    ADS1115_RecvACK();
    delay_us(5);

    Result_H = ADS1115_RecvByte();
    delay_us(5);
    Result_L = ADS1115_RecvByte();
    delay_us(5);
    ADS1115_Stop();
    delay_us(5);

    Result = Result_H*256 + Result_L;

    return Result;
}

float ADS1115_Get_channelRes(uint8_t channel)
{
    unsigned int  Result;
    float res;

    I2C_ADC_SDA_OUT();

    ADS1115_Confige(channel);
    delay_ms(5);
    ADS1115_PointReg();
    delay_ms(5);
    Result = ADS1115_Getresult();
    //res = (float)Result * (float)4.096 / (float)32767 ;

    return Result;
}

void ADS1115_Init(void)
{
	I2C_ADC_Init();
}
