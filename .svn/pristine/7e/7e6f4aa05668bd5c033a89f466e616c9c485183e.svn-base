/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbcore.h
 * Purpose: USB Core Definitions
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
 *----------------------------------------------------------------------------*/

#ifndef __USBCORE_H__
#define __USBCORE_H__


/* USB Endpoint Data Structure */
typedef struct _USB_EP_DATA {
  U8 *pData;
  U16 Count;
} USB_EP_DATA;

/* USB Core Global Variables */
extern U16 USB_DeviceStatus;
extern U8  USB_DeviceAddress;
extern U8  USB_Configuration;
extern U32 USB_EndPointMask;
extern U32 USB_EndPointHalt;
extern U32 USB_EndPointStall;
extern U8  USB_AltSetting[USB_IF_NUM];

/* USB Endpoint 0 Buffer */
extern U8  EP0Buf[USB_MAX_PACKET0];

/* USB Endpoint 0 Data Info */
extern USB_EP_DATA EP0Data;

/* USB Setup Packet */
extern USB_SETUP_PACKET SetupPacket;

/* USB Core Functions */
extern void USB_ResetCore (void);



#endif  /* __USBCORE_H__ */
