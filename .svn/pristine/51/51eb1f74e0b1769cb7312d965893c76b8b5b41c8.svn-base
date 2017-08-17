/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw.h
 * Purpose: USB Hardware Layer Definitions
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
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added USB_ClearEPBuf 
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __USBHW_H__
#define __USBHW_H__

#include "type.h"

/* USB PIN connect,soft connect,D+,D- */
#define USB_DP_PIN	    0x20000000u    /* positive and negative */
#define USB_DN_PIN	    0x40000000u    /* p0.29,p0.30 FCNT2 -- USB_D+,USB_D- */
#define USB_CON_PIN	        0x0200u    /* p2.9 FCNT2 -- USB_CONNECT */
#define USB_VBUS_PIN	0x40000000u	   /* p1.30 FCNT2 -- USB_VBUS */
#define USB_UP_LED_PIN	0x00040000u	   /* p1.18 FCNT2 -- USB_UP_LED */

/* USB RAM Definitions */
#define USB_RAM_ADR     0x7FD00000  /* USB RAM Start Address */
#define USB_RAM_SZ      0x00002000  /* USB RAM Size (8kB) */

/* DMA Endpoint Descriptors */
#define DD_NISO_CNT             16  /* Non-Iso EP DMA Descr. Count (max. 32) */
#define DD_ISO_CNT               8  /* Iso EP DMA Descriptor Count (max. 32) */
#define DD_NISO_SZ    (DD_NISO_CNT * 16)    /* Non-Iso DMA Descr. Size */
#define DD_ISO_SZ     (DD_ISO_CNT  * 20)    /* Iso DMA Descriptor Size */
#define DD_NISO_ADR   (USB_RAM_ADR + 128)   /* Non-Iso DMA Descr. Address */
#define DD_ISO_ADR    (DD_NISO_ADR + DD_NISO_SZ) /* Iso DMA Descr. Address */
#define DD_SZ         (128 + DD_NISO_SZ + DD_ISO_SZ) /* Descr. Size */

/* DMA Buffer Memory Definitions */
#define DMA_BUF_ADR   (USB_RAM_ADR + DD_SZ) /* DMA Buffer Start Address */
#define DMA_BUF_SZ    (USB_RAM_SZ  - DD_SZ) /* DMA Buffer Size */

/* USB Error Codes */
#define USB_ERR_PID         0x0001  /* PID Error */
#define USB_ERR_UEPKT       0x0002  /* Unexpected Packet */
#define USB_ERR_DCRC        0x0004  /* Data CRC Error */
#define USB_ERR_TIMOUT      0x0008  /* Bus Time-out Error */
#define USB_ERR_EOP         0x0010  /* End of Packet Error */
#define USB_ERR_B_OVRN      0x0020  /* Buffer Overrun */
#define USB_ERR_BTSTF       0x0040  /* Bit Stuff Error */
#define USB_ERR_TGL         0x0080  /* Toggle Bit Error */

/* USB DMA Status Codes */
#define USB_DMA_INVALID     0x0000  /* DMA Invalid - Not Configured */
#define USB_DMA_IDLE        0x0001  /* DMA Idle - Waiting for Trigger */
#define USB_DMA_BUSY        0x0002  /* DMA Busy - Transfer in progress */
#define USB_DMA_DONE        0x0003  /* DMA Transfer Done (no Errors)*/
#define USB_DMA_OVER_RUN    0x0004  /* Data Over Run */
#define USB_DMA_UNDER_RUN   0x0005  /* Data Under Run (Short Packet) */
#define USB_DMA_ERROR       0x0006  /* Error */
#define USB_DMA_UNKNOWN     0xFFFF  /* Unknown State */

/* USB DMA Descriptor */
typedef struct _USB_DMA_DESCRIPTOR {
  U32 BufAdr;                       /* DMA Buffer Address */
  U16 BufLen;                       /* DMA Buffer Length */
  U16 MaxSize;                      /* Maximum Packet Size */
  U32 InfoAdr;                      /* Packet Info Memory Address */
  union {                           /* DMA Configuration */
    struct {
      U32 Link   : 1;               /* Link to existing Descriptors */
      U32 IsoEP  : 1;               /* Isonchronous Endpoint */
      U32 ATLE   : 1;               /* ATLE (Auto Transfer Length Extract) */
      U32 Rsrvd  : 5;               /* Reserved */
      U32 LenPos : 8;               /* Length Position (ATLE) */
    } Type;
    U32 Val;
  } Cfg;
} USB_DMA_DESCRIPTOR;

/* USB Hardware Functions */
extern void USB_Init        (void);
extern void USB_Connect     (BOOL con);
extern void USB_Reset       (void);
extern void USB_Suspend     (void);
extern void USB_Resume      (void);
extern void USB_WakeUp      (void);
extern void USB_WakeUpCfg   (BOOL cfg);
extern void USB_SetAddress  (U32  adr);
extern void USB_Configure   (BOOL cfg);
extern void USB_ConfigEP    (USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void USB_DirCtrlEP   (U32  dir);
extern void USB_EnableEP    (U32  EPNum);
extern void USB_DisableEP   (U32  EPNum);
extern void USB_ResetEP     (U32  EPNum);
extern void USB_SetStallEP  (U32  EPNum);
extern void USB_ClrStallEP  (U32  EPNum);
extern void USB_ClearEPBuf  (U32  EPNum);
extern U32  USB_ReadEP      (U32  EPNum, U8 *pData);
extern U32  USB_WriteEP     (U32  EPNum, U8 *pData, U32 cnt);
//extern U32  __swi(8) USB_WriteEP (U32 EPNum, U8 *pData, U32 cnt);
extern BOOL USB_DMA_Setup   (U32  EPNum, USB_DMA_DESCRIPTOR *pDD);
extern void USB_DMA_Enable  (U32  EPNum);
extern void USB_DMA_Disable (U32  EPNum);
extern U32  USB_DMA_Status  (U32  EPNum);
extern U32  USB_DMA_BufAdr  (U32  EPNum);
extern U32  USB_DMA_BufCnt  (U32  EPNum);
extern U32  USB_GetFrame    (void);


//ºó¼Ó12-30
void WrCmdEP (U32 EPNum, U32 cmd);
void USBHwNakIntEnable(U8 bIntBits);

#endif  /* __USBHW_H__ */
