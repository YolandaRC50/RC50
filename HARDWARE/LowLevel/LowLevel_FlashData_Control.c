#include "Lowlevel_FlashData_Control.h"

void Cartridge_Calibration_And_Detect_Finished(void)
{

 
}
void Air_Gauge_Calibration(uint8_t Index)
{
	 uint8_t saveData=0;
	 uint16_t buffer_index=0;
	 switch(Index)
	 {
		 case 0:
		 {
//			uint8_t saveData=1;
//			Cartridge_Detect_Data.Air_Controller_Calibration_0bar=(float)ADC_READ(); 
//			buffer_index=20;
//			Float_to_Byte(Cartridge_Detect_Data.Air_Controller_Calibration_0bar,Params_buffer,buffer_index);
//			 break;
		 }
		 case 2:
		 { 
//			uint8_t saveData=1;
//			Cartridge_Detect_Data.Air_Controller_Calibration_2bar=(float)ADC_READ(); 
//			buffer_index=24;
//			Float_to_Byte(Cartridge_Detect_Data.Air_Controller_Calibration_2bar,Params_buffer,buffer_index);
			 break;
		 }
	 }
	 
	if(saveData==1)
	{
		//WriteFlashData(PARAMS_START_ADDR,Params_buffer,PARAMS_BUFFER_LEN);
	}
}

void Air_Gauge_0Bar_Calibration(void)
{
	Air_Gauge_Calibration(0);
}
void Air_Gauge_2Bar_Calibration(void)
{
	
	Air_Gauge_Calibration(2);
}

