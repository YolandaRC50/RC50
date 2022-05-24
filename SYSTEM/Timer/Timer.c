#include "timer.h"
uint32_t m_Timer_Counter=0;
void TIMx_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = ARR; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=PSC;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);//初始化TIM3
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);//清除更新中断请求位
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIMx,ENABLE); ////定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIMx_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=9; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIMx_IRQHandler_10MS(void);
//void TIMx_IRQHandler_100MS(void);
//void TIMx_IRQHandler_1S(void);

//定时器3中断服务函数
void TIMx_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)==SET) //溢出中断
	{
		m_Timer_Counter++;
		TIMx_IRQHandler_10MS(); 
		
		if((m_Timer_Counter+1)%100==0)
		{ 
			//TIMx_IRQHandler_100MS();
			TIMx_IRQHandler_1S();
			m_Timer_Counter=0;
		}
		else if((m_Timer_Counter+1)%10==0)
		{ 
			TIMx_IRQHandler_100MS();
		} 
	}
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);  //清除中断标志位
}
void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APBxPeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = ARR; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=PSC;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM3
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清除更新中断请求位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,ENABLE); ////定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=9; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIMx_IRQHandler_10MS(void);
//void TIMx_IRQHandler_100MS(void);
//void TIMx_IRQHandler_1S(void);

//定时器3中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		//m_Timer_Counter++;
		TIM4_IRQHandler_10MS(); 
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}