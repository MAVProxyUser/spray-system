/*
********************************************************************************
**                       DJI xp40 code
** project: xp40.Uv2                        file: usb_app.h
** environment: keil mdk3.7/lpc2368/72M cclock
** history:
**   v1.0 2009-11-06   Motorfeng
********************************************************************************
*/
#ifndef __VCOM_APP_H
#define	__VCOM_APP_H

void VCOM_Init(void);
void VCOM_Test(void);
void VCOM_putc(char c);
void VCOM_sendpackage(const char* buffer,int length);
char VCOM_getc(void);
void VCOM_SendByte(char c);
void SendStr(const char *str);
void VCOM_printf(const unsigned char *pstr, ...);
void VCOM_printf_debug(const unsigned char *pstr, ...);
unsigned char VCOM_GetByte(void);
short OutBufAvailBytes(void);
void VCOM_Clear(void);
#endif
/*
********************************************************************************
*                        END
********************************************************************************
*/

