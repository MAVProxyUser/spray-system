/*
********************************************************************************
**                       DJI xp40 code
** project: xp40.Uv2                        file: vcom_app.c
** environment: keil mdk3.7/lpc2368/72M cclock
** history:
**   v1.0 2009-11-30   Motorfeng
********************************************************************************
*/
#include <LPC17xx.h>                        /* LPC17xx definitions */
#include <string.h>

#include "type.h"

#include "usbreg.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

#include "vcom_app.h"
#include "vcom_buf.h"

#include "../drivers/drivers.h"

extern unsigned char BulkBufIn  [USB_CDC_BUFSIZE];            /* Buffer to store USB IN  packet */
extern unsigned char BulkBufOut [USB_CDC_BUFSIZE];            /* Buffer to store USB OUT packet */

/*
** Initialises the VCOM port.
** Call this function before using VCOM_putchar or VCOM_getchar
*/
void VCOM_Init(void) 
{
  	CDC_Init ();
    
	/* 将USB的初始化也封装起来 */
	USB_Init();                      /* USB Initialization */
  	USB_Connect(1);                  /* USB Connect */
}

/*
** VCOM_putc(char c)    
*/
void VCOM_putc(char c)
{
	while(!CDC_DepInEmpty){}
	CDC_DepInEmpty = 0;
	USB_WriteEP (CDC_DEP_IN, (unsigned char *)&c, 1);
}

/*
** VCOM_SendByte(char c)    
*/
void VCOM_SendByte(char c)
{
	unsigned int numAvailByte;
	CPU_SR_ALLOC();
	
	if(!CDC_BUF_FULL(CDC_InBuf)) 
	{
	    CDC_BUF_WR(CDC_InBuf, c);
	}

	numAvailByte = CDC_BUF_COUNT(CDC_InBuf);

	if(numAvailByte >= USB_CDC_BUFSIZE)
	{
		numAvailByte = USB_CDC_BUFSIZE;		
	}  
	
	CPU_CRITICAL_ENTER();		
	if(CDC_DepInEmpty)	   //	 while(!CDC_DepInEmpty){}
	{
		CDC_DepInEmpty = 0;
		CDC_RdInBuf ((char*)&BulkBufIn[0],(const int*)&numAvailByte);
		USB_WriteEP (CDC_DEP_IN, &BulkBufIn[0], numAvailByte);
	}
	CPU_CRITICAL_EXIT();   
}

void SendStr(const char *str)
{
	unsigned int length;
	length = strlen(str);
	VCOM_sendpackage(str,length);
}

/*
 * VCOM_sendpackage(const char* buffer,int length)    
 */
//add 2010-03-04
extern unsigned char  usb_on_flag;
void VCOM_sendpackage(const char* buffer,int length)
{
    unsigned int numAvailByte;
	CPU_SR_ALLOC();	
	
	//add 2010-03-04
	if(!usb_on_flag) 
	{
	    return;     /* usb off,return */ 
	}
		
	//uart_printf(0,"usb on flag on.\r\n");
	/* write to cdc in buffer */
	if(!CDC_WrInBuf(buffer, (int*)&length))
	{
		/* buffer is full */	
	}

	numAvailByte = CDC_BUF_COUNT(CDC_InBuf);

	if(numAvailByte >= USB_CDC_BUFSIZE)
	{
		numAvailByte = USB_CDC_BUFSIZE;		
	}  
	
	//uart_printf(0,"CDC_DepInEmpty: %d\r\n",CDC_DepInEmpty);
	/* write data from CDC_InBuf to BulkIn[] */
	CPU_CRITICAL_ENTER();

	if(CDC_DepInEmpty)	      // while(!CDC_DepInEmpty){}
	{	
		CDC_DepInEmpty = 0;
	    CDC_RdInBuf ((char*)&BulkBufIn[0],(const int*)&numAvailByte);
		USB_WriteEP (CDC_DEP_IN, &BulkBufIn[0], numAvailByte);		
	}

	CPU_CRITICAL_EXIT();
}

/*
** VCOM_printf()    
*/
//static INT8U vcom_buf_printf[128] __attribute__ ((section("USB_RAM"),aligned(8)));;
static uint8_t vcom_buf_printf[128] __attribute__ ((aligned(8)));
void VCOM_printf(const uint8_t *pstr, ...)    
{
    va_list arglist;
    uint8_t *fp; 
	
	if(!usb_on_flag) return;     /* usb off,return */
	  
    va_start(arglist, pstr);
    vsprintf((char  * )vcom_buf_printf, (char*)pstr, arglist);
    va_end(arglist);
    fp = vcom_buf_printf;    

    SendStr((const char*)fp);
}

void VCOM_printf_debug(const uint8_t *pstr, ...)
{
//    va_list arglist;
//    INT8U *fp; 
//	
//	if(!usb_on_flag) return;     /* usb off,return */
//	
//	temp_buff[0] = 0x55;
//	temp_buff[1] = 0xaa;
//	temp_buff[2] = 0x55;
//	temp_buff[3] = 0xaa;
//	temp_buff[4] = 0x03;	  
//
//    va_start(arglist, pstr);
//    vsprintf((char  * )&vcom_buf_printf[5], (char*)pstr, arglist);
//    va_end(arglist);
//    fp = vcom_buf_printf;    
//
//    SendStr((const char*)fp);	
}

/*
**  VCOM_getc()  
*/
char VCOM_getc(void)
{
    while(!CDC_BUF_COUNT(CDC_OutBuf)){}
	return CDC_BUF_RD(CDC_OutBuf);	
}

/*
** VCOM_GetByte()
** no wait 
*/
unsigned char VCOM_GetByte(void)
{
	return CDC_BUF_RD(CDC_OutBuf);
}

/*
** OutButAvailBytes(void)
** Bytes can be read
*/
short OutBufAvailBytes(void)
{
	return CDC_BUF_COUNT(CDC_OutBuf);
}

void VCOM_Clear(void)
{
	CDC_BUF_RESET(CDC_OutBuf);
}

/*
********************************************************************************
*                        END
********************************************************************************
*/

