/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    mscuser.c
 * Purpose: Mass Storage Class Custom User Module
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
 *          V1.20 Added SCSI_READ12, SCSI_WRITE12
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include <absacc.h>

//#include <File_Config.h>

//delete 2010-08-22
#include "type.h"
#include "usb_composite_app.h"

#include "usb.h"		  //in the usb folder
#include "msc.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "mscuser.h"

/*与SD卡共用缓冲区*/
//extern U8 SDWrBuf2[];	 /* 写入缓冲区安排在系统RAM中 */

//#pragma arm section zidata = "USB_RAM" 
//U8 BlockBuf[4096] __attribute__ ((aligned(4))) ;       //4096byte 8*512         
//#pragma arm section zidata
/* 与SD卡共用缓冲区,这里要注意 */
//U8 *BlockBuf = SDWrBuf2;       //4096byte 8*512         

BOOL MemOK;                                /* Memory OK */

//add 2010-09-08
U32  TEM_MSC_BlockCount;


U32  MSC_BlockCount;
U32  Block;                                /* R/W Block  */
U32  Offset;                               /* R/W Offset */
U32  Length;                               /* R/W Length */

U8   BulkStage;                            /* Bulk Stage */

U8   BulkBuf[MSC_MAX_PACKET];              /* Bulk In/Out Buffer */
U8   BulkLen;                              /* Bulk In/Out Length */

MSC_CBW CBW;                               /* Command Block Wrapper */
MSC_CSW CSW;                               /* Command Status Wrapper */


//extern volatile unsigned int ffs_process_flag;

//extern unsigned char SDInitState;

//extern volatile unsigned char read_card_flag;
//add 2010-09-06

//////////////////////////////////////////////Ace One Log


const unsigned char BootSect[] = {
0xEB,0x3C,0x90,0x4D,0x53,0x44,0x4F,0x53,0x35,0x2E,0x30,0x00,0x02,BLOCKS_PER_CLUSTER,0x01,0x00,
0x01,0x10,0x00,0xEC,0x03,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,0x00,0x00,
};

const unsigned char RootDirEntry[DIR_ENTRY] = {
 'D', 'J', 'I', '-', 'I','O', 'S', 'D', 'L', 'O', 'G',0x28,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 'R', 'E', 'A', 'D', 'M', 'E', ' ', ' ',  'T', 'X', 'T',0x20,0x18,0xbc,0x41,0x97,
0x37,0x38,0x37,0x38,0x00,0x00,0x3d,0x6e,0x2b,0x38,0x02,0x00,0x00,0x02,0x00,0x00,
}; 

//const unsigned char RootDirEntry[DIR_ENTRY] = {
//'D', 'J', 'I', '-', 'L', 'O', 'G',' ', ' ', ' ',' ',0x28,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
// 'R', 'E', 'A', 'D','M', 'E', ' ', ' ', 'T', 'X', 'T',0x20,0x18,0xbc,0x41,0x97,
//0x37,0x38,0x37,0x38,0x00,0x00,0x3d,0x6e,0x2b,0x38,0x02,0x00,0x00,0x02,0x00,0x00,
// };

//const unsigned char Virtual_Context1[1024] = { 
//"[Ace One] working with [Ace Assistant] software now.\r\n\r\n"
//"If you want to read the flight log files,\r\n" 
//"please click [TOOL] -> [Read Flight-Log] in [Ace Assistant] software.\r\n"
//"Then,the flight log files will be display on here, and [Ace Assistant] will be closed automaticly.\r\n\r\n"
//"If you want to use [Ace Assistant] software again,\r\n"
//"please power cycle your [Ace One],\r\n"
//"then launch [Ace Assistant] software.\r\n\r\n"
//"Note: After reading this document, be sure to close it!!! thank you!"
//};
//const unsigned char Virtual_Context1[64] = { 'W','a','r','n','i','n','g',':','\r','\n','(','1',')',' ','s','d',' ','c','a','r','d',' ','d','a','t','a',' ',
//'m','u','s','t',' ','b','e',' ','r','e','a','d',' ','w','i','t','h',' ','A','C','E',' ','A','s','s','i','s','t','a','n','t','!',};
//const unsigned char Virtual_Context2[256] = { 'W','a','r','n','i','n','g',':','\r','\n','(','1',')',' ','N','o',' ','s','d',' ','c','a','r','d',' ','!',};
const unsigned char Virtual_Context2[1024] = { " Warning:\r\n (1) No sd card !\r\n" 
};
unsigned char  Virtual_FatTable[5]= {
0xF8,0xFF,0xFF,0xFF,0x0F,
};

void Virtual_Disk_Read( U8 *buff,U32 offset, U32 length);
void Virtual_Disk_Write( U8 *buff,U32 offset, U32 length);
//////////////////////////////////////////////

/*
 *  Set Stall for MSC Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void MSC_SetStallEP (U32 EPNum) {          /* set EP halt status according stall status */
  USB_SetStallEP(EPNum);
  USB_EndPointHalt  |=  (EPNum & 0x80) ? ((1 << 16) << (EPNum & 0x0F)) : (1 << EPNum);
}


/*
 *  MSC Mass Storage Reset Request Callback
 *   Called automatically on Mass Storage Reset Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL MSC_Reset (void) {

  //LED_Off (LED_RD | LED_WR);               /* Turn Off R/W LED */

  USB_EndPointStall = 0x00000000;          /* EP must stay stalled */
  CSW.dSignature = 0;                      /* invalid signature */

  BulkStage = MSC_BS_CBW;

  return (__TRUE);
}


/*
 *  MSC Get Max LUN Request Callback
 *   Called automatically on Get Max LUN Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL MSC_GetMaxLUN (void) {
  EP0Buf[0] = 0;                           /* No LUN associated with this device */
  return (__TRUE);
}

/*
 *  MSC Memory Read Callback
 *   Called automatically on Memory Read Event
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */
void MSC_MemoryRead (void) {
  U32 n, m;
  //add 2010-09-04
  //modify 2010-09-08
  //if(ffs_process_flag != 0) return;

  if (Block >= MSC_BlockCount) {
    n = 0;
    BulkStage = MSC_BS_DATA_IN_LAST_STALL;
  } else {
    if (Length > MSC_MAX_PACKET) {
      n = MSC_MAX_PACKET;
    } else {
      n = Length;
    }
  }

  if ((Offset == 0) && (n != 0)) {
    m = (Length + (MSC_BlockSize-1)) / MSC_BlockSize;
    if (m > MSC_BlockGroup) {
      m = MSC_BlockGroup;
    }
    //modify 2010-09-06
//	if( (0 == SDInitState) && (1 == read_card_flag ) ) //sd卡初始化正常,并且读SD卡已经使能
//	{
//	 	mmc_read_sect(0,BlockBuf,Block, m);
//	}else
//	{
//		//uart_printf(1,"1!\r\n");
//		Virtual_Disk_Read( BlockBuf,Block, m);
//
//	}
  }

//  USB_WriteEP(MSC_EP_IN, &BlockBuf[Offset], n);
  Offset += n;
  Length -= n;

  if (Offset == MSC_BlockGroup*MSC_BlockSize) {
    Offset = 0;
    Block += MSC_BlockGroup;
  }

  CSW.dDataResidue -= n;

  if (Length == 0) {
    BulkStage = MSC_BS_DATA_IN_LAST;
  }

  if (BulkStage != MSC_BS_DATA_IN) {
    //LED_Off (LED_RD);            /* Turn Off Read LED */
    CSW.bStatus = CSW_CMD_PASSED;
  }
}

/*
 *  MSC Memory Write Callback
 *   Called automatically on Memory Write Event
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_MemoryWrite (void) {
  U32 n;
  //add 2010-09-04
  //if(ffs_process_flag != 0) return;

  if (Block >= MSC_BlockCount) {
    BulkLen = 0;
    BulkStage = MSC_BS_CSW;
    MSC_SetStallEP(MSC_EP_OUT);
  }

  for (n = 0; n < BulkLen; n++) {
//    BlockBuf[Offset + n] = BulkBuf[n];
  }

  Offset += BulkLen;
  Length -= BulkLen;

  if ((Length == 0) && (Offset != 0)) {
    n = (Offset + (MSC_BlockSize-1)) / MSC_BlockSize;
    if (n > MSC_BlockGroup) {
      n = MSC_BlockGroup;
    }
    
	//mmc_write_sect(0,BlockBuf,Block,n);
    //modify 2010-09-06
//	if( (0 == SDInitState) && (1 == read_card_flag ) ) //sd卡初始化正常,并且读SD卡已经使能
//	{
//		mmc_write_sect(0,BlockBuf,Block, n);
//  	}else
//	{
//		//uart_printf(1,"1!\r\n");
//		Virtual_Disk_Write( BlockBuf,Block, n);
//	}

	Offset = 0;
    Block += n;
  } else if (Offset == MSC_BlockGroup*MSC_BlockSize) {
    
	
	//mmc_write_sect(0,BlockBuf,Block, MSC_BlockGroup);
    //modify 2010-09-06
//	if( (0 == SDInitState) && (1 == read_card_flag ) ) //sd卡初始化正常,并且读SD卡已经使能
//	{
//	 	mmc_write_sect(0,BlockBuf,Block, MSC_BlockGroup);
//  	}else
//	{
//		//uart_printf(1,"1!\r\n");
//		Virtual_Disk_Write( BlockBuf,Block, MSC_BlockGroup);
//	}
	
	Offset = 0;
    Block += MSC_BlockGroup;
  }

  CSW.dDataResidue -= BulkLen;

  if ((Length == 0) || (BulkStage == MSC_BS_CSW)) {
    //LED_Off (LED_WR);            /* Turn Off Write LED */
    CSW.bStatus = CSW_CMD_PASSED;
    MSC_SetCSW();
  }
}

/*
 *  MSC Memory Verify Callback
 *   Called automatically on Memory Verify Event
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_MemoryVerify (void) {
  U32 n;
  //add 2010-09-04
  //if(ffs_process_flag != 0) return;

  if (Block >= MSC_BlockCount) {
    BulkLen = 0;
    BulkStage = MSC_BS_CSW;
    MSC_SetStallEP(MSC_EP_OUT);
  }

  if ((Offset == 0) && (BulkLen != 0)) {
    n = (Length + (MSC_BlockSize-1)) / MSC_BlockSize;
    if (n > MSC_BlockGroup) {
      n = MSC_BlockGroup;
    }
	
	//modify 2010-09-06
//	if( (0 == SDInitState) && (1 == read_card_flag ) ) //sd卡初始化正常,并且读SD卡已经使能 
//	{
//	 	mmc_read_sect(0,BlockBuf,Block, n);
//  	}else
//	{
//		//uart_printf(1,"1!\r\n");
//		Virtual_Disk_Read( BlockBuf,Block, n);
//
//	}
  }

  for (n = 0; n < BulkLen; n++) {
//    if (BlockBuf[Offset + n] != BulkBuf[n]) {
//      MemOK = __FALSE;
//      break;
//    }
  }

  Offset += BulkLen;
  Length -= BulkLen;

  if (Offset == MSC_BlockGroup*MSC_BlockSize) {
    Offset = 0;
    Block += MSC_BlockGroup;
  }

  CSW.dDataResidue -= BulkLen;

  if ((Length == 0) || (BulkStage == MSC_BS_CSW)) {
    CSW.bStatus = (MemOK) ? CSW_CMD_PASSED : CSW_CMD_FAILED;
    MSC_SetCSW();
  }
}


/*
 *  MSC SCSI Read/Write Setup Callback
 *    Parameters:      None (global variables)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL MSC_RWSetup (void) {
  U32 n;

  /* Logical Block Address of First Block */
  n = (CBW.CB[2] << 24) |
      (CBW.CB[3] << 16) |
      (CBW.CB[4] <<  8) |
      (CBW.CB[5] <<  0);

  Block  = n;
  Offset = 0;

  /* Number of Blocks to transfer */
  switch (CBW.CB[0]) {
    case SCSI_READ10:
    case SCSI_WRITE10:
    case SCSI_VERIFY10:
      n = (CBW.CB[7] <<  8) |
          (CBW.CB[8] <<  0);
      break;

    case SCSI_READ12:
    case SCSI_WRITE12:
      n = (CBW.CB[6] << 24) |
          (CBW.CB[7] << 16) |
          (CBW.CB[8] <<  8) |
          (CBW.CB[9] <<  0);
      break;
  }

  Length = n * MSC_BlockSize;

  if (CBW.dDataLength == 0) {              /* host requests no data */
    CSW.bStatus = CSW_CMD_FAILED;
    MSC_SetCSW();
    return (__FALSE);
  }

  if (CBW.dDataLength != Length) {
    if ((CBW.bmFlags & 0x80) != 0) {       /* stall appropriate EP */ 
      MSC_SetStallEP(MSC_EP_IN);
    } else {
      MSC_SetStallEP(MSC_EP_OUT);
    }

    CSW.bStatus = CSW_CMD_FAILED;
    MSC_SetCSW();

    return (__FALSE);
  }

  return (__TRUE);
}


/*
 *  Check Data IN Format
 *    Parameters:      None (global variables)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL DataInFormat (void) {

  if (CBW.dDataLength == 0) {
    CSW.bStatus = CSW_PHASE_ERROR;
    MSC_SetCSW();
    return (__FALSE);
  }
  if ((CBW.bmFlags & 0x80) == 0) {
    MSC_SetStallEP(MSC_EP_OUT);
    CSW.bStatus = CSW_PHASE_ERROR;
    MSC_SetCSW();
    return (__FALSE);
  }
  return (__TRUE);
}


/*
 *  Perform Data IN Transfer
 *    Parameters:      None (global variables)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

void DataInTransfer (void) {

  if (BulkLen >= CBW.dDataLength) {
    BulkLen = CBW.dDataLength;
    BulkStage = MSC_BS_DATA_IN_LAST;
  }
  else {
    BulkStage = MSC_BS_DATA_IN_LAST_STALL; /* short or zero packet */
  }

  USB_WriteEP(MSC_EP_IN, BulkBuf, BulkLen);

  CSW.dDataResidue -= BulkLen;
  CSW.bStatus = CSW_CMD_PASSED;
}

/*
 *  MSC SCSI Test Unit Ready Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_TestUnitReady (void) {

  if (CBW.dDataLength != 0) {
    if ((CBW.bmFlags & 0x80) != 0) {
      MSC_SetStallEP(MSC_EP_IN);
    } else {
      MSC_SetStallEP(MSC_EP_OUT);
    }
  }

  CSW.bStatus = CSW_CMD_PASSED;
  MSC_SetCSW();
}


/*
 *  MSC SCSI Request Sense Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_RequestSense (void) {

  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x70;                      /* Response Code */
  BulkBuf[ 1] = 0x00;
  BulkBuf[ 2] = 0x00;                      /* Sense Key */
  BulkBuf[ 3] = 0x00;
  BulkBuf[ 4] = 0x00;
  BulkBuf[ 5] = 0x00;
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x0A;                      /* Additional Length */
  BulkBuf[ 8] = 0x00;
  BulkBuf[ 9] = 0x00;
  BulkBuf[10] = 0x00;
  BulkBuf[11] = 0x00;
  BulkBuf[12] = 0x00;                      /* ASC */
  BulkBuf[13] = 0x00;                      /* ASCQ */
  BulkBuf[14] = 0x00;
  BulkBuf[15] = 0x00;
  BulkBuf[16] = 0x00;
  BulkBuf[17] = 0x00;

  BulkLen = 18;
  DataInTransfer();
}


/*
 *  MSC SCSI Inquiry Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_Inquiry (void) {

  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x00;                      /* Direct Access Device */
  BulkBuf[ 1] = 0x80;                      /* RMB = 1: Removable Medium */
  BulkBuf[ 2] = 0x00;                      /* Version: No conformance claim to standard */
  BulkBuf[ 3] = 0x01;

  BulkBuf[ 4] = 36-4;                      /* Additional Length */
  BulkBuf[ 5] = 0x80;                      /* SCCS = 1: Storage Controller Component */
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x00;

  BulkBuf[ 8] = 'D';                       /* Vendor Identification */
  BulkBuf[ 9] = 'J';
  BulkBuf[10] = 'I';
  BulkBuf[11] = ' ';
  BulkBuf[12] = ' ';
  BulkBuf[13] = ' ';
  BulkBuf[14] = ' ';
  BulkBuf[15] = ' ';

  BulkBuf[16] = 'I';                       /* Product Identification */
  BulkBuf[17] = 'O';
  BulkBuf[18] = 'S';
  BulkBuf[19] = 'D';
  BulkBuf[20] = 'L';
  BulkBuf[21] = 'O';
  BulkBuf[22] = 'G';
  BulkBuf[23] = ' ';
  BulkBuf[24] = ' ';
  BulkBuf[25] = ' ';
  BulkBuf[26] = ' ';
  BulkBuf[27] = ' ';
  BulkBuf[28] = ' ';
  BulkBuf[29] = ' ';
  BulkBuf[30] = ' ';
  BulkBuf[31] = ' ';

  BulkBuf[32] = '1';                       /* Product Revision Level */
  BulkBuf[33] = '.';
  BulkBuf[34] = '0';
  BulkBuf[35] = ' ';

  BulkLen = 36;
  DataInTransfer();
}


/*
 *  MSC SCSI Mode Sense (6-Byte) Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_ModeSense6 (void) {

  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x03;
  BulkBuf[ 1] = 0x00;
  BulkBuf[ 2] = 0x00;
  BulkBuf[ 3] = 0x00;

  BulkLen = 4;
  DataInTransfer();
}


/*
 *  MSC SCSI Mode Sense (10-Byte) Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_ModeSense10 (void) {

  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x00;
  BulkBuf[ 1] = 0x06;
  BulkBuf[ 2] = 0x00;
  BulkBuf[ 3] = 0x00;
  BulkBuf[ 4] = 0x00;
  BulkBuf[ 5] = 0x00;
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x00;

  BulkLen = 8;
  DataInTransfer();
}


/*
 *  MSC SCSI Read Capacity Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_ReadCapacity (void) {

  if (!DataInFormat()) return;

  /* Last Logical Block */
  BulkBuf[ 0] = ((MSC_BlockCount - 1) >> 24) & 0xFF;
  BulkBuf[ 1] = ((MSC_BlockCount - 1) >> 16) & 0xFF;
  BulkBuf[ 2] = ((MSC_BlockCount - 1) >>  8) & 0xFF;
  BulkBuf[ 3] = ((MSC_BlockCount - 1) >>  0) & 0xFF;

  /* Block Length */
  BulkBuf[ 4] = (MSC_BlockSize >> 24) & 0xFF;
  BulkBuf[ 5] = (MSC_BlockSize >> 16) & 0xFF;
  BulkBuf[ 6] = (MSC_BlockSize >>  8) & 0xFF;
  BulkBuf[ 7] = (MSC_BlockSize >>  0) & 0xFF;

  BulkLen = 8;
  DataInTransfer();
}

/*
 *  MSC SCSI Read Format Capacity Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_ReadFormatCapacity (void) {

  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x00;
  BulkBuf[ 1] = 0x00;
  BulkBuf[ 2] = 0x00;
  BulkBuf[ 3] = 0x08;                      /* Capacity List Length */

  /* Block Count */
  BulkBuf[ 4] = (MSC_BlockCount >> 24) & 0xFF;
  BulkBuf[ 5] = (MSC_BlockCount >> 16) & 0xFF;
  BulkBuf[ 6] = (MSC_BlockCount >>  8) & 0xFF;
  BulkBuf[ 7] = (MSC_BlockCount >>  0) & 0xFF;

  /* Block Length */
  BulkBuf[ 8] = 0x02;                      /* Descriptor Code: Formatted Media */
  BulkBuf[ 9] = (MSC_BlockSize >> 16) & 0xFF;
  BulkBuf[10] = (MSC_BlockSize >>  8) & 0xFF;
  BulkBuf[11] = (MSC_BlockSize >>  0) & 0xFF;

  BulkLen = 12;
  DataInTransfer();
}


/*
 *  MSC Get Command Block Wrapper Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_GetCBW (void) {
  U32 n;

  for (n = 0; n < BulkLen; n++) {
    *((U8 *)&CBW + n) = BulkBuf[n];
  }
  if ((BulkLen == sizeof(CBW)) && (CBW.dSignature == MSC_CBW_Signature)) {
    /* Valid CBW */
    CSW.dTag = CBW.dTag;
    CSW.dDataResidue = CBW.dDataLength;
    if ((CBW.bLUN      != 0) || 
        (CBW.bCBLength <  1) || 
        (CBW.bCBLength > 16)   ) {
fail: 
      CSW.bStatus = CSW_CMD_FAILED;
      MSC_SetCSW();
    } else {
      switch (CBW.CB[0]) {
        case SCSI_TEST_UNIT_READY:
          MSC_TestUnitReady();
          break;
        case SCSI_REQUEST_SENSE:
          MSC_RequestSense();
          break;
        case SCSI_FORMAT_UNIT:
          goto fail;
        case SCSI_INQUIRY:
          MSC_Inquiry();
          break;
        case SCSI_START_STOP_UNIT:
          goto fail;
        case SCSI_MEDIA_REMOVAL:
          goto fail;
        case SCSI_MODE_SELECT6:
          goto fail;
        case SCSI_MODE_SENSE6:
          MSC_ModeSense6();
          break;
        case SCSI_MODE_SELECT10:
          goto fail;
        case SCSI_MODE_SENSE10:
          MSC_ModeSense10();
          break;
        case SCSI_READ_FORMAT_CAPACITIES:
          MSC_ReadFormatCapacity();
          break;
        case SCSI_READ_CAPACITY:
          MSC_ReadCapacity();
          break;
        case SCSI_READ10:
		case SCSI_READ12:
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) != 0) {
              //LED_On (LED_RD);    /* Turn On Read LED */
              
			  BulkStage = MSC_BS_DATA_IN;
			  MSC_MemoryRead();
            } else {                       /* direction mismatch */
              MSC_SetStallEP(MSC_EP_OUT);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
            }
          }
          break;
        case SCSI_WRITE10:
		case SCSI_WRITE12:
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) == 0) {
              //LED_On (LED_WR);    /* Turn On Write LED */
              BulkStage = MSC_BS_DATA_OUT;
            } else {                       /* direction mismatch */
              MSC_SetStallEP(MSC_EP_IN);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
            }
          }
          break;
        case SCSI_VERIFY10:
          if ((CBW.CB[1] & 0x02) == 0) {
            // BYTCHK = 0 -> CRC Check (not implemented)
            CSW.bStatus = CSW_CMD_PASSED;
            MSC_SetCSW();
            break;
          }
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) == 0) {
              BulkStage = MSC_BS_DATA_OUT;
              MemOK = __TRUE;
            } else {
              MSC_SetStallEP(MSC_EP_IN);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
            }
          }
          break;
        default:
          goto fail;
      }
    }
  } else {
    /* Invalid CBW */
    MSC_SetStallEP(MSC_EP_IN);
                                           /* set EP to stay stalled */
    USB_EndPointStall |=  (MSC_EP_IN  & 0x80) ? ((1 << 16) << (MSC_EP_IN  & 0x0F)) : (1 << MSC_EP_IN);
    MSC_SetStallEP(MSC_EP_OUT);
                                           /* set EP to stay stalled */
    USB_EndPointStall |=  (MSC_EP_OUT & 0x80) ? ((1 << 16) << (MSC_EP_OUT & 0x0F)) : (1 << MSC_EP_OUT);
    BulkStage = MSC_BS_ERROR;
  }
}

/*
 *  MSC Set Command Status Wrapper Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_SetCSW (void) {
  CSW.dSignature = MSC_CSW_Signature;
  USB_WriteEP(MSC_EP_IN, (U8 *)&CSW, sizeof(CSW));
  BulkStage = MSC_BS_CSW;
}

/*
 *  MSC Bulk In Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */
int flag_msc_readmemory = 0;
//extern OS_EVENT*  SD_READ_Sem;
void MSC_BulkIn (void) {

  switch (BulkStage) {
    case MSC_BS_DATA_IN:
      switch (CBW.CB[0]) {
        case SCSI_READ10:
        case SCSI_READ12:
           //flag_msc_readmemory = 1;
		   
		   //OSSemPost(SD_READ_Sem);
		  MSC_MemoryRead();
		  break;
      }
      break;
    case MSC_BS_DATA_IN_LAST:
      MSC_SetCSW();
      break;
    case MSC_BS_DATA_IN_LAST_STALL:
      MSC_SetStallEP(MSC_EP_IN);
      MSC_SetCSW();
      break;
    case MSC_BS_CSW:
      BulkStage = MSC_BS_CBW;
      break;
  }
}


/*
 *  MSC Bulk Out Callback
 *    Parameters:      None (global variables)
 *    Return Value:    None
 */

void MSC_BulkOut (void) {

  BulkLen = USB_ReadEP(MSC_EP_OUT, BulkBuf);
  switch (BulkStage) {
    case MSC_BS_CBW:
      MSC_GetCBW();
      break;
    case MSC_BS_DATA_OUT:
      switch (CBW.CB[0]) {
        case SCSI_WRITE10:
        case SCSI_WRITE12:
          MSC_MemoryWrite();
          break;
        case SCSI_VERIFY10:
          MSC_MemoryVerify();
          break;
      }
      break;
      case MSC_BS_CSW:
        break;
    default:
      MSC_SetStallEP(MSC_EP_OUT);
      CSW.bStatus = CSW_PHASE_ERROR;
      MSC_SetCSW();
      break;
  }
}

//add 2010-09-06
//读虚拟磁盘
void Virtual_Disk_Read( U8 *buff,U32 offset, U32 length)
{
  	U32 i;
  	U8 data;
	
	offset = 512*offset;
	length = 512*length;

	//uart_printf(1,"%d %d!\r\n",offset,length);
	
  	for ( i = 0; i<length; i++)
  	{
    	if (offset < BOOT_SECT_SIZE)
		{
      		switch (offset)
	  		{
	    		case 19:
		  			data = (U8)(Virtual_MSC_BlockCount & 0xFF);
					break;
				case 20:
		  			data = (U8)((Virtual_MSC_BlockCount >> 8) & 0xFF);
					break;
				case 510:
		  			data = 0x55;
					break;
				case 511:
		  			data = 0xAA;
					break;
				default:
				if ( offset > 29 )
				{
	      			data = 0x0;
				}
				else
				{
		  			data = BootSect[offset];
				}
				break;
	  		}
		}
		else if (offset < (BOOT_SECT_SIZE + FAT_SIZE))
	    {
		  	//fat表的记录项只用一个,加上两个保留项一共三个
			if( offset < (BOOT_SECT_SIZE + 5) )
			{
				data = Virtual_FatTable[offset - BOOT_SECT_SIZE];
			}else
			{
				data = 0x0;
			}

		}
		else if(offset < (BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE))
		{
			//root dir区,用到两个项,一个是磁盘名目录项,一个是文件名目录项	
			if( offset < (BOOT_SECT_SIZE + FAT_SIZE + DIR_ENTRY) )
			{
				data = RootDirEntry[offset - BOOT_SECT_SIZE - FAT_SIZE];
			}else
			{
				data = 0x0; 
			}
		}
		else
		{
		     //txt 中的内容 Virtual_Context[] 64为Virtual_Context[]的长度
			 if( offset < (BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE + 1024))
			 {
					data = Virtual_Context2[offset - BOOT_SECT_SIZE - FAT_SIZE - ROOT_DIR_SIZE];
			 }else
			 {
			 	data = 0x0;
			 }
		}
   	
    	buff[i] = data;
		offset++;
  	}
}

//add 2010-09-06
//写虚拟磁盘
void Virtual_Disk_Write( U8 *buff,U32 offset, U32 length)
{
  	U32 i;
	offset = 512*offset;
	length = 512*length;
    
	if (offset < BOOT_SECT_SIZE)
	{
	  /* Can't write boot sector */
	}
	else if (offset < (BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE))
    {
    	for ( i = 0; i<length; i++)
	    {
			 //禁止修改虚拟磁盘内容
//		     Fat_RootDir[(offset+i) - BOOT_SECT_SIZE] = buff[i];
//			 if ( buff[i] == 0xe5 )
//			 {
//			     if ( (offset+i) == BOOT_SECT_SIZE + FAT_SIZE + 32 )
//				 {
//				 }
//			 }
		}
	}else
	{
		 //禁止修改虚拟磁盘内容
	}
}
