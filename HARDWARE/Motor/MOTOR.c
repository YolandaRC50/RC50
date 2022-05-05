#include "MOTOR.h"
#include "ADC.h"
#include "DAC8562.h"
#include "Control.h"
#include "sys_timer.h" 

void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = MD_DIB04 | MD_DIB03 | MD_DIB02;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MD_BPORT_2_3_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MD_DIB01;
	GPIO_Init(MD_BPORT_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MD_DOB04;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(MD_BPORT_O4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MD_DOB03;
	GPIO_Init(MD_BPORT_2_3_4, &GPIO_InitStructure);
	
	//GPIO_ResetBits(MD_BPORT_2_3_4, MD_DOB03);
	GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB04);
	GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB03);
	GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB02);
	GPIO_ResetBits(MD_BPORT_O4, MD_DOB04);
	GPIO_ResetBits(MD_BPORT_1, MD_DIB01);
	
	/************************************************************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = /*MD_DIA04 |*/ MD_DIA03;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MD_APORT_3_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MD_DIA01 | MD_DIA02;
	GPIO_Init(MD_APORT_1_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MD_DIA04;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MD_APORT_3_4, &GPIO_InitStructure);
	
	//GPIO_ResetBits(MD_APORT_3_4, MD_DOA04);
	//GPIO_ResetBits(MD_APORT_3_4, MD_DOA03);
	//GPIO_ResetBits(MD_APORT_3_4, MD_DIA04);
	GPIO_ResetBits(MD_APORT_3_4, MD_DIA03);
	GPIO_ResetBits(MD_APORT_1_2, MD_DIA02);
	GPIO_ResetBits(MD_APORT_1_2, MD_DIA01);
/************************************************************************************/	

	GPIO_InitStructure.GPIO_Pin = MOTORA_ID_S;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MOTORA_ID_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MOTORB_ID_S;
	GPIO_Init(MOTORB_ID_PORT, &GPIO_InitStructure);
	
	//ADC_Config();
	 
	
	//初始化Motor 1 使能
	GPIO_SetBits(MD_APORT_1_2, MD_DIA02);
	GPIO_ResetBits(MD_APORT_3_4, MD_DIA03);
	GPIO_SetBits(MD_APORT_1_2, MD_DIA01);
		
	
	//初始化Motor 2 使能
	GPIO_SetBits(MD_BPORT_2_3_4, MD_DIB03);
	GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB02);
	
		
	//初始化Motor 2 模拟量
	DAC8562_SetData(0 ,0, 0,0);
	DAC8562_SetData(0 ,1, 0,0);
}

#define MAX_SPEED_vol 7000
#define MAX_SPEED_C_vol 1000
#define MAX_OUT_VOL	 4
#define parmter_down -4
 

#define MOTOR_MAX_SPEED 												7000.0f
#define MOTOR_MAX_SPEED_C 										1000.0f
#define MAX_AN																		65535.0f 						//10.0V   两端作为死区
#define MIN_AN																			0.0f    							//0.0V     两端作为死区
#define MOTOR_SPEED_RATIO											69.0f
double VOL_TO_SPPED(uint16_t vol, uint8_t cmd)
{
	uint32_t tem = 0;
	double result = 0;	
	
	if(vol < 32271)
	{
		vol = 32271;
	}
	if(vol > 56101)
	{
		vol = 56101;
	}
	
	if(cmd)
	{
		tem = (vol - 32271);//
		result = (double)tem * (double)3.3;
		result = result / (double)parmter_down;
		result = result / (double)65535;
		tem = ((double)MAX_SPEED_vol / (double)MAX_OUT_VOL);
		result = result * tem;
	}else
	{
		tem = (vol - 32271);//
		result = (double)tem * (double)3.3;
		result = result / (double)parmter_down;
		result = result / (double)65535;
		tem = ((double)MAX_SPEED_C_vol / (double)MAX_OUT_VOL);
		result = result * tem;
	}
	
	if(result > 65535)
	{
		result = 65535;
	}
	
	return (uint16_t)result;
}

float getPumpCalibrationVol(uint8_t PU15_16)
{
	float RT_Value =0.0f;//(PU15_CALIBRATION_RATE/60.0f)*10.0f;
	if(PU15_16==16)
	{
		RT_Value=(((PU16_CALIBRATION_RATE/60.0f)* PUMP_CALIRATION_TIME_S )/**PU16_VOLUME_PER_REVOLUTION*/)/1000.0f;
	}
	else
	{
		RT_Value=((PU15_CALIBRATION_RATE/60.0f)*PUMP_CALIRATION_TIME_S );//* PU15_VOLUME_PER_REVOLUTION;
	}
	return RT_Value;
}

float ANALOG_TO_SPEED(float Analog, uint8_t cmd)
{
	uint16_t tem = 0;
	uint16_t result = 0;	
	tem=(Analog-2020.0f);
	result=(tem/(3520.0f-2020.0f))*7000.0f;
	
	return ((uint16_t)result)/MOTOR_SPEED_RATIO;
}


float SPEED_TO_ANALOG(float speed, uint8_t cmd)
{
		float result = 0;
		float Pump_Max_Rate= MOTOR_MAX_SPEED/MOTOR_SPEED_RATIO; //
		float ANALOG_SPEED=0.0f;
		
		if(speed<0.01f)
		{
			return 0.0f;
		}
		
		if(SPEED.PU15_16 == 16)
		{
			
				Pump_Max_Rate = PU16_VOLUME_PER_REVOLUTION*Pump_Max_Rate; 
		} 
		else
		{
				Pump_Max_Rate = PU15_VOLUME_PER_REVOLUTION*Pump_Max_Rate; 
		} 
	
		
		if(cmd)
		{ 
					ANALOG_SPEED= speed/ (SPEED.speed_revolation/100);	
					result  = (ANALOG_SPEED/Pump_Max_Rate)*(MAX_AN-MIN_AN)+MIN_AN;
		}
		else
		{
					result  = (speed/Pump_Max_Rate)*(MAX_AN-MIN_AN)+MIN_AN;
		}			
		
		if(result > MAX_AN)
		{
			result = MAX_AN;
		}
		
		return result;
}



void Acc_Dec_To_Anlog(void)
{ 
		float Acc_dec=(SPEED.Pump_Acc_Dec/MOTOR_MAX_ACC) * 65535.0f; 
		DAC8562_SetData(SPEED.speed_ch ,0, (uint16_t)Acc_dec,0);
		
		delay_us(100);
	
		SPEED_Check();
}



uint8_t SPEED_i = 0;
float SPEED_AVR[4] = {0};
float M_Speed=0;
uint8_t SPEED_Check(void)
{  
		SPEED.speed_set_an[SPEED.speed_ch] = (uint16_t)SPEED_TO_ANALOG(SPEED.speed_set[SPEED.speed_ch], SPEED.Pump_Calibration_Status[SPEED.speed_ch]);	
		DAC8562_SetData(SPEED.speed_ch ,1, SPEED.speed_set_an[SPEED.speed_ch],0);

		return 0;
 
}

void SPEED_Control(void)
{
	Acc_Dec_To_Anlog();
//	uint16_t tem = 0;
	if(SPEED.speed_upflag)
	{
		if(SPEED.speed_ch == 0)
		{
			
			
			if(SPEED.speed_dir[SPEED.speed_ch] == acc_r && SPEED.speed_set[SPEED.speed_ch]>0.0f)
			{
				GPIO_SetBits(MD_APORT_3_4, MD_DIA03);
				GPIO_ResetBits(MD_APORT_1_2, MD_DIA02);
			}
			else
			{
				GPIO_SetBits(MD_APORT_1_2, MD_DIA02);
				GPIO_ResetBits(MD_APORT_3_4, MD_DIA03);
			}
			if(SPEED.speed_set[SPEED.speed_ch]>0)
			{
				//DAC8562_Reset(); 
				//Acc_Dec_To_Anlog();
				
				GPIO_ResetBits(MD_APORT_1_2, MD_DIA01);
			}
			else
			{
				GPIO_SetBits(MD_APORT_1_2, MD_DIA01);
			}
		}
		else
		{
			if(SPEED.speed_dir[SPEED.speed_ch] == acc_r && SPEED.speed_set[SPEED.speed_ch]>0.0f)
			{
				GPIO_SetBits(MD_BPORT_2_3_4, MD_DIB03);
				GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB02);
			}
			else
			{
				GPIO_SetBits(MD_BPORT_2_3_4, MD_DIB03);
				GPIO_ResetBits(MD_BPORT_2_3_4, MD_DIB02);
			}
		}
		
		//tem = SPEED_TO_ANALOG(SPEED.speed_acr[SPEED.speed_ch], 0);
		
		SPEED.speed_set_an[SPEED.speed_ch] = (uint16_t)SPEED_TO_ANALOG(SPEED.speed_set[SPEED.speed_ch], SPEED.Pump_Calibration_Status[SPEED.speed_ch]); 
		DAC8562_SetData(SPEED.speed_ch ,1, SPEED.speed_set_an[SPEED.speed_ch],1);
		
		delay_us(100);
		float Acc_dec=(SPEED.Pump_Acc_Dec/MOTOR_MAX_ACC) * 65535.0f; 
		DAC8562_SetData(SPEED.speed_ch ,0, (uint16_t)Acc_dec,0);
		
		SPEED.speed_upflag = 0;
		if(SPEED.speed_set[0] > 0)  ///这里用来作为点胶开胶时间记时
			TIM_Cmd(TIM3,ENABLE);
	}
}

uint8_t Motor_ControllerStatusCheck(void)
{
	uint8_t Motor_Controller_Alm=GPIO_ReadInputDataBit(MD_APORT_3_4,MD_DIA04); //MD_DIA04
	 
	
	if(Motor_Controller_Alm==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
uint8_t Motor_ConnectCheck(void)
{ 
	uint8_t Motor_ID=GPIO_ReadInputDataBit(MOTORA_ID_PORT, MOTORA_ID_S);
	 
	
	if(Motor_ID==1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int16_t CheckPumpSpeed(void)
{
	  // filter();
		//u16 Speed=GetVolt(ADC_S[1]);
		//SPEED.Motot_Speed_Online=	ADC_S[1];//ANALOG_TO_SPEED(ADC_S[1],0);
		//uint8_t Motor_Dispensing_Status=GPIO_ReadOutputDataBit(MD_APORT_1_2, MD_DIA01);
		int16_t __SpeedStatus=0; 

	if(GetMotorRunStatus()==0)
		return 0;
	
		if(SPEED.PU15_16==15)
		{
			if((SPEED.speed_set[SPEED.speed_ch]/12.0f)<0.5f)
			{
				__SpeedStatus=-1;
			}
			else if((SPEED.speed_set[SPEED.speed_ch]/12.0f)>120.0f)
			{
				__SpeedStatus=1;
			}
		}
		else
		{
			if((SPEED.speed_set[SPEED.speed_ch]/50.0f)<0.5f)
			{ 
				__SpeedStatus=-1;
			}
			else if((SPEED.speed_set[SPEED.speed_ch]/50.0f)>120.0f)
			{
				
				__SpeedStatus=1;
			}
		}
				
	
	return	__SpeedStatus;
	
}

uint8_t GetMotorRunStatus(void)
{
	uint8_t Motor_Dispensing_Status=GPIO_ReadOutputDataBit(MD_APORT_1_2, MD_DIA01);
	return 1-Motor_Dispensing_Status;
}


void MotorControllerReset(void)
{
	GPIO_ResetBits(MD_APORT_3_4, MD_DIA03);
	GPIO_ResetBits(MD_APORT_1_2, MD_DIA02);
	GPIO_ResetBits(MD_APORT_1_2, MD_DIA01);
	
	delay_ms(200); 
	GPIO_SetBits(MD_APORT_1_2, MD_DIA02);
	delay_ms(100);
	GPIO_SetBits(MD_APORT_1_2, MD_DIA01);
}
