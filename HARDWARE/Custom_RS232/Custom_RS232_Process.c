#include "custom_rs232_process.h"
//#include "FlashData_Control.h"

POWER_STATUS Custom_Rs232_CMD_HandShake_Status=POWER_OFF;
static CUSTOM_RS232_QUEUE Custom_rs232_que = {0,0,0};                                            //指令队列
static uint32_t Custom_rs232_cmd_state = 0;                                           				   //队列帧尾检测状态
static Custom_rs232_qsize Custom_rs232_cmd_pos = 0;                                                     //当前指令指针位置
CUSTOM_RS232_CTR_MSG SendToServerMessage=CUSTOM_RS232_CTR_MSG_NONE;
CARTRIDGE_STATUS_FROM_DETECT_SYSTEM Level_Detect_Status;
CUSTOM_RS232_CON_DIV Custom_RS232_Con_Div;
POWER_STATUS DIspensingFinished_Mark=POWER_OFF;
POWER_STATUS System_Power_Level_Detect_Status=POWER_OFF; 

#define CONTROL_SERVER  0/*0--表示RC28端；1--表示加热桶端*/


 uint8_t Custom_rs232_cmd_buffer[CUSTOM_RS232_CMD_MAX_SIZE]={0};//指令缓存  
  
#if(1==1) /*custom_rs232_process*/
void Custom_rs232_queue_reset()
{
    Custom_rs232_que._head = Custom_rs232_que._tail = 0;
    Custom_rs232_cmd_pos = Custom_rs232_cmd_state = 0;
}

void Custom_rs232_queue_push(Custom_rs232_qdata _data)
{
    Custom_rs232_qsize pos = (Custom_rs232_que._head+1)%CUSTOM_RS232_QUEUE_MAX_SIZE;
    if(pos!=Custom_rs232_que._tail)                                                //非满状态
    {
        Custom_rs232_que._data[Custom_rs232_que._head] = _data;
        Custom_rs232_que._head = pos;
    }
}
 
static void Custom_rs232_queue_pop(Custom_rs232_qdata* _data)
{
    if(Custom_rs232_que._tail!=Custom_rs232_que._head)                                          //非空状态
    {
        *_data = Custom_rs232_que._data[Custom_rs232_que._tail];
        Custom_rs232_que._tail = (Custom_rs232_que._tail+1)%CUSTOM_RS232_QUEUE_MAX_SIZE;
    }
}
 
static Custom_rs232_qsize Custom_rs232_queue_size()
{
    return ((Custom_rs232_que._head+CUSTOM_RS232_QUEUE_MAX_SIZE-Custom_rs232_que._tail)%CUSTOM_RS232_QUEUE_MAX_SIZE);
}
 
Custom_rs232_qsize Custom_rs232_queue_find_cmd(Custom_rs232_qdata *buffer,Custom_rs232_qsize buf_len)
{
    Custom_rs232_qsize cmd_size = 0;
    Custom_rs232_qdata _data = 0;

    while(Custom_rs232_queue_size()>0)
    {
        //取一个数据
        Custom_rs232_queue_pop(&_data);

        if(Custom_rs232_cmd_pos==0&&_data!=CUSTOM_RS232_CMD_HEAD)      
        {
            continue;
        }
        //    LED2_ON;
        if(Custom_rs232_cmd_pos<buf_len)                                          
            buffer[Custom_rs232_cmd_pos++] = _data;

        Custom_rs232_cmd_state = ((Custom_rs232_cmd_state<<8)|_data);                          
 
        if(Custom_rs232_cmd_state==CUSTOM_RS232_CMD_TAIL)
        {
            //LED2_ON;
            cmd_size = Custom_rs232_cmd_pos;                                     
            Custom_rs232_cmd_state = 0;                                            
            Custom_rs232_cmd_pos = 0;                                              
 
            return cmd_size;
        }
    }
    return 0;                                                      
}
void CUSTOM_SENDNU8(uint8_t *pData,uint16_t nDataLen)
{
    uint16_t i = 0;
    for (;i<nDataLen;++i)
    {
        CUSTOM_TX_8(pData[i]);
    }
}
void CUSTOM_SENDNU16(uint16_t *pData,uint16_t nDataLen)
{
	    uint16_t i = 0;
    for (;i<nDataLen;++i)
    {
        CUSTOM_TX_16(pData[i]);
    }
}
#endif
#if(1==1) /*custom_rs232_APP*/

void Send_Float(float vf)
{
	uint8_t Byte_Arr[4]={0};		
	Float_to_Byte(vf,Byte_Arr,0) ;
	CUSTOM_TX_8(Byte_Arr[0]);
	CUSTOM_TX_8(Byte_Arr[1]);
	CUSTOM_TX_8(Byte_Arr[2]);
	CUSTOM_TX_8(Byte_Arr[3]);
}

void Send_uint32(uint32_t vl)
{ 
	CUSTOM_TX_8((unsigned char)vl);
	CUSTOM_TX_8((unsigned char)vl>>8);
	CUSTOM_TX_8((unsigned char)vl>>16);
	CUSTOM_TX_8((unsigned char)vl>>24);
}

#if(1==1)
void Custom_rs232_Send_HandShake(CUSTOM_RS232_CON_DIV div)
{
	CUSTOM_BEGIN_CMD();
	CUSTOM_TX_8(CUSTOM_RS232_CMD_HANDSHAKE);
	
	CUSTOM_TX_8(div);
	CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_HANDSHAKE);

	if(div==LEVEL_DETECT_SYSTEM)
	{
//		#if(CONTROL_SERVER==1)
//		Send_Float(Cartridge_Detect_Data.Cartridge_CalibrationAndDetect_Params.Delay_AT_Step);
//		Send_Float(Cartridge_Detect_Data.Cartridge_CalibrationAndDetect_Params.Step_Jump_AN_Accuracy);
//		Send_Float(Cartridge_Detect_Data.Cartridge_CalibrationAndDetect_Params.Step_Jump_Condition_Num);
//		#else
//		Send_Float(Cartridge_Detect_Data.Delay_AT_Step);
//		Send_Float(Cartridge_Detect_Data.Step_Jump_AN_Accuracy);
//		Send_Float(Cartridge_Detect_Data.Step_Jump_Condition_Num);
//		#endif
	
	}
	CUSTOM_END_CMD();
}

 
#endif

#if(CONTROL_SERVER==0)

#if(1==0)

void Product_Ststus_resolving ( uint8_t *Params)
{
	uint8_t Vavel_Status=Params[0];  	 
	Byte_to_Float(&Level_Detect_Status.Pressure_AN,Params,4) ;
	Byte_to_Float(&Level_Detect_Status.Pressure_Bar,Params,8) ;
	 
	if(Vavel_Status&0x01)
	{
		Level_Detect_Status.Vavel_Status_IN=1;
	}
	else
	{
		Level_Detect_Status.Vavel_Status_IN=0;
	}
	
	if(Vavel_Status&0x02)
	{
		Level_Detect_Status.Vavel_Status_Relief=1;
	}
	else
	{
		Level_Detect_Status.Vavel_Status_Relief=0;
	}
	
	if(Vavel_Status&0x04)
	{
		Level_Detect_Status.Vavel_Status_Out=1;
	}
	else
	{
		Level_Detect_Status.Vavel_Status_Out=0;
	}
}
void Cartridge_CAL_Empty_Finished ( uint8_t *Params)
{  
	
	Byte_to_Float(&Level_Detect_Status.LastTestValue,Params,0) ;  	
	Byte_to_Float(&Level_Detect_Status.Pressure_AN,Params,4) ;
	Byte_to_Float(&Level_Detect_Status.Pressure_Bar,Params,8) ;
	
	
	Running_status.cartridge_status=Level_Detect_Status.LastTestValue;
	 
	
	Running_status.cartridge = 1000.0 *
				(Cartridge_Detect_Data.Syringe_Full_Vol
				-Cartridge_Detect_Data.Syringe_Empty_Vol);
	
	 
	 Running_status.cartridge_now=Running_status.cartridge*Running_status.cartridge_status/100.0f;
	
	Cartridge_Detect_Data.Dispensing_Finished_Counter
		=Cartridge_Detect_Data.Auto_Detect_Counter; 
}
void Cartridge_CAL_Full_Finished ( uint8_t *Params)
{   
	
	Byte_to_Float(&Level_Detect_Status.LastTestValue,Params,0) ;  	
	Byte_to_Float(&Level_Detect_Status.Pressure_AN,Params,4) ;
	Byte_to_Float(&Level_Detect_Status.Pressure_Bar,Params,8) ;
	
	
	
	Running_status.cartridge_status=Level_Detect_Status.LastTestValue;
	

	Running_status.cartridge = 1000.0 *
				(Cartridge_Detect_Data.Syringe_Full_Vol
				-Cartridge_Detect_Data.Syringe_Empty_Vol);
	
	
	
	 Running_status.cartridge_now=Running_status.cartridge*Running_status.cartridge_status/100.0f;
	Cartridge_Detect_Data.Dispensing_Finished_Counter
		=Cartridge_Detect_Data.Auto_Detect_Counter; 
}
void Cartridge_Detect_Finished ( uint8_t *Params)
{ 
	
	Byte_to_Float(&Level_Detect_Status.LastTestValue,Params,0) ;  	
	Byte_to_Float(&Level_Detect_Status.Pressure_AN,Params,4) ;
	Byte_to_Float(&Level_Detect_Status.Pressure_Bar,Params,8) ;
	
	Running_status.cartridge_status=Level_Detect_Status.LastTestValue;
	

	Running_status.cartridge = 1000.0 *
				(Cartridge_Detect_Data.Syringe_Full_Vol
				-Cartridge_Detect_Data.Syringe_Empty_Vol);
	
	 Running_status.cartridge_now=Running_status.cartridge*Running_status.cartridge_status/100.0f;
	
	Cartridge_Detect_Data.Dispensing_Finished_Counter
		=Cartridge_Detect_Data.Auto_Detect_Counter;
	System_Power_Level_Detect_Status=POWER_ON;
	
}
void Cartridge_Params_resolving(uint8_t *Params)
{
	switch(Custom_RS232_Con_Div)
	{
		case  LEVEL_DETECT_SYSTEM:
		{
			float __Delay_AT_Step=0.0f;/*跳转等待时间*/
			float __Step_Jump_AN_Accuracy=0.0f;/*跳转控制精度*/
			float __Step_Jump_Condition_Num=0.0f;/*跳转条件判断次数*/
			
			Byte_to_Float(&__Delay_AT_Step,Params,0) ; 
			Byte_to_Float(&__Step_Jump_AN_Accuracy,Params,4) ;
			Byte_to_Float(&__Step_Jump_Condition_Num,Params,8) ;
		break;
		}
		case ROBOT_OR_PLC_SYSTEM:
		{
			
			break;
		}
	}
}
#endif
#if(1==0)
void Start_CalibrationAndDetect(CUSTOM_RS232_CTR_MSG voCmd)
{
	CUSTOM_BEGIN_CMD();	
	CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
	CUSTOM_TX_8(Custom_RS232_Con_Div);
	CUSTOM_TX_8(voCmd);	
	Send_Float(0.00f); 
	CUSTOM_END_CMD();
}

 
#endif

#endif

#if(1==1) //Con. With PC or PLC
void Custom_rs232_Set_Params_Form_PC_PLC(CUSTOM_RS232_CTR_MSG msg,float vfParams)
{
	switch(msg)
	{ 
		  
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_X:    //=0x71, 
		{
			Proparameter[Program_parameter_num-1].Dispensing_speed = vfParams * Running_status.ul_ml;
			SAVE_Program(15);
			break;
		}			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Y:    //=0x72, 
		{
			Proparameter[Program_parameter_num-1].Auto_purge_speed = vfParams * Running_status.ul_ml;
			SAVE_Program(15);
			break;
		}	
			 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Z:    //=0x73, 
		{
			Proparameter[Program_parameter_num-1].Manual_purge_speed = vfParams * Running_status.ul_ml;
			SAVE_Program(15);
			break;
		}	
			 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_CT:    //=0x74, 
		{
			if(vfParams<=MAX_TEM_SET)
			{			
				Temperature_mes[0].Target = vfParams;  
				SAVE_TEM();
			}
			break;
		} 	 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_PT:    //=0x75,
		{
			if(vfParams<=MAX_TEM_SET)
			{	
				Temperature_mes[1].Target = vfParams; 
				SAVE_TEM();
			}
			break;
		}	 
		default:break;
	}
}
void Custom_rs232_Response_To_PC_PLC(CUSTOM_RS232_CTR_MSG msg)
{
	switch(msg)
	{ 
			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_STATUS:    //=0x50, 
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_STATUS);	 	
			 
			Send_Float(Running_status.warn_status); //Status
			Send_Float(Temperature_mes[0].Temperature); //CT
			Send_Float(Temperature_mes[1].Temperature); ////PT
			
			CUSTOM_END_CMD();
			
			break;
		}
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_X:    //=0x51, 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_X:    //=0x71, 
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);	 
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_X);
			 
			Send_Float(Proparameter[Program_parameter_num-1].Dispensing_speed /Running_status.ul_ml); // X Params  
			
			CUSTOM_END_CMD();
			break;
		}
			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Y:    //=0x52, 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Y:    //=0x72, 
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE); 
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Y);	
			 
			Send_Float(Proparameter[Program_parameter_num-1].Auto_purge_speed /Running_status.ul_ml); // Y Params  
			
			CUSTOM_END_CMD();
			break;
		}
			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Z:    //=0x53, 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Z:    //=0x73, 
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Z);	 
			 
			Send_Float(Proparameter[Program_parameter_num-1].Manual_purge_speed /Running_status.ul_ml); // Z Params  
			
			CUSTOM_END_CMD();
			break;
		}
			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_CT:    //=0x54, 
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_CT:    //=0x74, 
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_CT);	 
			 
			Send_Float(Temperature_mes[0].Target ); // CT Params   
			
			CUSTOM_END_CMD();
			break;
		}
			
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_PT:    //=0x55,
		case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_PT:    //=0x75,
		{
			CUSTOM_BEGIN_CMD();	
			CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE); 
			CUSTOM_TX_8(Custom_RS232_Con_Div);
			CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_PT);	
			 
			Send_Float(Temperature_mes[1].Target ); // PT Params  
			
			CUSTOM_END_CMD();
			break;
		}
			 
		default:break;
	}
}
void Custom_rs232_Request_Form_PC_PLC(CUSTOM_RS232_CTR_MSG  msg,uint8_t *Params)
{ 
		switch(msg)
		{ 
			 /*PC OR PLC REQUEST*/ 
			#if (1==1)//REQUEST 请求
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_STATUS:    //=0x50, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_X:    //=0x51, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Y:    //=0x52, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Z:    //=0x53, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_CT:    //=0x54, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_PT:    //=0x55,
			{
				Custom_rs232_Response_To_PC_PLC(msg);
				break;
			}
			#endif
			
			/*PC OR PLC RESPONSE*/  	
			#if (1==1)//RESPONSE 响应
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_STATUS:    //=0x60,
			{
				break;
			}
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_X:    //=0x61,
			{
				break;
			}
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Y:    //=0x62,
			{
				break;
			}
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Z:    //=0x63,
			{
				break;
			}
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_CT:    //=0x64,
			{
				break;
			}
			case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_PT:    //=0x65,
			{
				break;
			}
				
			#endif
			
			
			/*PC OR PLC PARAMS SET*/  
			#if(1==1)  //PARAMS SET
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_X:    //=0x71, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Y:    //=0x72, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Z:    //=0x73, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_CT:    //=0x74, 
			case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_PT:    //=0x75,
			{
				float __NewParams=0.0f;/*设定参数*/  
				Byte_to_Float(&__NewParams,Params,0) ;  
				Custom_rs232_Set_Params_Form_PC_PLC(msg,__NewParams);
				
				Custom_rs232_Response_To_PC_PLC(msg);
				break;
			}
			#endif
			default:break;
		}
}
#endif

#if(CONTROL_SERVER==1) 

void Send_Pressure_An(void)
{
	  Send_Float(Cartridge_Detect_Data.RT_Pressure_Controller_Data_AN);
}
void Send_Pressure_Bar(void)
{
	  Send_Float(Cartridge_Detect_Data.RT_Pressure_Controller_Data);
}
void Send_Percent(void)
{
	 
	  Send_Float(Cartridge_Detect_Data.Cartridge_Detect_Data);
}

void Cartridge_CAL_Empty_Finished ( void)
{
	
	CUSTOM_BEGIN_CMD();	
	CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
	CUSTOM_TX_8(Custom_RS232_Con_Div);
	CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_EMPTY_FINISHED);
	
	Send_Float(0.00f);
	Send_Pressure_An();
	Send_Pressure_Bar();
	CUSTOM_END_CMD();
	
}
void Cartridge_CAL_Full_Finished (void)
{
	
	CUSTOM_BEGIN_CMD();
	CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);	
	CUSTOM_TX_8(Custom_RS232_Con_Div);
	CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_FULL_FINISHED);
	
	Send_Float(100.0f);
	Send_Pressure_An();
	Send_Pressure_Bar();
	CUSTOM_END_CMD();
}
void Cartridge_Detect_Finished (void)
{
	
	CUSTOM_BEGIN_CMD();
	CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);	
	CUSTOM_TX_8(Custom_RS232_Con_Div);
	CUSTOM_TX_8(CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_DETECT_FINISHED);
	Send_Percent();
	Send_Pressure_An();
	Send_Pressure_Bar();
	CUSTOM_END_CMD();
} 


void Cartridge_CalLibrationAndDetect_Finished(uint8_t voMode)
{
	switch(voMode)
	{ 
		case CARTRIDGE_STATUS_EMPTY_CALIBRATION: 
			{ 
				Cartridge_CAL_Empty_Finished();
				break;
			}
		case CARTRIDGE_STATUS_FULL_CALIBRATION: 
			{ 
				Cartridge_CAL_Full_Finished();
				break;
			}
		case CARTRIDGE_STATUS_CARTRIDGE_DETECT:
			{ 
				Cartridge_Detect_Finished();
				break;
			}
		default:
			break;
	}
}

void Update_Status(void)
{ 
	    CUSTOM_BEGIN_CMD();
	
	
	    CUSTOM_TX_8(CUSTOM_RS232_CMD_REQUEST_RESPONSE);
	   CUSTOM_TX_8(Custom_RS232_Con_Div);
	   CUSTOM_RS232_CTR_MSG msg_Type;
	   switch(Cartridge_Detect_Data.Cartridge_Status)
	   {
		   case CARTRIDGE_STATUS_PRODUCTION://0,
		   {
			   msg_Type=CUSTOM_RS232_CTR_MSG_PRODUCT_STSTUS;
			   break;
		   }
			case CARTRIDGE_STATUS_EMPTY_CALIBRATION://1,
		   {
			   msg_Type=CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_EMPTY;
			   break;
		   }
			case CARTRIDGE_STATUS_FULL_CALIBRATION://2,
		   {
			   msg_Type=CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_FULL;
			   break;
		   }
			case CARTRIDGE_STATUS_CARTRIDGE_DETECT://3 
		   {
			   msg_Type=CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT;
			   break;
		   }
	   } 
	    CUSTOM_TX_8(msg_Type);
	   
	   /*1.第1个字节表示阀状态 ,BIT0-进气阀，BIT1-泄气阀，BIT2-出气阀*/ 
	   #if(1==1)
	   uint8_t _VavelStatus=0x00;
	   switch(Cartridge_Detect_Data.Cartridge_Sub_Status)
		{
			/*无:默认动作*/
			case AIR_NONE:
			{   
				_VavelStatus+=1<<0;
				_VavelStatus+=1<<2;
				break;
			}
			/*管路泄气:  关闭阀IN，打开阀OUT，打开泄气阀，直到压力表压力不再下降为达到跳转条件*/
			case AIR_DIS:
			{ 
				_VavelStatus+=1<<1; 
				_VavelStatus+=1<<2; 
				break;
			}
			/*管路充气:  打开阀A，关闭阀B，关闭泄气阀，直到压力表压力不再上升为达到跳转条件*/
			case AIR_INFLATION:
			{   
				_VavelStatus+=1<<0; 
				break;
			}  
			/*管路充气平衡: 关闭阀A,关闭阀B，关闭泄气阀，等待2~3秒，达到跳转条件*/
			case AIR_INFLATION_BALANCE:
			{  
				 
				break;
			}
	
			/*胶桶充气：保持关闭阀A，打开阀B，关闭泄气阀，直到压力表读数不在下降,延时1秒，读取十次数据，求平均值*/
			case AIR_TO_CARTRIDGE:
			{   
				
				_VavelStatus+=1<<2; 
				break;
			}
	
			/*测试完成：打开阀A,阀B。关闭泄气阀，直到压力读数不再上升，完成动作*/
			case AIR_DETECT_COMPLETE:
			{   
				
				_VavelStatus+=1<<0; 
				_VavelStatus+=1<<2; 
				break;
			}
		}
		
	   CUSTOM_TX_8(_VavelStatus);
	   #endif
	   /*2.第2个字节预留 */
	   CUSTOM_TX_8(0x00);
	   #if(1==1)
	   #endif
	   /*3.第3个字节预留 */
	   CUSTOM_TX_8(0x00);
	   #if(1==1)
	   #endif
	   /*4.第4个字节预留 */
	   CUSTOM_TX_8(0x00);
	   #if(1==1)
	   #endif
	   
	  /*5.第5,6,7,8个字节，表示压力传感器数据模拟量数据*/
	   #if(1==1)
		 Send_Pressure_An();
	   #endif
	   
	  /*6.第9,10,11,12个字节，表示压力传感器数据显示数据，单位Bar*/
	   #if(1==1)
		 Send_Pressure_Bar();
	   #endif
	   
	
	    CUSTOM_END_CMD();

}

#endif
	


void Custom_rs232_APP(void)
{
	static uint8_t OutLineCounter=0;
	Custom_rs232_qsize  size = 0; 
	size = Custom_rs232_queue_find_cmd(Custom_rs232_cmd_buffer,CUSTOM_RS232_CMD_MAX_SIZE); //从缓冲区中获取一条指令 

	if(size<=0)
	{
		OutLineCounter++;
		if(OutLineCounter>=0x20 && Custom_Rs232_CMD_HandShake_Status==POWER_ON)
		{
			Custom_Rs232_CMD_HandShake_Status=POWER_OFF;
			OutLineCounter=0;
		}
	}
	else
	{ 
			OutLineCounter=0;
	}
	
	if(size>0&&Custom_rs232_cmd_buffer[1]!=0x07)                                              //接收到指令 ，及判断是否为开机提示if
	{   
		Custom_rs232_ProcessMessage((PCUSTOM_RS232_MSG)Custom_rs232_cmd_buffer, size);                             //指令处理  
		
		#if(CONTROL_SERVER==1)
		{ 
			static uint8_t LoopCounter=0;
			if(Cartridge_Detect_Data.Cartridge_DetectAndCalibration_Finished!=0)
			{
				Cartridge_CalLibrationAndDetect_Finished(Cartridge_Detect_Data.Cartridge_DetectAndCalibration_Finished);
				if(LoopCounter++>=0x10)
				{	
					Cartridge_Detect_Data.Cartridge_DetectAndCalibration_Finished=0;
					LoopCounter=0;
				}
			}
			else
			{ 
				
				Update_Status();
			}
		}
		#else
		{
			switch(Custom_RS232_Con_Div)
			{
				case LEVEL_DETECT_SYSTEM:
				{
					#if(1==0)
						static uint8_t LoopCounter=0;
						switch(SendToServerMessage)
						{
							case CUSTOM_RS232_CTR_MSG_RESSURE_CAL_0BAR: 
							case CUSTOM_RS232_CTR_MSG_RESSURE_CAL_2BAR: 
							case CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_EMPTY: 
							case CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_FULL: 
							case CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT: 
							{ 
								Start_CalibrationAndDetect(SendToServerMessage);
								SendToServerMessage=0;
								break;
							}
							default:
								break;
						}
						 
						if(System_Power_Level_Detect_Status==POWER_ON &&  DIspensingFinished_Mark==POWER_ON)
						{
							
							DIspensingFinished_Mark=POWER_OFF;
							Cartridge_Detect_Data.Dispensing_Finished_Counter--;
							if( Cartridge_Detect_Data.Dispensing_Finished_Counter<=0)
							{
								/*测试开始*/ 
									SendToServerMessage=CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT;
								/*测试开始*/ 
							}
						}
						#endif
						break;
				}
				case ROBOT_OR_PLC_SYSTEM:
				{
					break;
				}
			}
		}
		#endif
	}                                                                           
	else if(size>0&&Custom_rs232_cmd_buffer[1]==0x07)                                         //如果为指令0x07就软重置STM32  
	{                                                                           
//		__disable_fault_irq();                                                   
//		NVIC_SystemReset();                                                                                                                                          
	}  
	  
	 
	if(Level_Detect_Status.Main_Status==CARTRIDGE_STATUS_PRODUCTION)
	{
		PUTIO(9,0);
	}
	else
	{
		
		PUTIO(9,1);
	}
}

void Custom_rs232_ProcessMessage( PCUSTOM_RS232_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//指令类型
	uint8_t ctrl_msg = msg->ctrl_msg;   //消息的类型  
	
	uint8_t Con_div = msg->Con_div;   //消息的类型  
	
	//uint32_t value = CUSTOM_RS232_PTR2U32(msg->param);//数值
	Custom_RS232_Con_Div = Con_div;
	
	switch(cmd_type)
	{	
		case CUSTOM_RS232_CMD_HANDSHAKE 		://01,/*握手*/ 
		{
			#if(CONTROL_SERVER==1)
			{ 
				/*
				1.回复握手信息
				*/
				Custom_rs232_Send_HandShake();
				/*
				2.直接标记握手成功状态
				*/ 
				static uint8_t __HandShakeCounter=0;
				if(__HandShakeCounter++>0x10)
				{
					Custom_Rs232_CMD_HandShake_Status=POWER_ON; 
					__HandShakeCounter=0;
				}
				
			}
			#else
			{
				/*
				2.直接标记握手成功状态
				*/
				 switch(Custom_RS232_Con_Div)
				{
					case LEVEL_DETECT_SYSTEM:
					{
						//Cartridge_Params_resolving(msg->param); 
						break;
					}
					case ROBOT_OR_PLC_SYSTEM:
					{
						break;
					}
				}
				
				Custom_rs232_Send_HandShake(Custom_RS232_Con_Div);
				Custom_Rs232_CMD_HandShake_Status=POWER_ON; 
			}
			#endif
			break;
		}
		 
		case	CUSTOM_RS232_CMD_REQUEST_RESPONSE 		://03,/*通知/应答*/
		{ 
			
			switch(Custom_RS232_Con_Div)
			{
				case LEVEL_DETECT_SYSTEM:
				{
					#if(1==0)
					switch(ctrl_msg)
					{
						/*RESSURE_CAL*/
						#if (1==1) 
						case CUSTOM_RS232_CTR_MSG_RESSURE_CAL_0BAR://1:
						{
							/* 注释 */
							#if (CONTROL_SERVER==1) 
							
							   Air_Gauge_0Bar_Calibration();  
							#endif
							break;
						} 
						case CUSTOM_RS232_CTR_MSG_RESSURE_CAL_2BAR://2:
						{
							/* 注释 */
							#if (CONTROL_SERVER==1) 
							  Air_Gauge_2Bar_Calibration();
							
							#endif
							break;
						} 
						#endif
						
						/*CARTRIDGE CAL AND DETECT*/
						#if(1==1) 
						case CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_EMPTY://:
						{
							/* 注释 */
							#if (CONTROL_SERVER==1) 
							  Cartridge_Detect_Data.Cartridge_Status=CARTRIDGE_STATUS_EMPTY_CALIBRATION; 
							  Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_DIS; 

							#else 
							Product_Ststus_resolving(msg->param);
							Level_Detect_Status.Main_Status= CARTRIDGE_STATUS_EMPTY_CALIBRATION;
							#endif
							break;
						} 
						case CUSTOM_RS232_CTR_MSG_CARTRIDGE_CAL_FULL://:
						{
							/* 注释 */
							#if (CONTROL_SERVER==1) 
							  Cartridge_Detect_Data.Cartridge_Status=CARTRIDGE_STATUS_FULL_CALIBRATION; 
							  Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_DIS; 

							#else 
							Product_Ststus_resolving(msg->param);
							Level_Detect_Status.Main_Status= CARTRIDGE_STATUS_FULL_CALIBRATION;
							#endif
							break;
						} 
						case CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT://:
						{
							/* 注释 */
							#if (CONTROL_SERVER==1) 
							
							  Cartridge_Detect_Data.Cartridge_Status=CARTRIDGE_STATUS_CARTRIDGE_DETECT; 
							  Cartridge_Detect_Data.Cartridge_Sub_Status=AIR_DIS; 
							#else 
							Product_Ststus_resolving(msg->param);
							Level_Detect_Status.Main_Status= CARTRIDGE_STATUS_CARTRIDGE_DETECT;
							#endif
							break;
						} 
						#endif
						/*PARAMS*/
						#if(1==1)
						case CUSTOM_RS232_CTR_MSG_CARTRIDGE_DETECT_PARAMS:
						{
							#if(CONTROL_SERVER==1)
							#endif
							break;
						}
						#endif
						/*STATUS*/
						#if(1==1)
						case CUSTOM_RS232_CTR_MSG_PRODUCT_STSTUS:
						{
							/* 注释 */
							
							#if (CONTROL_SERVER==0)  
							Product_Ststus_resolving(msg->param);
							Level_Detect_Status.Main_Status=CARTRIDGE_STATUS_PRODUCTION;
							#else
							
							#endif
							break;
						}  
						
						case CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_EMPTY_FINISHED:
						{
							/* 注释 */
							#if (CONTROL_SERVER==0)  
							Cartridge_CAL_Empty_Finished(msg->param);
							Level_Detect_Status.Main_Status=CARTRIDGE_STATUS_PRODUCTION;
							
							#endif
							break;
						} 
						case CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_FULL_FINISHED:
						{
							/* 注释 */
							#if (CONTROL_SERVER==0)  
							Cartridge_CAL_Full_Finished(msg->param);
							Level_Detect_Status.Main_Status=CARTRIDGE_STATUS_PRODUCTION;
							
							#endif
							break;
						} 
						case CUSTOM_RS232_CTR_MSG_STSTUS_CARTRIDGE_CAL_DETECT_FINISHED:
						{
							/* 注释 */
							#if (CONTROL_SERVER==0)  
							Cartridge_Detect_Finished(msg->param);
							Level_Detect_Status.Main_Status=CARTRIDGE_STATUS_PRODUCTION;
							#endif
							break;
						} 
						#endif
						break;
					}
					
					#endif
					break;
				}
				case ROBOT_OR_PLC_SYSTEM:
				{
					switch(ctrl_msg)
					{ 
						 /*PC OR PLC REQUEST*/ 
						#if (1==1)//REQUEST 请求
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_STATUS:    //=0x50, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_X:    //=0x51, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Y:    //=0x52, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_Z:    //=0x53, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_CT:    //=0x54, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_PARAMS_PT:    //=0x55, 
						#endif
						
						/*PC OR PLC RESPONSE*/  	
						#if (1==1)//RESPONSE 响应
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_STATUS:    //=0x60, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_X:    //=0x61, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Y:    //=0x62, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_Z:    //=0x63, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_CT:    //=0x64, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_RESPONSE_PARAMS_PT:    //=0x65, 
							
						#endif
						
						
						/*PC OR PLC PARAMS SET*/  
						#if(1==1)  //PARAMS SET
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_X:    //=0x71, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Y:    //=0x72, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_Z:    //=0x73, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_CT:    //=0x74, 
						case CUSTOM_RS232_CTR_MSG_STSTUS_REQUEST_SET_PARAMS_PT:    //=0x75,
						{
							Custom_rs232_Request_Form_PC_PLC(ctrl_msg,msg->param);
							break;
						}
						#endif
					}
					break;
				}
				default: break;
		}
	 }
	default:
		break;
	}
}
 #endif
