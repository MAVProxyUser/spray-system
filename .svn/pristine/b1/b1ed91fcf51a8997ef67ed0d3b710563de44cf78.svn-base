/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       : BL_WKM2_LED_IAP.uvproj
  * @File Name          : lpc17xx_delay.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-04-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	           
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* Private variables ---------------------------------------------------------*/
#define US_COUNT_NUM    4u
#define MS_COUNT_NUM    4156u
 
/* Function Prototypes -------------------------------------------------------*/
void delay_nus(CPU_INT32U usCount)
{
    CPU_INT32U usNUM;
	CPU_INT32U i;
	for(i=0; i<usCount; i++)
	{
	  	usNUM = US_COUNT_NUM;
		while(usNUM--)
		{;}
	}
}

void delay_nms(CPU_INT32U msCount)
{
	CPU_INT32U msNUM;
	CPU_INT32U i;
	for(i=0; i<msCount; i++)
	{
	  	msNUM = MS_COUNT_NUM;
		while(msNUM--)
		{;}
	}
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/

