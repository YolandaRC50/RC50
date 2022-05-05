#include "HMI_App.h"
#include "usbh_usr.h" 

uint8_t cmd_buffer[CMD_MAX_SIZE];//指令缓存
static uint16_t current_screen_id = 0;//当前画面ID
/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id)
{
	//TODO: 添加用户代码
	current_screen_id = screen_id;//在工程配置中开启画面切换通知，记录当前画面ID

	if(current_screen_id==4)//
	{
	}
	else if(current_screen_id==9)//
	{
	}
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: 添加用户代码
}

void SetTextValueint32_t(uint16_t screen_id, uint16_t control_id,int32_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%ld",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char *)buffer,"%.1f",value);//把浮点数转换为字符串(保留一位小数)
	SetTextValue(screen_id,control_id,buffer);
}

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	//uint8_t HMI_MAIN_INDEX=2;
	//uint8_t HMI_UPGRAD_INDEX=1;
	//uint8_t HMI_PROGRAS_CONTROL_INDEX=4;
	//uint8_t HMI_ENTRY_BUTTON_INDEX=1;
	//uint8_t HMI_CANCEL_BUTTON_INDEX=2;
	
	//TODO: 添加用户代码
	if(screen_id==HMI_UPGRAD_INDEX)//按钮、图标、动画控制
	{
		if(control_id==HMI_ENTRY_BUTTON_INDEX)//升级按钮
		{
			answer = 1;
		}else if(control_id==HMI_CANCEL_BUTTON_INDEX)//不升级退出
		{
			answer = 2;
		}
	}
}


/*! 
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//指令类型
	uint8_t ctrl_msg = msg->ctrl_msg;   //消息的类型
	uint8_t control_type = msg->control_type;//控件类型
	uint16_t screen_id = PTR2U16(&msg->screen_id);//画面ID
	uint16_t control_id = PTR2U16(&msg->control_id);//控件ID

	switch(cmd_type)
	{		
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					NotifyButton(screen_id,control_id,msg->param[1]);
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}
