/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.h
 * Purpose: USB Descriptors Definitions
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

#ifndef __USBDESC_H__
#define __USBDESC_H__


#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)

#define USB_DEVICE_DESC_SIZE        (sizeof(USB_DEVICE_DESCRIPTOR))
#define USB_CONFIGUARTION_DESC_SIZE (sizeof(USB_CONFIGURATION_DESCRIPTOR))
#define USB_INTERFACE_DESC_SIZE     (sizeof(USB_INTERFACE_DESCRIPTOR))
#define USB_ENDPOINT_DESC_SIZE      (sizeof(USB_ENDPOINT_DESCRIPTOR))

//add 2010-02-25
#define USB_INTERFACE_ASSOCIATION_DESC_SIZE (sizeof(USB_INTERFACE_ASSOCIATION_DESCRIPTOR))

//modify 2010-12-16
//是否使用符合设备
//#define  __COMPOSITE_DEVICE__

#ifndef __COMPOSITE_DEVICE__  
extern U8 USB_DeviceDescriptor[18]; //default cdc device
extern U8 USB_ConfigDescriptor[76];

extern const U8 USB_DeviceDescriptor_MSC[18];
extern const U8 USB_ConfigDescriptor_MSC[33];
#else  //composite device
extern const U8 USB_DeviceDescriptor[18];
extern const U8 USB_ConfigDescriptor[];
#endif

extern const U8 USB_StringDescriptor[];


#endif  /* __USBDESC_H__ */
