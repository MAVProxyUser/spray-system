/*----------------------------------------------------------------------------
 * Name:    memory.c
 * Purpose: USB Memory Storage Demo
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                             /* LPCxx/LPC24xx definitions */

#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "mscuser.h"

#include "msc_app.h"

#include "..\drivers\drivers.h"

//#pragma arm section zidata = "USB_RAM" 
//U32 mc_cache[128 * (8 + 2)] ;                  /* MC Cache Buffer for Data and FAT Caching. */
//U16 const _MC_CSIZE = 8;
//#pragma arm section zidata

/*------------------------------------------------------------------------------
  Main Program
 *------------------------------------------------------------------------------*/
void msc_app (void) 
{
  	MMCFG mmcfg;
  	INT8U counter;
	USB_Init();                               /* USB Initialization */

  	
	for(counter=20;counter;counter--)
	{
		if (mmc_init()) 
		{
  			mmc_read_config(&mmcfg);
        	MSC_BlockCount = mmcfg.blocknr;
        	USB_Connect(__TRUE); 
			//uart_printf(0,"%d",mmcfg.blocknr);               
  			break;
		}
	}
	if(counter==0)
	{	
	   //sd card init fail
	}	  	 
  	//while(1){}
}
