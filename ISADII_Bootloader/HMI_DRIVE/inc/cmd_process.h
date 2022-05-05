#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"

#define NOTIFY_TOUCH_PRESS      0X01   //触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE  0X03  //触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK  0X0C  //写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD  0X0D  //写FLASH失败
#define NOTIFY_READ_FLASH_OK  0X0B  //读FLASH成功
#define NOTIFY_READ_FLASH_FAILD  0X0F  //读FLASH失败
#define NOTIFY_MENU                        0X14  //菜单事件通知
#define NOTIFY_TIMER                       0X43  //定时器超时通知
#define NOTIFY_CONTROL                0XB1  //控件更新通知
#define NOTIFY_READ_RTC               0XF7  //读取RTC时间
#define MSG_GET_CURRENT_SCREEN 0X01//画面ID变化通知
#define MSG_GET_DATA                            0X11//控件数据通知

#define MSG_PROGRAM_UPDATE_RES  0XA5//控件数据通知


#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  //从缓冲区取32位数据

enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //按钮
	kCtrlText,  //文本
	kCtrlProgress,  //进度条
	kCtrlSlider,    //滑动条
	kCtrlMeter,  //仪表
	kCtrlDropList, //下拉列表
	kCtrlAnimation, //动画
	kCtrlRTC, //时间显示
	kCtrlGraph, //曲线图控件
	kCtrlTable, //表格控件
	kCtrlMenu,//菜单控件
	kCtrlSelector,//选择控件
	kCtrlQRCode,//二维码
};

#pragma pack(push)
#pragma pack(1)	//按字节对齐

typedef struct
{
	uint8_t    cmd_head;  //帧头

	uint8_t    cmd_type;  //命令类型(UPDATE_CONTROL)	
	uint8_t    ctrl_msg;   //CtrlMsgType-指示消息的类型
	uint16_t   screen_id;  //产生消息的画面ID
	uint16_t   control_id;  //产生消息的控件ID
	uint8_t    control_type; //控件类型

	uint8_t    param[256];//可变长度参数，最多256个字节

	uint8_t  cmd_tail[4];   //帧尾
}CTRL_MSG,*PCTRL_MSG;

#pragma pack(pop)



#pragma pack(push)
#pragma pack(1)	//按字节对齐
typedef struct
{ 
	uint16_t    Img_Idx;  //命令类型( )	 
	uint8_t      Img_Spare_1;   //
	uint32_t   Img_Start_Addr;  // 
	uint16_t   Img_X_Len;  // 
	uint16_t   Img_Y_Len;  
	uint8_t      Img_Size_1;
	uint8_t      Img_Size_2;
	uint8_t      Img_Size_3;
	uint16_t   Img_Spare_2; //预留  
}HMI_IMG_IDX,*P_HMI_IMG_IDX;

#pragma pack(pop)



#pragma pack(push)
#pragma pack(1)	//按字节对齐
typedef struct
{
	uint8_t    		cmd_head;  //帧头 
	uint8_t    		cmd_Type;  //帧头 
	uint16_t    		Img_Count;  //命令类型( )	
	HMI_IMG_IDX	HMI_IMG_IDX[50];
	uint8_t  		cmd_tail[4];   //帧尾
}IMG_DATA,*P_IMG_DATA;



#pragma pack(pop)

void ProcessMessage( PCTRL_MSG msg, uint16_t size );

void NotifyScreen(uint16_t screen_id);

void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y);

void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state);

void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str);

void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value);

void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value);

void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value);

void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state);

void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item);

void NotifyTimer(uint16_t screen_id, uint16_t control_id);

void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length);

void NotifyWriteFlash(uint8_t status);

void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);

#endif
