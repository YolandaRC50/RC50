#include "Temperature.h"
#include "string.h"
#include "usart.h"
#include "sys_timer.h" 
#include "hmi_APP.h"  
uint8_t StartLoop=0;
uint8_t Receive_buf[256] = {0};
uint8_t Send_buf[200] = {0};
Temperature Temperature_mes[2];
uint8_t TMP_Getloop;
int16_t Temp_Alm_Counter=0;
int16_t WriteValueArr[10]={0};

uint16_t TEMINF_temget(uint8_t vich,uint8_t TMP_Getloop)
{
		
	uint8_t cmdFunction;   //READ,WRITE
	uint16_t StartAddress; //
	uint16_t ValueLenght; //都需要返回值,读写均需要
	
	uint8_t TMP_GetloopIndex; //循环状态数
	TMP_GetloopIndex=TMP_Getloop%20;
  
	StartAddress=address_2_PV;  
	
	cmdFunction=READ;
	ValueLenght=1;
	
	
	  float SetValueArr[10]={0};	
    //以下时初始化读取命令数组,，如果参数,也并初始化写入数组
	//1获取温度
	//间隔一次循环读一次
	switch(TMP_GetloopIndex)
	{
		case 0:
		{ 
			if(StartLoop<0x02)
			{
				StartLoop++;
				StartAddress=address_2_ACTION_OR_CMD;   
				cmdFunction=ACTION_OR_CMD;
				SetValueArr[0]=GOTO_PARAMS_VALUE; 
			}
			else if(StartLoop>=0x02 &&StartLoop<0x04)
			{  
				StartLoop++;
				StartAddress=address_2_ACTION_OR_CMD;   
				cmdFunction=ACTION_OR_CMD;
				SetValueArr[0]=GOTO_START_VALUE;
			}
			else
			{
				
				StartAddress=address_2_PV;  
			}
				break;
		}
		case 1:
		{
			//2.获取目标值
			//如果获取的实际值与目值不相等 ，则需要写入更新
			StartAddress=address_2_Tar;  
			break;
		} 
		case 3:
		{
			//3.获取P
			//如果获取的实际值与目标值不相等 ，则需要写入更新
			StartAddress=address_2_P; 
			ValueLenght=1;  
			break;
		}
		case 5:
		{
			//5.获取I	  
			//如果获取的实际值与目标值不相等 ，则需要写入更新
			StartAddress=address_2_I; 
			ValueLenght=1;  
			break;
		}
		case 7:
		{
			//7.获取D
			//如果获取的实际值与目标值不相等 ，则需要写入更新
			StartAddress=address_2_D; 
			ValueLenght=1;  
			break;
		}
		case 9:
		{
			//4.获取Offset
			//如果获取的实际值与目标值不相等 ，则需要写入更新
			StartAddress=address_2_offset;  	 
			break;
		}
		case 11:
		{
			if(StartLoop<4)
			{
			StartAddress=address_2_INPUT_TYPE;
			}
			else
			{
				
				StartAddress=address_2_PV;  
			}
			break;
		}
		case 13:
		{
			if(StartLoop<4)
			{
			StartAddress=address_2_INPUT_UNIT;
			}
			else
			{
				
				StartAddress=address_2_PV;  
			}
			break;
		}
		case 15:
		{
			if(StartLoop<4)
			{
			StartAddress=address_2_SUB2;
			}
			else
			{
				
				StartAddress=address_2_PV;  
			}
			break;
		}
		case 16:
		{
			if(StartLoop<4)
			{
			StartAddress=address_2_PID_ON_OFF;
			}
			else
			{
				
				StartAddress=address_2_PV;  
			}
			break;
		}
		case 17:
		{ 
			if(Temperature_mes[vich-1].AT==PID_AUTO_VALUE)
			  {
					StartAddress=address_2_ACTION_OR_CMD;   
					cmdFunction=ACTION_OR_CMD;
					SetValueArr[0]=PID_AUTO_VALUE;
			  }
			 break; 
		  } 
		case 19:
		{
			if(Temperature_mes[vich-1].CMW==CMW_VALUE)
			  { 
					StartAddress=address_2_ACTION_OR_CMD;   
					cmdFunction=ACTION_OR_CMD;
					SetValueArr[0]=CMW_VALUE;
			  } 
			break;
		  }
		default:
		{
			
			//获取状态  
			//将获取的状态更新到UI;
			if(TMP_GetloopIndex%6==0)
					StartAddress=address_2_status;   
			else
					StartAddress=address_2_PV;  
			break;
		}
			
	} 
  
	  if(Temperature_mes[vich-1].PID_Write==1)
		  {
			  if( (int)(Temperature_mes[vich-1].P*10)==(int)(Temperature_mes[vich-1].P_Temp *10)
				  &&(int)(Temperature_mes[vich-1].I*10)==(int)(Temperature_mes[vich-1].I_Temp *10)
				  &&(int)(Temperature_mes[vich-1].D*10)==(int)(Temperature_mes[vich-1].D_Temp*10) )
			  {
				  Temperature_mes[vich-1].PID_Write=0;
			  }
				 
		  }
	 
					
	
	SetValue:
	 //以下是向串口发送获取数据命令,并获取反馈数据
		if(TEM_WriteCMD(vich,cmdFunction,StartAddress,ValueLenght,SetValueArr)==1)
		{
			
			delay_ms(10);
			
			uint16_t	ReceiveLen=0;
			uint16_t Receive_buf_len;
			if(cmdFunction == WRITE||cmdFunction == ACTION_OR_CMD)
			{
				ReceiveLen=8;
			}
			else if(cmdFunction==READ)
			{
				ReceiveLen=5+ValueLenght*2;
			}
			else
			{
				return 0;
			}
			 RS485_Receive_Data(Receive_buf,ReceiveLen, &Receive_buf_len); 
			if(ReceiveLen==Receive_buf_len)
			{
					/////////////////////
					///////////////////////
					//////////////////////
					//解析数据
					TMP_ResolveReceiveData(Receive_buf,vich,cmdFunction,StartAddress,ValueLenght);
			}
			delay_ms(20);
			
			if(cmdFunction==WRITE ||cmdFunction==ACTION_OR_CMD ) return 1; 
			
			if(StartAddress==address_2_PV)
				return 1;
		switch(cmdFunction)	
		{ 
				case READ:
				 {
					 
					switch(StartAddress)
					{
						 case  address_2_PV://					0x0000 //温度
						 case  address_2_UP://					0x2105 //报警上限
						 case  address_2_DN://					0x2106 //报警下限 
							 break;
						 case  address_2_Tar://					0x0106 //106 602 r
						 {
							 if((int)(Temperature_mes[vich-1].Target*10)!=(int)(Temperature_mes[vich-1].Target_Temp*10))
							 {
								 SetValueArr[0]=Temperature_mes[vich-1].Target;
								  cmdFunction=WRITE; 
								 goto SetValue;
							 }
							 break;
						 }  
						 case  address_2_status://				0x2001
						 {
							 if((Temperature_mes[vich-1].StatusH>>9 & 1)==1)
							 { 
								 break;
							 }
							 else
							{
									 cmdFunction=ACTION_OR_CMD; 
									StartAddress=address_2_ACTION_OR_CMD;
									SetValueArr[0]=CMW_VALUE;
									 goto SetValue;
							 }
							 break;
						 }
						 case  address_2_P://						0x0A00
						 {
							 if((int)(Temperature_mes[vich-1].P*10)!=(int)(Temperature_mes[vich-1].P_Temp*10) )
							 {
								  if(Temperature_mes[vich-1].PID_Write==1)
								 {
									  SetValueArr[0]=Temperature_mes[vich-1].P;
									 cmdFunction=WRITE; 
									 goto SetValue;
								 } 
							 }
								break; 
						 }
						 case  address_2_I://						0x0A00
						 {
							 if((int)(Temperature_mes[vich-1].I*10)!=(int)(Temperature_mes[vich-1].I_Temp*10) )
							 {
								 if(Temperature_mes[vich-1].PID_Write==1)
								 {
									  SetValueArr[0]=Temperature_mes[vich-1].I;  
									  cmdFunction=WRITE; 
									  goto SetValue;
								 } 
							 }
								break; 
						 }
						 case  address_2_D://						0x0A00
						 {
							 if((int)(Temperature_mes[vich-1].D*10)!=(int)(Temperature_mes[vich-1].D_Temp*10) )
							 {
								 if(Temperature_mes[vich-1].PID_Write==1)
								 {
									  SetValueArr[0]=Temperature_mes[vich-1].D;  
									  cmdFunction=WRITE; 
									  goto SetValue;
						 } 
							 }
								break; 
						 }
						 case  address_2_offset://			0x0746
						 {
							 
							 if((int)(Temperature_mes[vich-1].Offset*10)!=(int)(Temperature_mes[vich-1].Offset_Temp*10))
							 {
								  SetValueArr[0]=Temperature_mes[vich-1].Offset;
								  cmdFunction=WRITE;
								 goto SetValue;
							 }
							 
							 break;
						 } 
						 case address_2_INPUT_TYPE:						
						{ 
							if(Temperature_mes[vich-1].INPUT_TYPE_temp!=INPUT_TYPE_VALUE)
							{ 
								  cmdFunction=WRITE;
								SetValueArr[0]=(int16_t)INPUT_TYPE_VALUE;
								 goto SetValue;
							}
							break;
						}							 
						case address_2_INPUT_UNIT:						
						{ 
							if(Temperature_mes[vich-1].INPUT_UNIT_temp!=INPUT_UNIT_VALUE)
							{ 
								  cmdFunction=WRITE;
								SetValueArr[0]=(int16_t)INPUT_UNIT_VALUE;
								 goto SetValue;
							}
							break;
						}		
						case address_2_SUB2:						
						{ 
							
							if(Temperature_mes[vich-1].SUB2_temp!=SUB2_VALUE)
							{ 
								  cmdFunction=WRITE;
								SetValueArr[0]=(int16_t)SUB2_VALUE;
								 goto SetValue;
							}
							break;
						}		
						case address_2_PID_ON_OFF:						
						{  
								if(Temperature_mes[vich-1].PID_ON_OFF_temp!=PID_ON_OFF_VALUE)
								{ 
									  cmdFunction=WRITE;
									 SetValueArr[0]=(int16_t)PID_ON_OFF_VALUE;
									 goto SetValue;
								}
								
								break;
							}		
					} 
					 break;
				 }
		 }
	}
	return 1;
	
}

/*写入指令*/
uint8_t TEM_WriteCMD(uint8_t vich,uint8_t viFunction,uint16_t StartAddress,uint16_t vlValueLenght,float SetValueArr[])
{
	 
		uint8_t i = 0;
	
  //#if 1  //moedbus_busy ，最多等待200ms，否则返回
		while(moedbus_busy && i<200)
		{ 
				delay_ms(1);
				i++; 
		} 
  //#endif
		 
		if(moedbus_busy == 0)
		{
			moedbus_busy = 1;
			uint8_t SenderLen=0;
			GetSendCMDBuffer(vich, viFunction, StartAddress, vlValueLenght,SetValueArr);
			if(viFunction==READ )  
				SenderLen=8;
			else if(viFunction==WRITE) 
				SenderLen=11;
			else if(viFunction==ACTION_OR_CMD)
				SenderLen=8;
			
			
			RS485_Send_Data(Send_buf, SenderLen);
			while(moedbus_busy && i < 200)
			{
				delay_ms(1);
				i++;
			} 
		}
		
		if(i>=200)
			return 0;
		else
			return 1;
}





//解析接收到的数据
uint8_t TMP_ResolveReceiveData(uint8_t *Receive_buf ,uint8_t vich,uint8_t viFunction,uint16_t StartAddress,uint16_t vlValueLength)
{ 
	switch(viFunction)
	{
		case READ:
		{
				if(((uint8_t)Receive_buf[0])!=vich) return 0;

				if(((uint8_t)Receive_buf[1])!=viFunction) return 0;

				if(((uint16_t)Receive_buf[2])!=(vlValueLength*2)) return 0;
		 
				switch(StartAddress)
				{
					case address_2_PV:
					{
						Temperature_mes[vich-1].Temperature=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4])*10.0/100;
						break;
					}
					case address_2_status:
					{
						//8 byte 
						Temperature_mes[vich-1].StatusH=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						break;
					}
					case address_2_Tar:
					{
						Temperature_mes[vich-1].Target_Temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4])*10.0/100;
						break;
					}
					case address_2_offset:
					{
						Temperature_mes[vich-1].Offset_Temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4])*10.0/100;
						break;
					}
					case address_2_P:
					{
						Temperature_mes[vich-1].P_Temp=((uint16_t)(Receive_buf[3] << 8) | Receive_buf[4])*10.0/100;
						if(Temperature_mes[vich-1].PID_Write!=1
							&&current_screen_id!=SCREEN_TEMSET_M_2
							&&current_screen_id!=SCREEN_SAVE_DIA5
						)
						 {
							 Temperature_mes[vich-1].P=Temperature_mes[vich-1].P_Temp;
						 }
						break;
					} 
					case address_2_I:
					{ 
						Temperature_mes[vich-1].I_Temp=((uint16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						if(Temperature_mes[vich-1].PID_Write!=1
							&&current_screen_id!=SCREEN_TEMSET_M_2
							&&current_screen_id!=SCREEN_SAVE_DIA5)
						 {
							 Temperature_mes[vich-1].I=Temperature_mes[vich-1].I_Temp;
						 }
						break;
					} 
					case address_2_D:
					{ 
						Temperature_mes[vich-1].D_Temp=((uint16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						if(Temperature_mes[vich-1].PID_Write!=1
							&&current_screen_id!=SCREEN_TEMSET_M_2
							&&current_screen_id!=SCREEN_SAVE_DIA5)
						 {
							 Temperature_mes[vich-1].D=Temperature_mes[vich-1].D_Temp;
						 }
						break;
					} 
					case address_2_INPUT_TYPE://			   	0x2C00
					{
						Temperature_mes[vich-1].INPUT_TYPE_temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						break;
					}
					 case address_2_INPUT_UNIT://			   	0x2C01
					{
						Temperature_mes[vich-1].INPUT_UNIT_temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						break;
					}
					case address_2_SUB2://			   				0x2E10
					{
						Temperature_mes[vich-1].SUB2_temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						break;
					} 
					case address_2_PID_ON_OFF:
					{
						Temperature_mes[vich-1].PID_ON_OFF_temp=((int16_t)(Receive_buf[3] << 8) | Receive_buf[4]);
						break;
					}
				} 
				break;
		}
	case WRITE:
	{
		
			if(((uint8_t)Receive_buf[0])!=vich) return 0;

			if(((uint8_t)Receive_buf[1])!=viFunction) return 0;

			if(((uint16_t)(Receive_buf[2] << 8) | Receive_buf[3])!=StartAddress) return 0;
			if(((uint16_t)(Receive_buf[4] << 8) | Receive_buf[5])!=(vlValueLength*2)) return 0;
			break;
	}
	case ACTION_OR_CMD: 
		{
			
			if(((uint8_t)Receive_buf[0])!=vich) return 0;

			if(((uint8_t)Receive_buf[1])!=viFunction) return 0;

			if(((uint16_t)(Receive_buf[2] << 8) | Receive_buf[3])!=StartAddress) return 0;
			
			uint16_t ACTION_OR_CMD_Value=(uint16_t)(Receive_buf[4] << 8) | Receive_buf[5]; 
			 
			switch(ACTION_OR_CMD_Value)
			{
				case CMW_VALUE:
				{ 
					Temperature_mes[vich-1].CMW=0x00;
					break;
				}
				case PID_AUTO_VALUE:
				{ 
					Temperature_mes[vich-1].AT=0x00;
					break; 
				}
			} 
			break;
		} 
		
	}
	return 1;
}


void TEM_TargetSET(uint8_t cmd)
{	
	moedbus_busy = 1;	
	if((cmd == 2 || cmd == 3))	
	{	
		//inf_set(ch2, WRITE, address_2_Tar, (uint16_t)(Temperature_mes[1].Target_Temp * 10));
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
	}
	
	if((cmd == 1 || cmd == 3))
	{
		//inf_set(ch1, WRITE, address_2_Tar, (uint16_t)(Temperature_mes[0].Target_Temp * 10));
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
	}
}

void TEM_PIDSET(uint8_t cmd)
{
	if(cmd == 1 || cmd == 3)
	{
		moedbus_busy = 1;
		//inf_set(ch1, WRITE, address_2_P, Temperature_mes[0].P);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);

		//inf_set(ch1, WRITE, address_2_I, Temperature_mes[0].I);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
		
		//inf_set(ch1, WRITE, address_2_D, Temperature_mes[0].D);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
		
		//inf_set(ch1, WRITE, address_2_offset, Temperature_mes[0].Offset);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
	}
	
	if((cmd == 2 || cmd == 3))	
	{	
		moedbus_busy = 1;
		//inf_set(ch2, WRITE, address_2_P, Temperature_mes[1].P);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);

		//inf_set(ch2, WRITE, address_2_I, Temperature_mes[1].I);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
		
		//inf_set(ch2, WRITE, address_2_D, Temperature_mes[1].D);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
		
		//inf_set(ch2, WRITE, address_2_offset, Temperature_mes[1].Offset);
		RS485_Send_Data(Send_buf, 13);
		delay_ms(50);
	}
}

void buf_clr(uint8_t length, uint8_t data[])
{
	for(uint8_t i = 0; i < length; i++)
	{
		data[i] = 0;
	}
}

//MODEBUS CRC16
uint16_t crc16(uint8_t *ptr,uint8_t len)
{
	unsigned long wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1

	int i=0,j=0;//定义计数
//  uint16_t _CRC;
	for(i=0;i<len;i++)//循环计算每个数据
	{
		wcrc^=*ptr++;//将八位数据与crc寄存器亦或.指针地址增加，指向下个数据
		for(j=0;j<8;j++)//循环计算数据的
		{
			if(wcrc&0X0001)//判断右移出的是不是1，如果是1则与多项式进行异或。
			{
				wcrc=wcrc>>1^0XA001;//先将数据右移一位再与上面的多项式进行异或
			}
			else//如果不是1，则直接移出
			{
				wcrc>>=1;//直接移出
			}
		}
	}
	return wcrc<<8|wcrc>>8;//低八位在前，高八位在后
 
}

void GetSendCMDBuffer(uint8_t ch, uint8_t viFunction,  uint16_t address, uint16_t dataLen,float SetValueArr[])
{
	 uint16_t crc_check = 0;
	
	switch (viFunction)
	{
		case READ:
		{
			Send_buf[0] = ch;
			Send_buf[1] = viFunction;
			Send_buf[2] = address >> 8;
			Send_buf[3] = address & 0xff;
			Send_buf[4] = (dataLen) >> 8;
			Send_buf[5] = (dataLen) & 0xff;
			crc_check=crc16(Send_buf, 6); 
			Send_buf[6] = crc_check >> 8;
			Send_buf[7] = crc_check & 0xff;
		
			break;
		}
		case WRITE:
		{	
			Send_buf[0] = ch;
			Send_buf[1] = viFunction;
			Send_buf[2] = address >> 8;
			Send_buf[3] = address & 0xff;
		
			Send_buf[4] = (dataLen) >> 8;
			Send_buf[5] = (dataLen) & 0xff;
			Send_buf[6] = dataLen*2;
			
//			for(uint8_t iIndex=0;iIndex<dataLen;dataLen++)
//			{
//				 int16_t value16=(SetValueArr[iIndex]*10);
//				 Send_buf[7+iIndex*4+2] = value16 >> 8;
//			   Send_buf[7+iIndex*4+3] = value16 & 0xff;
//			}
			switch(address)
			{
				case address_2_Tar:
				case address_2_offset:
				{ 
					Send_buf[7]=((int16_t)((SetValueArr[0]+0.05)*10)) >> 8;
					Send_buf[8]=((int16_t)((SetValueArr[0]+0.05)*10)) & 0xff; 
					break;
				}
				case address_2_P:
				{
					//P 
					Send_buf[7]=((uint16_t)((SetValueArr[0]+0.05)*10)) >> 8;
					Send_buf[8]=((uint16_t)((SetValueArr[0]+0.05)*10)) & 0xff;  
					break;
				}
				case address_2_I: 
				case address_2_D: 
				{ 
					Send_buf[7]=((uint16_t)(SetValueArr[0]+0.05)) >> 8;
					Send_buf[8]=((uint16_t)(SetValueArr[0]+0.05)) & 0xff; 
					break;
				}
				case address_2_INPUT_TYPE: 
				case address_2_INPUT_UNIT: 
				case address_2_SUB2: 
				case address_2_PID_ON_OFF:
				{ 
					Send_buf[7]=((int16_t)(SetValueArr[0]+0.05))>> 8;
					Send_buf[8]=((int16_t)(SetValueArr[0]+0.05)) & 0xff; 
					break;
				}
			}
			 
			crc_check=crc16(Send_buf, 9); 
			Send_buf[9] = crc_check >> 8;
			Send_buf[10] = crc_check & 0xff;
			break;
		}
		
		case ACTION_OR_CMD:
		{ 
			uint16_t Action_CMD=(uint16_t)(SetValueArr[0]+0.05);
			 
			Send_buf[0] = ch;
			Send_buf[1] = viFunction;
			Send_buf[2] = address >> 8;
			Send_buf[3] = address & 0xff;
			
			switch(Action_CMD)
			{
				case CMW_VALUE: 
				case PID_AUTO_VALUE: 
				case GOTO_PARAMS_VALUE: 
				case GOTO_START_VALUE:
				{
					
					Send_buf[4]=Action_CMD >> 8;
					Send_buf[5]=Action_CMD & 0xff; 
					break;
				}
			}
			
			crc_check=crc16(Send_buf, 6); 
			Send_buf[6] = crc_check >> 8;
			Send_buf[7] = crc_check & 0xff;
			break;
		}
	}
}

void inf_get(void)
{
	uint8_t ch, ack_cmd, length;
	uint16_t crc_check = 0;
	uint16_t tem;
	ch = Receive_buf[0];
	ack_cmd = Receive_buf[1];
	length = Receive_buf[2];
	
	crc_check = crc16(Receive_buf, length + 3);


	if(crc_check == (Receive_buf[length + 3] << 8 | Receive_buf[length + 4]))
	{
		switch (ack_cmd)
		{
			case 0x03:
				tem = ((Receive_buf[3] << 8) | Receive_buf[4]);
				if(ch == ch1)
				{
					Temperature_mes[0].Temperature = (double)tem / 10; 
				}else
				{
					Temperature_mes[1].Temperature = (double)tem / 10; 
				}
				break;
			case 0x04:
				break;
		}				
	}
}

