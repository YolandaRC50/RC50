#ifndef __CARTRIDGE_DETECT_H
#define __CARTRIDGE_DETECT_H
#include "includes.h"
#define LEVEL_DETECT_ONLINE_VALUE_BUFFER_LEN (uint16_t)30
#define GAS_SOURCE_ONLINE_VALUE_BUFFER_LEN (uint16_t)30 

#define DELAY_AT_STEP Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Delay_AT_Step
#define AN_ACCURACY   Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Step_Jump_AN_Accuracy/*跳转条件精度*/
#define STEP_JUMP_CONDITION_NUM  (uint16_t)Cartridge_Detect_Data.Cartridge_Specification_And_Step_Params_Data.Step_Jump_Condition_Num/*达到跳转条件后进行的再循环判断次数*/
extern double ADV_Online_Value_Buffer[128];

void Cartridge_DetectSystem_Working(void);
void Cartridge_Debugging(void);
void DetectSystem_Productting(void);
void DetectSystem_Detectting(void);

/*检测手动排气开关*/
void Manual_Exhaust_Switch(void); 
 
void Detect_Air_Guage_Data(void);
	/*打开电磁阀3，关闭电磁阀2和电磁阀4，使整个气路内部气体卸掉*/
void DetectSystem_AIR_DIS(void);
	/*管路充气:  打开阀A，关闭阀B，关闭泄气阀，直到压力表压力不再上升为达到跳转条件*/
void DetectSystem_AIR_INFLATION(void);
	/*管路充气平衡: 关闭阀A,关闭阀B，关闭泄气阀，等待2~3秒，达到跳转条件*/
void DetectSystem_AIR_INFLATION_BALANCE(void);	
	/*胶桶充气：保持关闭阀A，打开阀B，关闭泄气阀，直到压力表读数不在下降,延时1秒，读取十次数据，求平均值*/
void DetectSystem_AIR_TO_CARTRIDGE(void);	
	/*测试完成：打开阀A,阀B。关闭泄气阀，直到压力读数不再上升，完成动作*/
void DetectSystem_AIR_DETECT_COMPLETE(void);

uint8_t CheckOnlineData(void);
#endif
