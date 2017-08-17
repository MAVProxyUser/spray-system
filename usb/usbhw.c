/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw.c
 * Purpose: USB Hardware Layer Module for Philips LPC17xx
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added USB_ClearEPBuf
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>                        /* LPC17xx definitions */

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbreg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"

#include "..\drivers\drivers.h"


#pragma diag_suppress 1441


#define EP_MSK_CTRL 0x0001      /* Control Endpoint Logical Address Mask */
#define EP_MSK_BULK 0xC924      /* Bulk Endpoint Logical Address Mask */
#define EP_MSK_INT  0x2492      /* Interrupt Endpoint Logical Address Mask */
//#define EP_MSK_INT  0x4492      /* Interrupt Endpoint Logical Address Mask */
#define EP_MSK_ISO  0x1248      /* Isochronous Endpoint Logical Address Mask */


#if USB_DMA

//#pragma arm section zidata = "USB_RAM"
U32 UDCA[USB_EP_NUM];                       /* UDCA in USB RAM */
U32 DD_NISO_Mem[4*DD_NISO_CNT];             /* Non-Iso DMA Descriptor Memory */
U32 DD_ISO_Mem [5*DD_ISO_CNT];              /* Iso DMA Descriptor Memory */
//#pragma arm section zidata
U32 udca[USB_EP_NUM];                       /* UDCA saved values */

U32 DDMemMap[2];                            /* DMA Descriptor Memory Usage */

#endif


/*
 *  Get Endpoint Physical Address
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    Endpoint Physical Address
 */

U32 EPAdr (U32 EPNum) {
  U32 val;

  val = (EPNum & 0x0F) << 1;
  if (EPNum & 0x80) {
    val += 1;
  }
  return (val);
}


/*
 *  Write Command
 *    Parameters:      cmd:   Command
 *    Return Value:    None
 */

void WrCmd (U32 cmd) {

  LPC_USB->USBDevIntClr = CCEMTY_INT;
  LPC_USB->USBCmdCode = cmd;
  while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
}


/*
 *  Write Command Data
 *    Parameters:      cmd:   Command
 *                     val:   Data
 *    Return Value:    None
 */

void WrCmdDat (U32 cmd, U32 val) {

  LPC_USB->USBDevIntClr = CCEMTY_INT;
  LPC_USB->USBCmdCode = cmd;
  while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
  LPC_USB->USBDevIntClr = CCEMTY_INT;
  LPC_USB->USBCmdCode = val;
  while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
}

/*
** 后加，
*/
void USBHwNakIntEnable(U8 bIntBits)
{
	bIntBits = 0x00000100 | (bIntBits << 16);
	//USBHwCmdWrite(CMD_DEV_SET_MODE, bIntBits);
	WrCmdDat (CMD_SET_MODE, bIntBits);
}

/*
 *  Write Command to Endpoint
 *    Parameters:      cmd:   Command
 *                     val:   Data
 *    Return Value:    None
 */

void WrCmdEP (U32 EPNum, U32 cmd){

  LPC_USB->USBDevIntClr = CCEMTY_INT;
  LPC_USB->USBCmdCode = CMD_SEL_EP(EPAdr(EPNum));
  while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
  LPC_USB->USBDevIntClr = CCEMTY_INT;
  LPC_USB->USBCmdCode = cmd;
  while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
}


/*
 *  Read Command Data
 *    Parameters:      cmd:   Command
 *    Return Value:    Data Value
 */

U32 RdCmdDat (U32 cmd) {

  LPC_USB->USBDevIntClr = CCEMTY_INT | CDFULL_INT;
  LPC_USB->USBCmdCode = cmd;
  while ((LPC_USB->USBDevIntSt & CDFULL_INT) == 0);
  return (LPC_USB->USBCmdData);
}


/*
 *  USB Initialize Function
 *   Called by the User to initialize USB
 *    Return Value:    None
 */
extern void usb_bus_off(void);
extern void usb_bus_on(void);

void USB_Init (void) 
{
	usb_bus_off();

	LPC_SC->PCONP |= ( 1ul << 31 );
	
	LPC_USB->USBClkCtrl = 0x12;               /* Dev, PortSel, AHB clock enable */
	while ((LPC_USB->USBClkSt & 0x12) != 0x12); 

	NVIC_SetPriority( USB_IRQn, 4 ); /* 中断优先级降低防止can接收缓冲区溢出 */
	NVIC_EnableIRQ( USB_IRQn );
	
	/* Partial Manual Reset since Automatic Bus Reset is not working */
	USB_Reset();
	USB_SetAddress(0);

//    USB_Connect(__TRUE);
	usb_bus_on();
}


/*
 *  USB Connect Function
 *   Called by the User to Connect/Disconnect USB
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USB_Connect (BOOL con) {
  WrCmdDat(CMD_SET_DEV_STAT, DAT_WR_BYTE(con ? DEV_CON : 0));
}


/*
 *  USB Reset Function
 *   Called automatically on USB Reset
 *    Return Value:    None
 */

void USB_Reset (void) {
#if USB_DMA
  U32 n;
#endif

  LPC_USB->USBEpInd = 0;
  LPC_USB->USBMaxPSize = USB_MAX_PACKET0;
  LPC_USB->USBEpInd = 1;
  LPC_USB->USBMaxPSize = USB_MAX_PACKET0;
  while ((LPC_USB->USBDevIntSt & EP_RLZED_INT) == 0);

  LPC_USB->USBEpIntClr = 0xFFFFFFFF;
  LPC_USB->USBEpIntEn   = 0xFFFFFFFF ^ USB_DMA_EP;
  LPC_USB->USBDevIntClr = 0xFFFFFFFF;
  LPC_USB->USBDevIntEn  = DEV_STAT_INT    | EP_SLOW_INT    |
                         (USB_SOF_EVENT   ? FRAME_INT : 0) |
                         (USB_ERROR_EVENT ? ERR_INT   : 0);

#if USB_DMA
  UDCA_HEAD   = USB_RAM_ADR;
  DMA_REQ_CLR = 0xFFFFFFFF;
  EP_DMA_DIS  = 0xFFFFFFFF;
  EP_DMA_EN   = USB_DMA_EP;
  EOT_INT_CLR = 0xFFFFFFFF;
  NDD_REQ_INT_CLR = 0xFFFFFFFF;
  SYS_ERR_INT_CLR = 0xFFFFFFFF;
  DMA_INT_EN  = 0x00000007;
  DDMemMap[0] = 0x00000000;
  DDMemMap[1] = 0x00000000;
  for (n = 0; n < USB_EP_NUM; n++) {
    udca[n] = 0;
    UDCA[n] = 0;
  }
#endif
}


/*
 *  USB Suspend Function
 *   Called automatically on USB Suspend
 *    Return Value:    None
 */

void USB_Suspend (void) {
  /* Performed by Hardware */
}


/*
 *  USB Resume Function
 *   Called automatically on USB Resume
 *    Return Value:    None
 */

void USB_Resume (void) {
  /* Performed by Hardware */
}


/*
 *  USB Remote Wakeup Function
 *   Called automatically on USB Remote Wakeup
 *    Return Value:    None
 */

void USB_WakeUp (void) {

  if (USB_DeviceStatus & USB_GETSTATUS_REMOTE_WAKEUP) {
    WrCmdDat(CMD_SET_DEV_STAT, DAT_WR_BYTE(DEV_CON));
  }
}


/*
 *  USB Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Enable/Disable
 *    Return Value:    None
 */

void USB_WakeUpCfg (BOOL cfg) {
  /* Not needed */
}


/*
 *  USB Set Address Function
 *    Parameters:      adr:   USB Address
 *    Return Value:    None
 */

void USB_SetAddress (U32 adr) {
  WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr)); /* Don't wait for next */
  WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr)); /*  Setup Status Phase */
}


/*
 *  USB Configure Function
 *    Parameters:      cfg:   Configure/Deconfigure
 *    Return Value:    None
 */

void USB_Configure (BOOL cfg) {

  WrCmdDat(CMD_CFG_DEV, DAT_WR_BYTE(cfg ? CONF_DVICE : 0));

  LPC_USB->USBReEp = 0x00000003;
  while ((LPC_USB->USBDevIntSt & EP_RLZED_INT) == 0);
  LPC_USB->USBDevIntClr = EP_RLZED_INT;
}


/*
 *  Configure USB Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Endpoint Descriptor
 *    Return Value:    None
 */

void USB_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
  U32 num;

  num = EPAdr(pEPD->bEndpointAddress);
  LPC_USB->USBReEp |= (1 << num);
  LPC_USB->USBEpInd = num;
  LPC_USB->USBMaxPSize = pEPD->wMaxPacketSize;
  while ((LPC_USB->USBDevIntSt & EP_RLZED_INT) == 0);
  LPC_USB->USBDevIntClr = EP_RLZED_INT;
}


/*
 *  Set Direction for USB Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USB_DirCtrlEP (U32 dir) {
  /* Not needed */
}


/*
 *  Enable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_EnableEP (U32 EPNum) {
  WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_DisableEP (U32 EPNum) {
  WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(EP_STAT_DA));
}


/*
 *  Reset USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ResetEP (U32 EPNum) {
  WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}


/*
 *  Set Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_SetStallEP (U32 EPNum) {
  WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(EP_STAT_ST));
}


/*
 *  Clear Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ClrStallEP (U32 EPNum) {
  WrCmdDat(CMD_SET_EP_STAT(EPAdr(EPNum)), DAT_WR_BYTE(0));
}


/*
 *  Clear USB Endpoint Buffer
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ClearEPBuf (U32 EPNum) {
  WrCmdEP(EPNum, CMD_CLR_BUF);
}


/*
 *  Read USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

U32 USB_ReadEP (U32 EPNum, U8 *pData) {
  U32 cnt, n;

  LPC_USB->USBCtrl = ((EPNum & 0x0F) << 2) | CTRL_RD_EN;

  do {
    cnt = LPC_USB->USBRxPLen;
  } while ((cnt & PKT_RDY) == 0);
  cnt &= PKT_LNGTH_MASK;

  for (n = 0; n < (cnt + 3) / 4; n++) {
    *((__packed U32 *)pData) = LPC_USB->USBRxData;
    pData += 4;
  }

  LPC_USB->USBCtrl = 0;

  if (((EP_MSK_ISO >> (EPNum & 0x0F)) & 1) == 0) {   /* Non-Isochronous Endpoint */
    WrCmdEP(EPNum, CMD_CLR_BUF);
  }

  return (cnt);
}


/*
 *  Write USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

//U32 __swi(8) USB_WriteEP (U32 EPNum, U8 *pData, U32 cnt);
//U32 __SWI_8              (U32 EPNum, U8 *pData, U32 cnt) {
U32 USB_WriteEP (U32 EPNum, U8 *pData, U32 cnt) {
  U32 n;

  LPC_USB->USBCtrl = ((EPNum & 0x0F) << 2) | CTRL_WR_EN;

  LPC_USB->USBTxPLen = cnt;

  for (n = 0; n < (cnt + 3) / 4; n++) {
    LPC_USB->USBTxData = *((__packed U32 *)pData);
    pData += 4;
  }

  LPC_USB->USBCtrl = 0;

  WrCmdEP(EPNum, CMD_VALID_BUF);

  return (cnt);
}


#if USB_DMA


/* DMA Descriptor Memory Layout */
const U32 DDAdr[2] = { DD_NISO_ADR, DD_ISO_ADR };
const U32 DDSz [2] = { 16,          20         };


/*
 *  Setup USB DMA Transfer for selected Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                     pDD: Pointer to DMA Descriptor
 *    Return Value:    TRUE - Success, FALSE - Error
 */
BOOL USB_DMA_Setup(U32 EPNum, USB_DMA_DESCRIPTOR *pDD) {
  U32 num, ptr, nxt, iso, n;

  iso = pDD->Cfg.Type.IsoEP;                /* Iso or Non-Iso Descriptor */
  num = EPAdr(EPNum);                       /* Endpoint's Physical Address */

  ptr = 0;                                  /* Current Descriptor */
  nxt = udca[num];                          /* Initial Descriptor */
  while (nxt) {                             /* Go through Descriptor List */
    ptr = nxt;                              /* Current Descriptor */
    if (!pDD->Cfg.Type.Link) {              /* Check for Linked Descriptors */
      n = (ptr - DDAdr[iso]) / DDSz[iso];   /* Descriptor Index */
      DDMemMap[iso] &= ~(1 << n);           /* Unmark Memory Usage */
    }
    nxt = *((U32 *)ptr);                    /* Next Descriptor */
  }

  for (n = 0; n < 32; n++) {                /* Search for available Memory */
    if ((DDMemMap[iso] & (1 << n)) == 0) {
      break;                                /* Memory found */
    }
  }
  if (n == 32) return (__FALSE);            /* Memory not available */

  DDMemMap[iso] |= 1 << n;                  /* Mark Memory Usage */
  nxt = DDAdr[iso] + n * DDSz[iso];         /* Next Descriptor */

  if (ptr && pDD->Cfg.Type.Link) {
    *((U32 *)(ptr + 0))  = nxt;             /* Link in new Descriptor */
    *((U32 *)(ptr + 4)) |= 0x00000004;      /* Next DD is Valid */
  } else {
    udca[num] = nxt;                        /* Save new Descriptor */
    UDCA[num] = nxt;                        /* Update UDCA in USB */
  }

  /* Fill in DMA Descriptor */
  *(((U32 *)nxt)++) =  0;                   /* Next DD Pointer */
  *(((U32 *)nxt)++) =  pDD->Cfg.Type.ATLE |
                       (pDD->Cfg.Type.IsoEP << 4) |
                       (pDD->MaxSize <<  5) |
                       (pDD->BufLen  << 16);
  *(((U32 *)nxt)++) =  pDD->BufAdr;
  *(((U32 *)nxt)++) =  pDD->Cfg.Type.LenPos << 8;
  if (iso) {
    *((U32 *)nxt) =  pDD->InfoAdr;
  }

  return (__TRUE); /* Success */
}


/*
 *  Enable USB DMA Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_DMA_Enable (U32 EPNum) {
  EP_DMA_EN = 1 << EPAdr(EPNum);
}


/*
 *  Disable USB DMA Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_DMA_Disable (U32 EPNum) {
  EP_DMA_DIS = 1 << EPAdr(EPNum);
}


/*
 *  Get USB DMA Endpoint Status
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    DMA Status
 */

U32 USB_DMA_Status (U32 EPNum) {
  U32 ptr, val;

  ptr = UDCA[EPAdr(EPNum)];                 /* Current Descriptor */
  if (ptr == 0) return (USB_DMA_INVALID);

  val = *((U32 *)(ptr + 3*4));              /* Status Information */
  switch ((val >> 1) & 0x0F) {
    case 0x00:                              /* Not serviced */
      return (USB_DMA_IDLE);
    case 0x01:                              /* Being serviced */
      return (USB_DMA_BUSY);
    case 0x02:                              /* Normal Completition */
      return (USB_DMA_DONE);
    case 0x03:                              /* Data Under Run */
      return (USB_DMA_UNDER_RUN);
    case 0x08:                              /* Data Over Run */
      return (USB_DMA_OVER_RUN);
    case 0x09:                              /* System Error */
      return (USB_DMA_ERROR);
  }

  return (USB_DMA_UNKNOWN);
}

/*
 *  Get USB DMA Endpoint Current Buffer Address
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    DMA Address (or -1 when DMA is Invalid)
 */

U32 USB_DMA_BufAdr (U32 EPNum) {
  U32 ptr, val;

  ptr = UDCA[EPAdr(EPNum)];                 /* Current Descriptor */
  if (ptr == 0) return ((U32)(-1));         /* DMA Invalid */

  val = *((U32 *)(ptr + 2*4));              /* Buffer Address */

  return (val);                             /* Current Address */
}


/*
 *  Get USB DMA Endpoint Current Buffer Count
 *   Number of transfered Bytes or Iso Packets
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    DMA Count (or -1 when DMA is Invalid)
 */

U32 USB_DMA_BufCnt (U32 EPNum) {
  U32 ptr, val;

  ptr = UDCA[EPAdr(EPNum)];                 /* Current Descriptor */
  if (ptr == 0) return ((U32)(-1));         /* DMA Invalid */

  val = *((U32 *)(ptr + 3*4));              /* Status Information */

  return (val >> 16);                       /* Current Count */
}
#endif /* USB_DMA */

/*
 *  Get USB Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */
U32 USB_GetFrame (void) {
  U32 val;

  WrCmd(CMD_RD_FRAME);
  val = RdCmdDat(DAT_RD_FRAME);
  val = val | (RdCmdDat(DAT_RD_FRAME) << 8);

  return (val);
}

/*
 *  USB Interrupt Service Routine
 */
//add 2010-03-04
extern uint8_t usb_on_flag;
void USB_IRQHandler (void){
  U32 disr, val, n, m;
  U32 episr, episrCur;
  U32 tmp_disr;

  disr = LPC_USB->USBDevIntSt;                  /* Device Interrupt Status */
  tmp_disr = disr;

  /* Device Status Interrupt (Reset, Connect change, Suspend/Resume) */
  if (disr & DEV_STAT_INT) {
    LPC_USB->USBDevIntClr = DEV_STAT_INT;
    WrCmd(CMD_GET_DEV_STAT);
    val = RdCmdDat(DAT_GET_DEV_STAT);       /* Device Status */
    if (val & DEV_RST) {                    /* Reset */
	  USB_Reset();
	  //add 2010-03-04
	  //usb_on_flag = 1;             		    /* usb on ,enable vcom_printf()*/
	  //LED_On();
#if   USB_RESET_EVENT
      USB_Reset_Event();
#endif
    }
    if (val & DEV_CON_CH) {                 /* Connect change */
#if   USB_POWER_EVENT
      USB_Power_Event(val & DEV_CON);
#endif
    }
    if (val & DEV_SUS_CH) {                 /* Suspend/Resume */
      if (val & DEV_SUS) {                  /* Suspend */
		USB_Suspend();
	   
	    //add 2010-03-04				    
		usb_on_flag = 0;					/* usb off,disable vcom_printf() */

#if     USB_SUSPEND_EVENT
        USB_Suspend_Event();
#endif
      } else {                              /* Resume */
		USB_Resume();
		//add 2010-01-29
		//CDC_Init();
		USB_Connect(1);
	    //delete 2010-08-21
		//usb_on_flag = 1;					/* usb on ,enable vcom_printf()*/

#if     USB_RESUME_EVENT
        USB_Resume_Event();
#endif
      }
    }
    goto isr_end;
  }

#if USB_SOF_EVENT
  /* Start of Frame Interrupt */
  if (disr & FRAME_INT) {
    LPC_USB->USBDevIntClr = FRAME_INT;
    USB_SOF_Event();
  }
#endif

#if USB_ERROR_EVENT
  /* Error Interrupt */
  if (disr & ERR_INT) {
    LPC_USB->USBDevIntClr = ERR_INT;
    WrCmd(CMD_RD_ERR_STAT);
    val = RdCmdDat(DAT_RD_ERR_STAT);
    USB_Error_Event(val);
  }
#endif

  /* Endpoint's Slow Interrupt */
  if (disr & EP_SLOW_INT) {
    episrCur = 0;
    episr    = LPC_USB->USBEpIntSt;
    for (n = 0; n < USB_EP_NUM; n++) {      /* Check All Endpoints */
      if (episr == episrCur) break;         /* break if all EP interrupts handled */
      if (episr & (1 << n)) {
        episrCur |= (1 << n);
        m = n >> 1;
  
        LPC_USB->USBEpIntClr = (1 << n);
        while ((LPC_USB->USBDevIntSt & CDFULL_INT) == 0);
        val = LPC_USB->USBCmdData;
  
        if ((n & 1) == 0) {                 /* OUT Endpoint */
          if (n == 0) {                     /* Control OUT Endpoint */
            if (val & EP_SEL_STP) {         /* Setup Packet */
              if (USB_P_EP[0]) {
                USB_P_EP[0](USB_EVT_SETUP);
				//delete 2010-08-09
				//usb_on_flag = 1;
                
				continue;
              }
            }
          }
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_OUT);
          }
        } else {                            /* IN Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_IN);
          }
        }
      }
    }
    LPC_USB->USBDevIntClr = EP_SLOW_INT;
  }

  LPC_USB->USBDevIntClr = tmp_disr;	/* clear all interrupt */

#if USB_DMA
  if (DMA_INT_STAT & 0x00000001) {          /* End of Transfer Interrupt */
    val = EOT_INT_STAT;
    for (n = 2; n < USB_EP_NUM; n++) {      /* Check All Endpoints */
      if (val & (1 << n)) {
        m = n >> 1;
        if ((n & 1) == 0) {                 /* OUT Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_OUT_DMA_EOT);
          }
        } else {                            /* IN Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_IN_DMA_EOT);
          }
        }
      }
    }
    EOT_INT_CLR = val;
  }

  if (DMA_INT_STAT & 0x00000002) {          /* New DD Request Interrupt */
    val = NDD_REQ_INT_STAT;
    for (n = 2; n < USB_EP_NUM; n++) {      /* Check All Endpoints */
      if (val & (1 << n)) {
        m = n >> 1;
        if ((n & 1) == 0) {                 /* OUT Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_OUT_DMA_NDR);
          }
        } else {                            /* IN Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_IN_DMA_NDR);
          }
        }
      }
    }
    NDD_REQ_INT_CLR = val;
  }

  if (DMA_INT_STAT & 0x00000004) {          /* System Error Interrupt */
    val = SYS_ERR_INT_STAT;
    for (n = 2; n < USB_EP_NUM; n++) {      /* Check All Endpoints */
      if (val & (1 << n)) {
        m = n >> 1;
        if ((n & 1) == 0) {                 /* OUT Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_OUT_DMA_ERR);
          }
        } else {                            /* IN Endpoint */
          if (USB_P_EP[m]) {
            USB_P_EP[m](USB_EVT_IN_DMA_ERR);
          }
        }
      }
    }
    SYS_ERR_INT_CLR = val;
  }
#endif /* USB_DMA */

isr_end:
  ;
}

