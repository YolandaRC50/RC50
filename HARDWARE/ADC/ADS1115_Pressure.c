#include "ADS1115_Pressure.h"



uint  Config;  
uchar Writebuff[4];
uchar Readbuff[3]; 
uint  Result[2]; 
unsigned long ADC_DAT;

#if(1==1)

//**********************************************


//**********************************************
void  ADS1115_Start(void) 
{  
	SCL_1; 
	SDA_1; 
	delay_us(15);  
	SDA_0;  
	delay_us(15);  
	SCL_0;  
	delay_us(15);  
} 
//**********************************************  
void  ADS1115_Stop(void) 
{ 
	SDA_0;  
	delay_us(15);  
	SCL_1;  
	delay_us(15);  
	SDA_1;  
	delay_us(15);  
}
//**********************************************
void  ADS1115_ACK(void) 
{  
	SDA_0;  
	delay_us(2);  
	SCL_1;  
	delay_us(15);  
	SCL_0; 
	delay_us(2);  
	SDA_1;  
	delay_us(15);  
}  
 
//**********************************************
void  ADS1115_NACK(void) 
{ 
	SDA_1; 
	delay_us(2);  
	SCL_1; 
	delay_us(15);  
	SCL_0; 
	delay_us(2);  
	SDA_0; 
	delay_us(15);  
}
//**********************************************
uchar  ADS1115_Check(void) 
{  
	uchar slaveack;  
	
	SDA_1;  
	delay_us(2);  
	SCL_1;  
	delay_us(4); 
	
	SDA_GPIO_INP();
	delay_us(4);  
	slaveack = GET_SDA;
	SDA_GPIO_OUTP();
	
	SCL_0;  
	delay_us(4);  
	
	if(slaveack) 
		return FALSE; 
	else 
		return TRUE; 
}  

//**********************************************
void  ADS1115_Write_1_Byte(unsigned char DataByte) 
{  
	uint i; 

	for(i=0;i<8;i++)  
	{  
		if( DataByte&0x80 ) //if((DataByte<<i)&0x80)  
			SDA_1;  
		else 
			SDA_0;
		 
		delay_us(1);  
		SCL_1;  	
		delay_us(1);  
		SCL_0;  
		delay_us(1);  
		
		DataByte <<= 1;  
	}
  
	SDA_1; 
 	delay_us(1);  
// 	SCL_1;  	
// 	delay_us(1);  
// 	SCL_0; 
}  
//**********************************************
unsigned char  ADS1115_Write_N_Byte(unsigned char *writebuffer,unsigned char n) 
{ 
	uint i; 
 
	for(i=0; i<n; i++)  
	{ 
		 ADS1115_Write_1_Byte(*writebuffer); 
		
		if( ADS1115_Check()) 
		{ 
			writebuffer++; 
		} 
		else 
		{ 
			Stop(); 
			return FALSE;  
		} 
	} 

	Stop(); 	
	return TRUE; 
} 
//**********************************************
uchar  ADS1115_Read_1_Byte(void) 
{  
	uchar data_Value=0,FLAG=0, i=0; 
	
  for(i=0; i<8; i++) 
  { 
	  data_Value <<= 1; 
	  SDA_1; 
		delay_us(1);
		
    SCL_1; 
		delay_us(1);  
	
		SDA_GPIO_INP();
		delay_us(4);  		
		FLAG = GET_SDA;
		SDA_GPIO_OUTP();	
		 
		if( FLAG ) 
			data_Value |= 0x01; 
		
		SCL_0;  
		delay_us(1);
	}  
	
  return data_Value; 
} 
//**********************************************
void  ADS1115_Read_N_Byte(unsigned int*readbuff, unsigned char n) 
{  
	uchar i; 
	for(i=0;i<n;i++) 
	{ 
		readbuff[i]= ADS1115_Read_1_Byte(); 
		
		if(i==n-1) 
			 ADS1115_NACK(); //不连续读字节
		else 
			 ADS1115_ACK(); //连续读字节
		
	}  
  Stop();  
}  
//**********************************************
void  ADS1115_InitADS1115(uint16_t S_MUX) 
{ 
	//if (S_MUX == MUX_A0) //AIN0 
	Config = OS+S_MUX+PGA+DR+COMP_QUE+MODE;  
	
//	if (S_MUX == MUX_A1) //AIN1 
//		Config = OS+MUX_A1+PGA+DR+COMP_QUE+MODE;
//	
//	if (S_MUX == MUX_A2) //AIN2  
//		Config = OS+MUX_A2+PGA+DR+COMP_QUE+MODE; 
//	
//	if (S_MUX == MUX_A3) //AIN3  
//		Config = OS+MUX_A3+PGA+DR+COMP_QUE+MODE;
	
	Writebuff[0]=ADDRESS_W; //GND	
	Writebuff[1]=Pointer_1; //config  register
	Writebuff[2]=Config/256; //high byte
	Writebuff[3]=Config%256; //low byte
	
	Readbuff[0]=ADDRESS_W;  //GND	
	Readbuff[1]=Pointer_0; //Convertion register 
	Readbuff[2]=ADDRESS_R; //读地址 
} 

//**********************************************
 
void  ADS1115_WriteWord(void) 
{  
	 ADS1115_Start(); //写入4个字节
	 ADS1115_Write_N_Byte(Writebuff,4); 
} 
 
//**********************************************
void  ADS1115_ReadWord(void) 
{  
//	int i=0;  
	
	 ADS1115_Start(); //写入2个字节	 
	 ADS1115_Write_N_Byte(Readbuff,2); 
	 ADS1115_Stop();  
	
	 ADS1115_Start(); 	//写入1个字节
	 ADS1115_Write_N_Byte(&Readbuff[2],1); 
	
   ADS1115_Read_N_Byte(Result,2); //读出2个字节 
	
   ADS1115_Stop(); 	
}  

//**********************************************
void ADS1115_Config(uint16_t S_MUX) 
{  
	 ADS1115_InitADS1115(S_MUX); 
	
	 ADS1115_WriteWord();
	
	delay_ms(1); 
	
	 ADS1115_ReadWord(); 
} 
#endif
#if(1==1)

void ADS1115_Pressure_Init(void)
{

//	#define I2C_ADC_SCL_PORT	GPIOG
//	#define I2C_ADC_SCL_Pin		GPIO_Pin_1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SCL_Pin; //SCL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(I2C_ADC_SCL_PORT, &GPIO_InitStructure);
	
	SDA_GPIO_OUTP();
}
 
void SDA_GPIO_OUTP(void)
{
	//	#define I2C_ADC_SDA_PORT	GPIOE
	//	#define I2C_ADC_SDA_Pin		GPIO_Pin_7
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SDA_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(I2C_ADC_SDA_PORT, &GPIO_InitStructure);
}
void SDA_GPIO_INP(void)
{
	//	#define I2C_ADC_SDA_PORT	GPIOE
	//	#define I2C_ADC_SDA_Pin		GPIO_Pin_7
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin = I2C_ADC_SDA_Pin; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING; 
	GPIO_Init(I2C_ADC_SDA_PORT, &GPIO_InitStructure);
}
#endif

/****************************************************************************
* 名    称： StartChannel
* 功    能： 选择通道测量
* 入口参数： 通道序号0、1、2、3
* 出口参数： 采样值
* 说    明:  完成ADS1115的采样工作
****************************************************************************/
float ADS1115_StartChannlel(uint16_t S_MUX)
{
	uint8_t i;
	 float data0=0,data1=0;
	for(i=0,data0=0; i<10; i++)
	{
		 ADS1115_Config(S_MUX);
		data0+=abs((Result[0]*256.0+Result[1]));//)/32768.0;
	}
	return data0/10.0f;//-data1/10.0f;
}



