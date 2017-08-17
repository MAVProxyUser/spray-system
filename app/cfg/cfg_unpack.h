/**
  ******************** (C) COPYRIGHT 2012 DJI **********************************
  *
  * @Project Name       : BL_WKM2_LED_IAP.uvproj
  * @File Name          : cfg_unpack.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2012-05-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    Begginning of application   
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_UNPACK_H__
#define __CFG_UNPACK_H__

#define CFG_CMD_MAX_SIZE							(512)

void cfg_unpack_init( void );
void cfg_unpack_tick( void );

/*******************  (C) COPYRIGHT 2012 DJI ************END OF FILE***********/
#endif

