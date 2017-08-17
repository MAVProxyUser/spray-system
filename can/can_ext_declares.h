/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : WKM2_CAN_LOADER.uvproj
  * @File Name          : can_ext_declares.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-10-17 
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *	      
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_EXT_DECLARES_H__
#define __CAN_EXT_DECLARES_H__

/* extern variables ----------------------------------------------------------*/
extern CAN_MSG_Type       msgRxBuf;

extern CPU_INT08U         can1RxBuf[CAN_BUFSIZE] ;	   //
extern CAN_RING_BUF_Type  can1RxRingBuf          ;

extern CPU_INT08U         can1Rx2Buf[CAN_BUFSIZE] ;	   //
extern CAN_RING_BUF_Type  can1Rx2RingBuf          ;

extern CPU_INT08U         can1RxUpgradeBuf[CAN_BUFSIZE] ;	   //
extern CAN_RING_BUF_Type  can1RxUpgradeRingBuf          ;

extern CPU_INT08U         can1RxLEDBuf[CAN_BUFSIZE] ;	   //
extern CAN_RING_BUF_Type  can1RxLEDRingBuf          ;

extern CPU_INT08U         can1TxBuf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can1TxRingBuf          ;

extern CPU_INT08U         can1Tx2Buf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can1Tx2RingBuf          ;

extern CPU_INT08U         can1Tx3Buf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can1Tx3RingBuf          ;

extern CPU_INT08U         can1UpgradeAckTxBuf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can1UpgradeAckTxRingBuf          ;

extern CPU_INT08U         can1LEDTxBuf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can1LEDTxRingBuf          ;
#if __CAN2_ENABLE__
extern CPU_INT08U         can2RxBuf[CAN_BUFSIZE] ;	   //
extern CAN_RING_BUF_Type  can2RxRingBuf          ;
#endif
extern CPU_INT08U         can2TxBuf[CAN_BUFSIZE] ;
extern CAN_RING_BUF_Type  can2TxRingBuf          ;

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif
