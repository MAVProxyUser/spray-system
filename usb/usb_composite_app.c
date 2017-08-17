/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       : xp60.uvproj
  * @File Name          : usb_composite_app.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-02-16 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx usb composite device application        
  */

/* Includes ------------------------------------------------------------------*/
#include <LPC17xx.H>                        /* LPC17xx definitions */
#include <string.h>

#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "mscuser.h"
#include "usb_composite_app.h"
#include "usbdesc.h"

//for cdc interface
#include "cdc.h"
#include "cdcuser.h"

#include "../drivers/drivers.h"


//add 2010-03-04 sdtask.c
//extern volatile unsigned char read_card_flag;    /* extern variables */ 
extern unsigned char SDInitState;	             //获得SD卡初始化状态
extern const unsigned char RootDirEntry[DIR_ENTRY];
extern unsigned int TEM_MSC_BlockCount;
extern unsigned char var_USB_MSC_IF_NUM;         /* 切换VCOM和MSC设备时用到 */

/* vcom on flag */
unsigned char usb_on_flag = 0;


/* public function declaration -----------------------------------------------*/
void usb_bus_off(void)
{ 
	PINSEL1 &= ~( 0xF << 26 );
	LPC_GPIO0->FIODIR |= ( 3 << 29 );
	LPC_GPIO0->FIOCLR |= ( 3 << 29 );
}

void usb_bus_on(void)
{ 
	PINSEL1 |= ( 0x5 << 26 );
}

//add 2010-12-16 
void USB_Init_CDC(void)
{
	CDC_Init ();
	//USB stack 使用的一些变量
	//CDC设备不用MSC的东西，所以初始化为零
	var_USB_MSC_IF_NUM = 0;
	//默认的描述就是CDC的,所以描述符不用初始化
	USB_Init();
}

void USB_Init_MSC(void)
{
   	//MSC设备只用到一个接口，排号从0开始
	var_USB_MSC_IF_NUM = 0;
	memcpy(&USB_DeviceDescriptor[0],&USB_DeviceDescriptor_MSC[0],sizeof(USB_DeviceDescriptor_MSC));
	memcpy(&USB_ConfigDescriptor[0],&USB_ConfigDescriptor_MSC[0],sizeof(USB_ConfigDescriptor_MSC));
	USB_Init();
}

/*
 *  init usb composite device,cdc and msc
 */
void usb_composite_init(void)
{
//	read_card_flag = 0;           //enable sd card
//	usb_on_flag    = 0;			  //disable vcom_printf()
//
//	//modify 2010-09-06
//	if(0 == SDInitState)
//	{
//		if(msc_init())
//		{
//			uart_printf( 0, "msc_init ok!\r\n" );
//		}else
//		{
//			TEM_MSC_BlockCount = 0;
//			SDInitState        = 0xff;
//		}
//	}
//	//没有SD卡,或者SD初始化出错，或者获取磁盘大小失败
//	//没有使能SD卡读
//	//虚拟一个磁盘
//	if( (0 != SDInitState) || (1 != read_card_flag) )
//	{
//		MSC_BlockCount = Virtual_MSC_BlockCount;
//		 
//	}else
//	{
//		MSC_BlockCount = TEM_MSC_BlockCount;
//	}
//
//	uart_printf( 0, "usb composite device init ok!" );
//
//	//only cdc device
//    USB_Init_CDC();
}

unsigned char msc_init(void)
{
 	/* msc init */	
//	unsigned int blocknr;
//	if(disk_ioctl(0, GET_SECTOR_COUNT, &blocknr) == RES_OK)
//	{
//		TEM_MSC_BlockCount = blocknr;
//		uart_printf( 0, "TEM_MSC_BlockCount:%d\r\n", blocknr );
//		return 1;
//	}
	return 0; 
}


/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
