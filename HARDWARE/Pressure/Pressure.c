#include "Pressure.h"
#include "Includes.h"
#define N 2000 //每通道采1000次
#define M 1 //为12个通道 

#define BUFFER_LEN 20
//float ADC_S[2] = {0}; 

__IO uint16_t AD_Value[N]; //用来存放ADC转换结果，也是DMA的目标地址

uint16_t AD_Value_Buffer[BUFFER_LEN]={0};

#if(1)
 void Enable_PressureSensor_Auto_Detect(void)
{
				/*NVIC 中断*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef    NVIC_InitStructure;      
		NVIC_InitStructure.NVIC_IRQChannel=DMA_STREAMx_IRQN;     //中断     哪个通道？ 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;    //抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;           //响应(子）优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //使能IBQ通道
		NVIC_Init(&NVIC_InitStructure);                            //初始化
		
		/*传输过半中断*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_HT, DISABLE);
		/*传输完成中断*/
		DMA_ITConfig(DMA_STREAMx, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA_STREAMx, ENABLE);

		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_HTIF0);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TE); 
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_DME);
		DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_FE); 
	
		
}
 
void	ADC_Configuration()
{
		
		GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_DMAx, ENABLE);			//使能DMA2时钟    
	RCC_AHB1PeriphClockCmd(SENSOR_AN_IN_RCCx, ENABLE);			//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(SENSOR_AN_IN_ADCx_RCCx, ENABLE); 			//使能ADC1时钟

	
  //先初始化ADC1通道 IO口
  GPIO_InitStructure.GPIO_Pin = SENSOR_AN_IN_GOIP_PINx_0;			//PC1	和 PC2			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;							//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//不带上下拉
  GPIO_Init(SENSOR_AN_IN_PORTx, &GPIO_InitStructure);										 
      
//    DMA_DeInit(DMA2_Stream0);
//    DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_CHANNELx;                   						//选择Channel_0
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADCx_DR_ADDRESS;							//数据传输的外设首地址，详解见上
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;	 	//自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       							//数据传输方向选择为外设->内存
	DMA_InitStructure.DMA_BufferSize = N*M;                    										//数据项的值和我们定义的数组大小一致		 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//外设非增量模式			
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        							//内存非增量模式		 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据大小为半字	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     		//内存数据大小为半字		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     												//DMA为循环模式	 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;   											//DMA优先级为高		
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;												//这里和官方例程不同
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;							//FIFO相关的设置
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;										//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;						//外设突发单次传输
	DMA_Init(DMA_STREAMx, &DMA_InitStructure);
   
	DMA_Cmd(DMA_STREAMx, ENABLE);	 //使能DMA2_Stream0通道
       




  /* 初始化ADC */
	/* ADC_CommonInit 的参数设置对应的是ADC->CCR */
	/* ADC_Init				的参数设置对应的是ADC1->CR1和ADC1->CR2 */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;											//单个ADC多通道采用独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	//两个采样阶段之间的延迟20个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 			//DMA使能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;										//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);																			//初始化
    
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;												//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;																	//扫描模式    
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;														//连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;												//右对齐    
  ADC_InitStructure.ADC_NbrOfConversion = M;																		//1个转换在规则序列中 也就是只转换规则序列1 
  
	ADC_Init(SENSOR_AN_IN_ADCx, &ADC_InitStructure);
	
  ADC_RegularChannelConfig(SENSOR_AN_IN_ADCx, SENSOR_AN_IN_ADCx_Channelx_0 , 1, ADC_SampleTime_480Cycles);	//设置通道1为第一个转换顺序 
	
	

  /* 使能 ADC1 DMA */
  ADC_DMACmd(SENSOR_AN_IN_ADCx, ENABLE);
	

	/*只要ADC发生数据转换，且DMA=1，便发出DMA请求
	对应的是设置ADC1->CR2寄存器的位9 */
	ADC_DMARequestAfterLastTransferCmd(SENSOR_AN_IN_ADCx, ENABLE);


  /* 开启AD1转换器 */
  ADC_Cmd(SENSOR_AN_IN_ADCx, ENABLE);      


  /* 启动ADC1 */
  ADC_SoftwareStartConv(SENSOR_AN_IN_ADCx);	
		
		 
}

void	DAC_Configuration()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		DAC_InitTypeDef DAC_InitType;
		
		RCC_AHB1PeriphClockCmd(SENSOR_AN_O_RCCx, ENABLE);//使能GPIOA时钟
		RCC_APB1PeriphClockCmd(SENSOR_AN_O_DAC_RCCx, ENABLE);//使能DAC时钟
			 
		GPIO_InitStructure.GPIO_Pin = SENSOR_AN_O_GOIP_PINx_0;/*|SENSOR_AN_O_GOIP_PINx_1;*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
		GPIO_Init(SENSOR_AN_O_PORTx, &GPIO_InitStructure);//初始化

		DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
		DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
		DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
		DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable; //DAC1输出缓存关闭 BOFF1=1
		DAC_Init(SENSOR_AN_DCA_Channel_0,&DAC_InitType);	 //初始化DAC通道1 
		DAC_Cmd(SENSOR_AN_DCA_Channel_0, ENABLE);  //使能DAC通道1 
		SENSOR_AN_DCA_SetChannel0Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
		
}



void filter(void)
{
		float sum = 0;     
		float sum_1 = 0;  
		for(int i=0;i<N;i++) 
		{
				sum+=AD_Value[i];
		}
		sum/=N;
		for(int i=BUFFER_LEN-1;i>0;i--)
		{
			AD_Value_Buffer[i]=AD_Value_Buffer[i-1];
			sum_1+=AD_Value_Buffer[i]; 
		} 
		AD_Value_Buffer[0]=sum;
		sum_1+=sum;
		Sys_RT_Status.RT_AN_Pressure=sum_1/BUFFER_LEN;
		
		Sys_RT_Status.RT_Pressure=(Sys_RT_Status.RT_AN_Pressure-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value)
		/((Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_2_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.Analog_1_Value)
		/(Sys_Params.Pressure_Param.Calibration_RT_Param.RT_2_Value-Sys_Params.Pressure_Param.Calibration_RT_Param.RT_1_Value));
	 	 
}

void DMA_STREAMx_IRQHandler(void)      
{
	
		if(DMA_GetITStatus(DMA_STREAMx,DMA_IT_TCIF0)==SET)
		{ 
			filter();
			DMA_ClearITPendingBit(DMA_STREAMx,DMA_IT_TCIF0);          
		} 
		
}
#endif

void PressureSensor_Init(void)
{
//  RCC_AHB2PeriphClockCmd(SENSOR_AN_IN_ADCx_RCCx, ENABLE);  
//  
	  
	ADC_Configuration();
	
	DAC_Configuration();

}
 
uint16_t _OUT_PUT_AN=0;
float getOutputPercent(void)
{
	float _Max_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value,_Min_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value;
	
	float _DeltaPressure=Sys_Params.Pressure_Param.TargetPreessure;//-Sys_RT_Status.RT_Pressure;
	float _MeasureRange=Sys_Params.Pressure_Param.Calibration_Set_Param.RT_2_Value-Sys_Params.Pressure_Param.Calibration_Set_Param.RT_1_Value+0.01f;
	float _unitPreBar=/*417.0142;*/(_Max_AN-_Min_AN)/(_MeasureRange); 
	
	float _Delta_AN=_Min_AN+((_DeltaPressure)*_unitPreBar);
	
	//	float _Cal_1_AN=909;
	//	float _Cal_2_AN=2273;
	//	
	//	float _RT_1=1.925;
	//	float _RT_2=5.195;
	//	
	//	
	//	if(1==0)
	//	{
	//		
	//	}
//	if(_Delta_AN>=_Min_AN+(9.0*_unitPreBar))
//	{
//		_Delta_AN=_Max_AN;
//	}
	_OUT_PUT_AN=_Delta_AN;
}

void SetPressureSensorTargetValue(void)
{ 
	uint16_t temp=0;
	if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_OFF
		&&Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_OFF)
	{
		if(Sys_RT_Status.System_Run_Mode==POWER_ON)
		{
			getOutputPercent();
			temp=_OUT_PUT_AN;
		}
		else
		{
			_OUT_PUT_AN=0;
		}
	}
	else
	{
		if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_ON && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_OFF)
		{
			_OUT_PUT_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_1_Value;
		}
		else if(Sys_RT_Status.Pressure_Set_Cal_0Bar_Enable==POWER_OFF && Sys_RT_Status.Pressure_Set_Cal_5Bar_Enable==POWER_ON)
		{
			_OUT_PUT_AN=Sys_Params.Pressure_Param.Calibration_Set_Param.Analog_2_Value;
		}
	}
	temp=_OUT_PUT_AN;
	
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值 
}
 