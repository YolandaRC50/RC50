#ifndef ADS1115_PRESSURE_DMA_HEAD
#define ADS1115_PRESSURE_DMA_HEAD
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
 
//---------------------------------------------------------------------

#if (1==1)//ADS 1115 Params
#define Accuracy 32768 //定义ADC精度，ADS1115单端输入时15位 

#define ADDRESS_0 0x90 //ADDR PIN ->GND 
#define ADDRESS_1 0x92 //ADDR PIN ->VDD 
#define ADDRESS_2 0x94 //ADDR PIN ->SDA 
#define ADDRESS_3 0x96 //ADDR PIN ->SCL 

/************POINTER REGISTER*****************/ 
#define Pointer_0 0x00 //Convertion register 
#define Pointer_1 0x01 //Config register 
#define Pointer_2 0x02 //Lo_thresh register 
#define Pointer_3 0x03 //Hi_thresh register 
  
/************CONFIG REGISTER*****************/ 
#define OS_0 0x0000 
#define OS_1 0x8000 
  
#define MUX_0 0x0000 //AINp=AIN0, AINn=AIN1 
#define MUX_1 0x1000 //AINp=AIN0, AINn=AIN3 
#define MUX_2 0x2000 //AINp=AIN1, AINn=AIN3 
#define MUX_3 0x3000 //AINp=AIN2, AINn=AIN3 
#define MUX_4 0x4000 //AINp=AIN0, AINn=GND 
#define MUX_5 0x5000 //AINp=AIN1, AINn=GND 
#define MUX_6 0x6000 //AINp=AIN2, AINn=GND 
#define MUX_7 0x7000 //AINp=AIN3, AINn=GND 
#define PGA_0 0x0000 //FS=6.144V 
#define PGA_1 0x0200 //FS=4.096V 
#define PGA_2 0x0400 //FS=2.048V 
#define PGA_3 0x0600 //FS=1.024V 
#define PGA_4 0x0800 //FS=0.512V 
#define PGA_5 0x0A00 //FS=0.256V 
#define PGA_6 0x0C00 //FS=0.256V 
#define PGA_7 0x0E00 //FS=0.256V 
  
#define MODE_0 0x0000 
#define MODE_1 0x0100 
  
#define DR_0 0x0000 //Data Rate = 8 
#define DR_1 0x0020 //Data Rate = 16 
#define DR_2 0x0040 //Data Rate = 32 
#define DR_3 0x0060 //Data Rate = 64 
#define DR_4 0x0080 //Data Rate = 128 
#define DR_5 0x00A0 //Data Rate = 250 
#define DR_6 0x00C0 //Data Rate = 475 
#define DR_7 0x00E0 //Data Rate = 860 
  
#define COMP_MODE_0 0x0000 
#define COMP_MODE_1 0x0010 
#define COMP_POL_0 0x0000 
#define COMP_POL_1 0x0008 
#define COMP_LAT_0 0x0000 
#define COMP_LAT_1 0x0040 
#define COMP_QUE_0 0x0000 
#define COMP_QUE_1 0x0001 
#define COMP_QUE_2 0x0002 
#define COMP_QUE_3 0x0003 
  
//*************ADDR Initial********************/ 
#define ADDRESS ADDRESS_0 //ADDR PIN ->GND  
#define ADDRESS_W ADDRESS|0x00 //写地址
#define ADDRESS_R ADDRESS|0x01 //读地址 
/*************Config Initial*********************/ 
#define OS OS_1  
#define MUX_A0 MUX_0 //AINp=AIN0, AINn=AIN1 
#define MUX_A1 MUX_3  //AINp=AIN2, AINn=AIN3
#define MUX_A2 MUX_6 //AINp=AIN2, AINn=GND 
#define MUX_A3 MUX_7 //AINp=AIN3, AINn=GND 
#define PGA PGA_4 //FS=6.144V  
#define MODE MODE_0 //Continuous conversion mode 
#define DR DR_4 //Data Rate = 860 
#define COMP_QUE COMP_QUE_3 
//--------------------------------------------------------------------------------------

#define uchar unsigned char
#define uint unsigned int
	
#define I2C_ADC_SDA_PORT	GPIOE
#define I2C_ADC_SDA_Pin		GPIO_Pin_7
#define I2C_ADC_SCL_PORT	GPIOG
#define I2C_ADC_SCL_Pin		GPIO_Pin_1


#define SDA_1  GPIO_SetBits(I2C_ADC_SDA_PORT,I2C_ADC_SDA_Pin)
#define SDA_0  GPIO_ResetBits(I2C_ADC_SDA_PORT,I2C_ADC_SDA_Pin)
#define SCL_1  GPIO_SetBits(I2C_ADC_SCL_PORT,I2C_ADC_SCL_Pin)
#define SCL_0  GPIO_ResetBits(I2C_ADC_SCL_PORT,I2C_ADC_SCL_Pin)
#define GET_SDA GPIO_ReadInputDataBit(I2C_ADC_SDA_PORT, I2C_ADC_SDA_Pin); //读入SDA数值

#define TRUE  1 
#define FALSE 0 
#endif

extern uint  Config;  
extern uchar Writebuff[4];
extern uchar Readbuff[3];
extern uint  Result[2]; 
extern unsigned long ADC_DAT;


void ADS1115_Pressure_Init(void);
float ADS1115_StartChannlel(uint16_t S_MUX);

///-----------------------------------------------


void SDA_GPIO_OUTP(void);
void SDA_GPIO_INP(void);

void _nop_(void);
void Delay_us_15 (void);
void Delay_ms_1 (void);
void ADS1115_Start(void);
void ADS1115_Stop(void); 
void ADS1115_ACK(void); 
void ADS1115_NACK(void); 
uchar ADS1115_Check(void);
void ADS1115_Write_1_Byte(unsigned char DataByte); 
unsigned char ADS1115_Write_N_Byte(unsigned char *writebuffer,unsigned char n); 
uchar ADS1115_Read_1_Byte(void); 
void ADS1115_Read_N_Byte(unsigned int*readbuff, unsigned char n);
void ADS1115_InitADS1115(uint16_t S_MUX); 
void ADS1115_WriteWord(void); 
void ADS1115_ReadWord(void); 
void ADS1115_Config(uint16_t S_MUX); 


#endif