#ifndef HMI_APP_H
#define HMI_APP_H


#include "stm32f4xx.h"                  // Device header
#include "includes.h" 
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>

void UI_Init(void);
void UpdateUI(void);

//��������Load_0����������ID
#define  _SCREEN_LOAD_0                                                        0

//��������SoftUpdate_1����������ID
#define  _SCREEN_SOFTUPDATE_1                                                  1

//��������Main_2����������ID
#define  _SCREEN_MAIN_2                                                        2

//��������Setting_3����������ID
#define  _SCREEN_SETTING_3                                                     3

//��������Setting_More_4����������ID
#define  _SCREEN_SETTING_MORE_4                                                4

//��������Systemini_5����������ID
#define  _SCREEN_SYSTEMINI_5                                                   5

//��������Setting_Save_Dlg_6����������ID
#define  _SCREEN_SETTING_SAVE_DLG_6                                            6

//��������Setting_More_Save_Dlg_7����������ID
#define  _SCREEN_SETTING_MORE_SAVE_DLG_7                                       7

//��������Setting_More_Save_Dlg_7����������ID
#define  _SCREEN_PRESSURE_ALM_DLG_9                                       		9

//��������Setting_More_Save_Dlg_7����������ID
#define  _SCREEN_DEBUG_10                                       								10

#define  _SCREEN_DSETTINGCOMM_11																			11


#define  _SCREEN_DSETTINGCOMM_SAVE_DLG_12		                         12

#define  _SCREEN_DSETTINGCOMM_WIFI_SELECT_13																				13

#define  _SCREEN_CHANGE_PWD_14																				14


#define  _SCREEN_INPUT_PWD_15																				15

extern uint8_t cmd_buffer[CMD_MAX_SIZE];//ָ���

extern uint16_t current_screen_id;//��ǰ����ID

extern uint8_t CHx_Manual_Dispensing[];
extern POWER_STATUS Pressure_Display_Enable;
#endif
