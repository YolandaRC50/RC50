#include "hmi_Update.h"

uint8_t __SendeData[2050]={0};

#ifdef _WIN32
#include "hmidriver.h"
#endif

#include <stdio.h>
//#include <io.h>
//#include <fcntl.h>
#include <string.h>

int DC_WriteCom(int len,unsigned char *data)
{	
#ifdef _WIN32
	return WritePort(data,len);
#else
#endif
}

int DC_ReadCom(int len,unsigned char *data)
{
#ifdef _WIN32
	//等待应答，最多1秒，如果超时认为出错
	int tick = GetTickCount();
	while(GetTickCount()-tick<1000)
	{
		if(ReadPort(data,len)==len)
			return len;
	}
	return 0;
#else
#endif
}


uint8_t DC_BeginUpdate(void)
{
	unsigned char cmd[] = {0xEE,0xF0,0x04,0xBF,0xFF,0xFC,0xFF,0xFF};

	if(!DC_WriteCom(sizeof(cmd),cmd))
		return 0;

	unsigned char ack = 0;
	return (DC_ReadCom(1,&ack)==1&&ack==0xA5);
}

uint8_t DC_EndUpdate(void)
{
	unsigned char cmd = 0xF1;

	if(!DC_WriteCom(1,&cmd))
		return 0;

	unsigned char ack = 0;
	return (DC_ReadCom(1,&ack)==1&&ack==0xA5);
}

uint8_t DC_UpdateImage(char *strPathImageIndex, char *strPathImageBin)
{			
	if(DC_UpdateFile(strPathImageIndex,0x90)&&DC_UpdateFile(strPathImageBin,0x92))
	{					
		return 1;
	}

	return 0;
}

uint8_t DC_UpdateFont(char *strPathFontIndex, char *strPathFontBin)
{
	if(DC_UpdateFile(strPathFontIndex,0x93)&&DC_UpdateFile(strPathFontBin,0x91))
	{					
		return 1;
	}

	return 0;
}

uint8_t DC_UpdateScreen(char *strPathScreen)
{
	return DC_UpdateFile(strPathScreen,0x97);
}

uint8_t DC_SendData(unsigned char *data, int len)
{
	if(!DC_WriteCom(len,data))
		return 0;

	unsigned char ack = 0;
	if((DC_ReadCom(1,&ack)&&ack==0xA5))
	{
		return 1;
	}

#if 1
	printf("DC_SendData no ack!!!\n");
#endif

	return 0;
}

int DC_GetFileSize(char * strFileName)
{
	int fd;
	int len = -1;

	//打开文件(只读)
	fd = open(strFileName, O_RDWR);

	//判断打开是否成功
	if(fd == -1)
	{
		//DEBUG("Can't open file %s\n", fileName);
		return -1;
	}

	// 获取文件大小
	len = lseek(fd, 0L, SEEK_END);       //跳到文件尾, 并读取文件大小

	//关闭文件
	close(fd);

	return len;
}

int DC_ReadFile(char * fileName, char * data, int length, int pos)
{
	int fd;
	int len = - 1;

	//打开文件
	fd = open(fileName, O_RDONLY|O_BINARY);

	//判断打开是否成功
	if(fd == -1)
	{
		//DEBUG("Can't open file %s\n", fileName);
		return -1;
	}

	//跳回读取位置
	lseek(fd, pos, SEEK_SET);

	//读取数据
	len = read(fd, data, length);

	//关闭文件
	close(fd);

#if 1
	if(len<=0)
		printf("DC_ReadFile error!!!\n");
#endif

	return len;
}

uint8_t DC_UpdateFile(char * strFileName,unsigned char cmdID)
{
	const int PACKET_SIZE = 2048;
	unsigned char sendBuffer[PACKET_SIZE+2];

	int nFileSize = DC_GetFileSize(strFileName);
	//空文件直接跳过
	if(nFileSize<=0)
		return 1;

	if(!DC_SendData(&cmdID,1))
		return 0;

	int nByteToRead = 0;
	int nByteReadCnt = 0;
	while (nByteReadCnt<nFileSize)
	{	
		memset(sendBuffer,0,sizeof(sendBuffer));

		nByteToRead = PACKET_SIZE;
		if(nByteReadCnt+nByteToRead>nFileSize)
			nByteToRead = nFileSize-nByteReadCnt;

		if(DC_ReadFile(strFileName,(char *)sendBuffer+2,nByteToRead,nByteReadCnt)<=0)
			return 0;

		*(short *)sendBuffer = nByteToRead;

		if(!DC_SendData(sendBuffer,PACKET_SIZE+2))
			return 0;

		nByteReadCnt += nByteToRead;

#if 1
		printf("update %s %d%%...\r",strFileName,nByteReadCnt*100/nFileSize);
#endif
	}

	//确保最后发送的是空包，用来表示结束
	if ((nByteReadCnt%PACKET_SIZE)==0)
	{
		memset(sendBuffer,0,sizeof(sendBuffer));
		if(!DC_SendData(sendBuffer,PACKET_SIZE+2))
			return 0;
	}

#if 1
	printf("\r\n");
#endif

	return 1;
}

uint8_t CheckHMIUpdateFiles(void)
{
	
}
uint8_t UpdateHMI(void)
{
	if(CheckHMIUpdateFiles())
		return 0;
	
	if(!DC_BeginUpdate())
		return 0;
	
	///////////////
	/////////////////
	////////////////
	///////////////
	
	
	if(DC_EndUpdate())
	{
		return 1;
	}
}
