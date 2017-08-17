/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : WKM2_CAN_LOADER.uvproj
  * @File Name          : can_handler.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-10-14 
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *	           
  */

/* Includes ------------------------------------------------------------------*/
#include "../drivers/drivers.h"
#include "can_inc.h"

/* extern variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t PingBuf[29] __attribute__ ((aligned (4))) = {0x55,0xbb,0x55,0xbb,0x0f,0x00,0x00,0x00,0x00,0x98,0x0f,0x00,0x00,0x00,0x00,0xcd,0xab,0x00,0x00};//主控确认帧
uint8_t tmp_buf[32] __attribute__ ((aligned (4)));

/* public Function -----------------------------------------------------------*/
unsigned short getcrc(unsigned char *ptr, unsigned char len) 
{
	unsigned short crc;
	unsigned char  da;
	unsigned char  calen    = len;
	unsigned char  *calptr  = ptr;
	unsigned char  temp;
	unsigned short crc_ta[16] = 
	{ /* CRC 余式表 */
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	};
	crc=0;
	
	while(calen--!=0) {
		da=(unsigned char)(((crc&0xf000)>>12)&0x000f); /* 暂存 CRC 的高4 位 */

		//da=((unsigned char)(crc/256))/16; /* 暂存 CRC 的高四位 */
		crc<<=4; /* CRC 右移4 位，相当于取CRC 的低12 位）*/
		temp = *calptr;
		crc^=crc_ta[da^(temp>> 4)&0x0f];  //为什么右移之后会变成0xf#.
		/* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，
		然后加上上一次 CRC 的余数 */

		da=(unsigned char)(((crc&0xf000)>>12)&0x000f); /* 暂存 CRC 的高4 位 */

		//da=((unsigned char)(crc/256))/16; /* 暂存 CRC 的高4 位 */
		crc<<=4; /* CRC 右移4 位， 相当于 CRC 的低12 位） */
		temp = *calptr;
		crc^=crc_ta[da^(temp&0x0f)]; /* CRC 的高4 位和本字节的后半字节相加后查表计算
									 CRC，
									 然后再加上上一次 CRC 的余数 */
		calptr++;
	}

	return crc;
}

void send_ping_package(void)
{
	uint8_t TmpBuf[20];
	uint32_t tmp;
	uint16_t crc;

	TmpBuf[0] = 0x0f; 
	TmpBuf[1] = 0x00;
	
	TmpBuf[2] = 0x00; 
	TmpBuf[3] = 0x00;
	TmpBuf[4] = 0x00;
	TmpBuf[5] = 0x98;
	TmpBuf[6] = 0x0f;
	
//	tmp       = LPC_TIM0->TC;			//必须开启定时器0,才可以读它的值不然会出现异常
	tmp       = 0;
	TmpBuf[7] = (uint8_t)tmp;	    
	TmpBuf[8] = (uint8_t)(tmp>>8);
	
//	tmp       = LPC_TIM0->TC;
	tmp       = 0;
	TmpBuf[9] = (uint8_t)tmp; 	    
	TmpBuf[10]= (uint8_t)(tmp>>8);
	
	TmpBuf[11]= 0xcd;
	TmpBuf[12]= 0xab;

	crc       = getcrc(&TmpBuf[0],11);
	TmpBuf[13]= (uint8_t)crc;
	TmpBuf[14]= (uint8_t)(crc>>8);

	
	memcpy(&PingBuf[4],&TmpBuf[0],15);
	
	CAN_SendPackage_View(CONTROLLER,28,&PingBuf[0]);
}



/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

