#ifndef APP_H
#define APP_H
#include "includes.h" 

extern uint8_t  m_WIFI_Connect_Mark;
extern uint8_t m_Wifi_Search_Success;  
extern char m_WIFI_Ecn[5][10];
extern uint8_t m_Get_HMI_RTC_MARK;
extern uint64_t m_Timer_10MS;
extern uint64_t m_Timer_100MS;
extern uint64_t m_Timer_1S; 
void AppTaskCreate(void);
void AppObjCreate (void);
void SysTick_shine_handle(void);
#endif
