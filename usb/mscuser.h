/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    mscuser.h
 * Purpose: Mass Storage Class Custom User Definitions
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

#ifndef __MSCUSER_H__
#define __MSCUSER_H__

//add 2010-09-06

//////////////////////////////////////////////
//虚拟磁盘的一些宏定义
#define Virtual_DATA_SIZE           49152 //96 sectors
#define Virtual_MSC_DiskSize        ( BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE + Virtual_DATA_SIZE ) 
#define Virtual_MSC_BlockSize       512
#define Virtual_MSC_BlockCount      100//(Virtual_MSC_DiskSize / Virtual_MSC_BlockSize)

#define BOOT_SECT_SIZE 	            Virtual_MSC_BlockSize
#define ROOT_DIR_SIZE       		Virtual_MSC_BlockSize
#define FAT_SIZE		    		512

#define BLOCKS_PER_CLUSTER          2

#define DIR_ENTRY       64
//////////////////////////////////////////////

/* Mass Storage Memory Layout */
#define     MSC_BlockSize  512
#define     MSC_BlockGroup 8
extern U32  MSC_BlockCount;
//add 2010-09-08
extern U32  TEM_MSC_BlockCount;

/* Max In/Out Packet Size */
#define MSC_MAX_PACKET  64

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x85
#define MSC_EP_OUT      0x05

/* MSC Requests Callback Functions */
extern BOOL MSC_Reset     (void);
extern BOOL MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);


#endif  /* __MSCUSER_H__ */
