/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : WKM2_CAN_LOADER.uvproj
  * @File Name          : can_handler.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-10-14 
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *	           
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_HANDLER_H__
#define __CAN_HANDLER_H__

/* public function declaration -----------------------------------------------*/
void send_ping_package(void);
void osd_cmd_get_start_alt_status(void);
void osd_cmd_get_start_point_status(void);

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif

