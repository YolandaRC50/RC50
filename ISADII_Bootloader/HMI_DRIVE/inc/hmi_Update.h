#ifndef __HMI_UPDATE_H
#define __HMI_UPDATE_H	

#include "stm32f4xx.h"

int DC_WriteCom(int len,unsigned char *data);

int DC_ReadCom(int len,unsigned char *data);

uint8_t DC_BeginUpdate(void);

uint8_t DC_EndUpdate(void);

uint8_t DC_UpdateImage(char *strPathImageIndex, char *strPathImageBin);

uint8_t DC_UpdateFont(char *strPathFontIndex, char *strPathFontBin);

uint8_t DC_UpdateScreen(char *strPathScreen);

uint8_t DC_SendData(unsigned char *data, int len);

uint8_t DC_UpdateFile(char * strFileName,unsigned char cmdID);

uint8_t CheckHMIUpdateFiles(void);
uint8_t UpdateHMI(void);
#endif