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
** buffer for vcom
*/
/* Buffer masks */
#define CDC_BUF_SIZE               (512)               /* Output buffer in bytes (power 2) */
                                                       /* large enough for file transfer */
#define CDC_BUF_MASK               (CDC_BUF_SIZE-1ul)

/* Buffer read / write macros */
#define CDC_BUF_RESET(cdcBuf)      (cdcBuf.rdIdx = cdcBuf.wrIdx = 0)
#define CDC_BUF_WR(cdcBuf, dataIn) (cdcBuf.data[CDC_BUF_MASK & cdcBuf.wrIdx++] = (dataIn))
#define CDC_BUF_RD(cdcBuf)         (cdcBuf.data[CDC_BUF_MASK & cdcBuf.rdIdx++])   
#define CDC_BUF_EMPTY(cdcBuf)      (cdcBuf.rdIdx == cdcBuf.wrIdx)
#define CDC_BUF_FULL(cdcBuf)       ((CDC_BUF_MASK & cdcBuf.rdIdx) == (CDC_BUF_MASK & (cdcBuf.wrIdx+1)))
#define CDC_BUF_COUNT(cdcBuf)      (CDC_BUF_MASK & (cdcBuf.wrIdx - cdcBuf.rdIdx))


/* CDC output buffer */
typedef struct __CDC_BUF_T {
  unsigned int wrIdx;
  unsigned int rdIdx;
  unsigned char data[CDC_BUF_SIZE];
} CDC_BUF_T;

extern CDC_BUF_T  CDC_OutBuf,CDC_InBuf;              /* buffer for all CDC Out data */


void BulkIn_Read (char *buffer,int length);

/*
********************************************************************************
*                        END
********************************************************************************
*/
