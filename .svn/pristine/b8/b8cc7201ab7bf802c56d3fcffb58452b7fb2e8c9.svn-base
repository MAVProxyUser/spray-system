/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : vcom_buf.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-02-16 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx usb composite device application        
  */

/*
********************************************************************************
**                       DJI xp40 code
** project: xp40.Uv2                        file: vcom_buf.c
** environment: keil mdk3.7/lpc2368/72M cclock
** history:
**   v1.0 2009-11-30   Motorfeng
********************************************************************************
*/
#include "vcom_buf.h"

//#pragma arm section zidata = "USB_RAM"
CDC_BUF_T  CDC_OutBuf,CDC_InBuf;                  /* buffer for all CDC Out data */
//#pragma arm section
/*
**  read data from vcom buffer
*/
void BulkIn_Read (char *buffer,int length) 
{
  while (length--) {
    while (CDC_BUF_EMPTY(CDC_InBuf));                     /* Block until data is available if none */
    *buffer++ = CDC_BUF_RD(CDC_InBuf);
  }
}

/*
**  write data from vcom buffer
*/
#if 0
int BulkOut_Write (const char *buffer, int *length) {
  int  bytesToWrite, bytesWritten;

  /* Write *length bytes */
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;

  while (!SER_BUF_EMPTY(ser_out));                     /* Block until space is available if none */
  while (bytesToWrite) {
      SER_BUF_WR(ser_out, *buffer++);                  /* Read Rx FIFO to buffer */  
      bytesToWrite--;
  }     

  if (ser_txRestart) {
    ser_txRestart = 0;
    U1THR = SER_BUF_RD(ser_out);                       /* Write to the Tx Register */
  }

  return (bytesWritten); 
}
#endif 

/*
********************************************************************************
*                        END
********************************************************************************
*/

