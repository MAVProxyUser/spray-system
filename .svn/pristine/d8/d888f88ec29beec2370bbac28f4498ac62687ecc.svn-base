/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Descriptors
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
 *          V1.20 Changed string descriptor handling
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbdesc.h"

#include "cdc.h"
#include "msc.h"

//是否使用符合设备
//#define  __COMPOSITE_DEVICE__

#ifndef __COMPOSITE_DEVICE__ 
//不使用复合设备的情况
//modify 2010-12-16
//RAM descriptor
//default for cdc device
//////////////////////////////////////////
//for cdc device
/* USB Standard Device Descriptor for CDC */
U8 USB_DeviceDescriptor[18] = {
    USB_DEVICE_DESC_SIZE,              /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
    USB_DEVICE_CLASS_COMMUNICATIONS,   /* bDeviceClass CDC*/
    0x00,                              /* bDeviceSubClass */
    0x00,                              /* bDeviceProtocol */
    USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
    WBVAL(0xfff0),                     /* idVendor */
    WBVAL(0x0008),                     /* idProduct */
    WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
    //modify 全部改为1
    0x01,                              /* iManufacturer */
    0x02,                              /* iProduct */
    0x03,                              /* iSerialNumber */
    0x01                               /* bNumConfigurations: one possible configuration*/
};

/* USB Configuration Descriptor for CDC */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor) */
U8 USB_ConfigDescriptor[76] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(                             /* wTotalLength */
        1*USB_CONFIGUARTION_DESC_SIZE +
        1*USB_INTERFACE_DESC_SIZE     +  /* communication interface */
        0x0013                        +  /* CDC functions */
        1*USB_ENDPOINT_DESC_SIZE      +  /* interrupt endpoint */
        1*USB_INTERFACE_DESC_SIZE     +  /* data interface */
        2*USB_ENDPOINT_DESC_SIZE      +  /* bulk endpoints 2 */
	
	    //IAD 后加，xp sp3之后才支持
	    USB_INTERFACE_ASSOCIATION_DESC_SIZE
	  ),
  
    0x02,                              /* bNumInterfaces */
    0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
    0x00,                              /* iConfiguration: no string to describe this configuration */
    USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes */
    /*USB_CONFIG_REMOTE_WAKEUP*/,
    USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */

    /* CDC IAD for multi  */
    USB_INTERFACE_ASSOCIATION_DESC_SIZE,		  /*Size of this descriptor in bytes */
    USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,   /*Descriptor type (<USBGenericDescriptor_IAD>)*/	
    0x00,										  /*Interface number of the first interface that is associated with this function */
    0x02,										  /*Number of contiguous interfaces that are associated with this function */
    CDC_COMMUNICATION_INTERFACE_CLASS,		  /*Class code (assigned by USB-IF)	*/
    CDC_ABSTRACT_CONTROL_MODEL,				  /*Subclass code (assigned by USB-IF) */
    0x00,										  /*Protocol code (assigned by USB-IF) */
    0x04,										  /*Index of string descriptor describing this function*/

    /* Interface 0, Alternate Setting 0, cdc Class */
    USB_INTERFACE_DESC_SIZE,           /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    USB_CDC_CIF_NUM,                   /* bInterfaceNumber: the Number of Interface */
    0x00,                              /* bAlternateSetting: Alternate setting */
    0x01,                              /* bNumEndpoints: One endpoint used */
    CDC_COMMUNICATION_INTERFACE_CLASS, /* bInterfaceClass: Communication Interface Class */
    CDC_ABSTRACT_CONTROL_MODEL,        /* bInterfaceSubClass: Abstract Control Model */
    0x00,                              /* bInterfaceProtocol: no protocol used */
    0x04,                              /* iInterface: */
    /* Header Functional Descriptor */
    0x05,                              /* bLength: Endpoint Descriptor size */
    CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
    CDC_HEADER,                        /* bDescriptorSubtype: Header Func Desc */
    WBVAL(CDC_V1_10), /* 1.10 */       /* bcdCDC */
	/* Call Management Functional Descriptor */
    0x05,                              /* bFunctionLength */
    CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
    CDC_CALL_MANAGEMENT,               /* bDescriptorSubtype: Call Management Func Desc */
    0x01,                              /* bmCapabilities: device handles call management */
    0x01,                              /* bDataInterface: CDC data IF ID */
	/* Abstract Control Management Functional Descriptor */
    0x04,                              /* bFunctionLength */
    CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
    CDC_ABSTRACT_CONTROL_MANAGEMENT,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,                              /* bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported */
	/* Union Functional Descriptor */
    0x05,                              /* bFunctionLength */
    CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
    CDC_UNION,                         /* bDescriptorSubtype: Union func desc */
    USB_CDC_CIF_NUM,                   /* bMasterInterface: Communication class interface is master */
    USB_CDC_DIF_NUM,                   /* bSlaveInterface0: Data class interface is slave 0 */
    /* Endpoint, EP1 Interrupt IN */   /* event notification (optional) */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_ENDPOINT_IN(1),                /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
    WBVAL(0x0010),                     /* wMaxPacketSize */
    0x02,          /* 2ms */           /* bInterval */

    /* Interface 1, Alternate Setting 0, Data class interface descriptor*/
    USB_INTERFACE_DESC_SIZE,           /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    USB_CDC_DIF_NUM,                   /* bInterfaceNumber: Number of Interface */
    0x00,                              /* bAlternateSetting: no alternate setting */
    0x02,                              /* bNumEndpoints: two endpoints used */
    CDC_DATA_INTERFACE_CLASS,          /* bInterfaceClass: Data Interface Class */
    0x00,                              /* bInterfaceSubClass: no subclass available */
    0x00,                              /* bInterfaceProtocol: no protocol used */
    0x04,                              /* iInterface */

    /* Endpoint, EP2 Bulk OUT */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_ENDPOINT_OUT(2),               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
    WBVAL(USB_CDC_BUFSIZE),            /* wMaxPacketSize */
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /* Endpoint, EP2 Bulk IN */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_ENDPOINT_IN(2),                /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
    WBVAL(USB_CDC_BUFSIZE),            /* wMaxPacketSize */
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /* Terminator */
    0                                  /* bLength */
};



///////////////////////////////////////////
// for msc device
/* USB Standard Device Descriptor for MSC*/
const U8 USB_DeviceDescriptor_MSC[18] = {
    USB_DEVICE_DESC_SIZE,              /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
    0x00,                              /* bDeviceClass */
    0x00,                              /* bDeviceSubClass */
    0x00,                              /* bDeviceProtocol */
    USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
    WBVAL(0xfff0),                     /* idVendor */
    WBVAL(0x0008),                     /* idProduct */
    WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
    0x01,                              /* iManufacturer */
    0x02,                              /* iProduct */
    0x06,                              /* iSerialNumber */
    0x01                               /* bNumConfigurations: one possible configuration*/
};

/* USB Configuration Descriptor for MSC*/
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor) */
const U8 USB_ConfigDescriptor_MSC[33] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(                             /* wTotalLength */
        1*USB_CONFIGUARTION_DESC_SIZE +
        1*USB_INTERFACE_DESC_SIZE     +
        2*USB_ENDPOINT_DESC_SIZE
    ),
    0x01,                              /* bNumInterfaces */
    0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
    0x00,                              /* iConfiguration: no string to describe this configuration */
    USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes */
    /*USB_CONFIG_REMOTE_WAKEUP*/,
    USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */

    /* Interface 0, Alternate Setting 0, MSC Class */
    USB_INTERFACE_DESC_SIZE,           /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    //接口序号,索引
    0x00,                              /* bInterfaceNumber */
    0x00,                              /* bAlternateSetting */
    0x02,                              /* bNumEndpoints */
    USB_DEVICE_CLASS_STORAGE,          /* bInterfaceClass */
    MSC_SUBCLASS_SCSI,                 /* bInterfaceSubClass */
    MSC_PROTOCOL_BULK_ONLY,            /* bInterfaceProtocol */
    0x05,                              /* iInterface */

    /* Endpoint, EP5 Bulk IN */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_ENDPOINT_IN(5),                /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
    WBVAL(0x0040),                     /* wMaxPacketSize */
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /* Endpoint, EP5 Bulk OUT */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    USB_ENDPOINT_OUT(5),               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
    WBVAL(0x0040),                     /* wMaxPacketSize */
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /* Terminator */
    0                                  /* bLength */
};



#else
////////////////////////////////////////////
//for composite device
/* USB Standard Device Descriptor */
const U8 USB_DeviceDescriptor[18] = {
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  
  //USB_DEVICE_CLASS_COMMUNICATIONS,   /* bDeviceClass CDC*/
  //0x00,                              /* bDeviceSubClass */
  //0x00,                              /* bDeviceProtocol */
  
  0xEF,// MI
  0x02,//
  0x01,//

  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  WBVAL(0xfff0),                     /* idVendor */
  WBVAL(0x0008),                     /* idProduct */
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
  0x01,                              /* iManufacturer */
  0x02,                              /* iProduct */
  0x03,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations: one possible configuration*/
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor) */
const U8 USB_ConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +  /* communication interface */
    0x0013                        +  /* CDC functions */
    1*USB_ENDPOINT_DESC_SIZE      +  /* interrupt endpoint */
    1*USB_INTERFACE_DESC_SIZE     +  /* data interface */
    2*USB_ENDPOINT_DESC_SIZE      +  /* bulk endpoints 2 */
	
	//IAD
	USB_INTERFACE_ASSOCIATION_DESC_SIZE	+
	//88 新加
	1*USB_INTERFACE_DESC_SIZE	  +	 /* msc interface */
    2*USB_ENDPOINT_DESC_SIZE   		 /* bulk endpoints 5 */	  
	  ),
  
  0x03,                              /* bNumInterfaces */
  
  0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
  0x00,                              /* iConfiguration: no string to describe this configuration */
  USB_CONFIG_BUS_POWERED /*|*/       /* bmAttributes */
/*USB_CONFIG_REMOTE_WAKEUP*/,
  USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */

/* CDC IAD for multi  */
  USB_INTERFACE_ASSOCIATION_DESC_SIZE,		  /*Size of this descriptor in bytes */
  USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,  /*Descriptor type (<USBGenericDescriptor_IAD>)*/	
  0x00,										  /*Interface number of the first interface that is associated with this function */
  0x02,										  /*Number of contiguous interfaces that are associated with this function */
  CDC_COMMUNICATION_INTERFACE_CLASS,		  /*Class code (assigned by USB-IF)	*/
  CDC_ABSTRACT_CONTROL_MODEL,				  /*Subclass code (assigned by USB-IF) */
  0x00,										  /*Protocol code (assigned by USB-IF) */
  0x04,										  /*Index of string descriptor describing this function*/

/* Interface 0, Alternate Setting 0, cdc Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  USB_CDC_CIF_NUM,                   /* bInterfaceNumber: the Number of Interface */
  0x00,                              /* bAlternateSetting: Alternate setting */
  0x01,                              /* bNumEndpoints: One endpoint used */
  CDC_COMMUNICATION_INTERFACE_CLASS, /* bInterfaceClass: Communication Interface Class */
  CDC_ABSTRACT_CONTROL_MODEL,        /* bInterfaceSubClass: Abstract Control Model */
  0x00,                              /* bInterfaceProtocol: no protocol used */
  0x04,                              /* iInterface: */
/* Header Functional Descriptor */
  0x05,                              /* bLength: Endpoint Descriptor size */
  CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
  CDC_HEADER,                        /* bDescriptorSubtype: Header Func Desc */
  WBVAL(CDC_V1_10), /* 1.10 */       /* bcdCDC */
/* Call Management Functional Descriptor */
  0x05,                              /* bFunctionLength */
  CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
  CDC_CALL_MANAGEMENT,               /* bDescriptorSubtype: Call Management Func Desc */
  0x01,                              /* bmCapabilities: device handles call management */
  0x01,                              /* bDataInterface: CDC data IF ID */
/* Abstract Control Management Functional Descriptor */
  0x04,                              /* bFunctionLength */
  CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
  CDC_ABSTRACT_CONTROL_MANAGEMENT,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,                              /* bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported */
/* Union Functional Descriptor */
  0x05,                              /* bFunctionLength */
  CDC_CS_INTERFACE,                  /* bDescriptorType: CS_INTERFACE */
  CDC_UNION,                         /* bDescriptorSubtype: Union func desc */
  USB_CDC_CIF_NUM,                   /* bMasterInterface: Communication class interface is master */
  USB_CDC_DIF_NUM,                   /* bSlaveInterface0: Data class interface is slave 0 */

/* Endpoint, EP1 Interrupt IN */     /* event notification (optional) */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(1),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
  WBVAL(0x0010),                     /* wMaxPacketSize */
  0x02,          /* 2ms */           /* bInterval */

/* Interface 1, Alternate Setting 0, Data class interface descriptor*/
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  USB_CDC_DIF_NUM,                   /* bInterfaceNumber: Number of Interface */
  0x00,                              /* bAlternateSetting: no alternate setting */
  0x02,                              /* bNumEndpoints: two endpoints used */
  CDC_DATA_INTERFACE_CLASS,          /* bInterfaceClass: Data Interface Class */
  0x00,                              /* bInterfaceSubClass: no subclass available */
  0x00,                              /* bInterfaceProtocol: no protocol used */
  0x04,                              /* iInterface */

/* Endpoint, EP2 Bulk OUT */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_OUT(2),               /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(USB_CDC_BUFSIZE),            /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/* Endpoint, EP2 Bulk IN */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(2),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(USB_CDC_BUFSIZE),            /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/*
后加msc 配置相关的东西
*/
/* Interface 2, Alternate Setting 0, MSC Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  //接口序号,索引
  0x02,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x02,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_STORAGE,          /* bInterfaceClass */
  MSC_SUBCLASS_SCSI,                 /* bInterfaceSubClass */
  MSC_PROTOCOL_BULK_ONLY,            /* bInterfaceProtocol */
  0x05,                              /* iInterface */

/* Endpoint, EP5 Bulk IN */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_IN(5),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(0x0040),                     /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/* Endpoint, EP5 Bulk OUT */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  USB_ENDPOINT_OUT(5),               /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(0x0040),                     /* wMaxPacketSize */
  0x00,                              /* bInterval: ignore for Bulk transfer */

/* Terminator */
  0                                  /* bLength */
};

#endif

/* USB String Descriptor (optional) */
const U8 USB_StringDescriptor[] = {
    /* Index 0x00: LANGID Codes */
    0x04,                              /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0409), /* US English */    /* wLANGID */
    /* Index 0x01: Manufacturer */
    (13*2 + 2),                        /* bLength (13 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'D',0,
    'J',0,
    'I',0,
    ' ',0,
    ' ',0,
    'C',0,
    'O',0,
    'N',0,
    'F',0,
    'I',0,
    'F',0,
    ' ',0,
    ' ',0,
    /* Index 0x02: Product */
    (17*2 + 2),                        /* bLength (17 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'D',0,
    'J',0,
    'I',0,
    ' ',0,
    ' ',0,
    ' ',0,
    ' ',0,
    'C',0,
    'O',0,
    'N',0,
    'T',0,
    'R',0,
    'O',0,
    'L',0,
    'L',0,
    'E',0,
    'R',0,
    /* Index 0x03: Serial Number */
    (12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    '0',0,                             /* allowed characters are       */
    '0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
    '0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
    '1',0,                             /*   length >= 26               */
    'A',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    /* Index 0x04: Interface 0, Alternate Setting 0 */
    ( 4*2 + 2),                        /* bLength (4 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'V',0,
    'C',0,
    'O',0,
    'M',0,
    /* Index 0x05: Interface 2, Alternate Setting 0 */
    ( 4*2 + 2),                        /* bLength (4 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'M',0,
    'S',0,
    'C',0,
    'M',0,
    /* Index 0x06: Serial Number */
    (12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    '0',0,                             /* allowed characters are       */
    '0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
    '0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
    '1',0,                             /*   length >= 26               */
    'A',0,
    'A',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,
};

