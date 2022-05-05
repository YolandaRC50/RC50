#ifndef Temperature_h
#define Temperature_h

#include "stm32f4xx.h"                  // Device header

#define ch1			0x01
#define ch2			0x02

#define READ							            0x03
#define ACTION_OR_CMD				0x06
#define WRITE									0x10
//#define WRITE_SIGNEL					0x06
#define CIR_TEST								0x08

#define address_2_PV					0x2000 //温度
#define address_2_status			    0x2407
#define address_2_UP					0x2105 //报警上限
#define address_2_DN					0x2106 //报警下限
//#define address_2_SP0					0x2103 //SP0


//一下参数只需要在初始化的时候写一次就OK
#define address_2_INPUT_TYPE			   	0x2C00
#define address_2_INPUT_UNIT			   	0x2C01
#define address_2_SUB2			   				0x2E11
#define address_2_PID_ON_OFF			0x2D14
///////////////////////////////////////////////////////

#define INPUT_TYPE_VALUE			   	0x01
#define INPUT_UNIT_VALUE			   	0x00
#define SUB2_VALUE			   					0x0A
#define PID_ON_OFF_VALUE				0x01
///////////////////////////////////////////////////////

#define address_2_Tar					0x2103 //106 602 r
#define address_2_P						0x2A00
#define address_2_I						0x2A01
#define address_2_D						0x2A02
#define address_2_offset			    0x2723


#define address_2_ACTION_OR_CMD		0x0000 

#define CMW_VALUE								0x0001
#define PID_AUTO_VALUE					0x0301
#define GOTO_PARAMS_VALUE		0x0700
#define GOTO_START_VALUE		    0x0600


typedef struct Temperature
{
	 uint16_t StatusH;
	float Temperature;
	float Working;
	float Target;
	float Upper_Limit;
	float P;
	float I;
	float D; 
	float Offset;
	
	uint16_t INPUT_TYPE_temp;
	uint16_t INPUT_UNIT_temp;
	uint16_t SUB2_temp;
	uint16_t PID_ON_OFF_temp;
	
	
	
	uint16_t AT;
	uint16_t CMW;
	
	
	float Working_Temp;
	float Upper_Limit_Temp;
	float Target_Temp;
	float P_Temp;
	float I_Temp;
	float D_Temp;
	float Offset_Temp;
	 
	uint16_t  PID_Write; 
}Temperature;

extern Temperature Temperature_mes[2];
extern uint8_t Receive_buf[256]; 
extern int16_t Temp_Alm_Counter;

void GetSendCMDBuffer(uint8_t ch, uint8_t viFunction,  uint16_t address, uint16_t dataLen,float SetValueArr[]);
void inf_get(void);
uint16_t TEMINF_UPDOWMSet(uint8_t vich, uint16_t UP, uint16_t DOWN);
uint16_t TEMINF_temget(uint8_t vich,uint8_t TMP_Getloop);
void TEM_TargetSET(uint8_t vich);
void TEM_PIDSET(uint8_t vich);
uint8_t TEM_WriteCMD(uint8_t vich,uint8_t viFunction,uint16_t StartAddress,uint16_t vlValueLength,float SetValueArr[]);
uint8_t TMP_ResolveReceiveData(uint8_t *Receive_buf ,uint8_t vich,uint8_t viFunction,uint16_t StartAddress,uint16_t vlValueLength);
#endif
