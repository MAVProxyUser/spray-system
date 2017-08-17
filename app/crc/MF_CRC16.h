/******************************************************************************* 
*                      CRC16  functions algorithm  liboray                     * 
*   file : MF_CRC16.h                                                          *
*                                                                              *
*   history :                                                                  * 
*     v1.0 2010-07-07   Motorfeng                                              *
*******************************************************************************/ 
#ifndef  __MF_CRC16_CPP__
#define  __MF_CRC16_CPP__

#include "../../cstartup/type.h"

#define CRC_INIT 			0x3692 


uint16_t Get_CRC16_Check_Sum( uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC ); 
uint32_t Verify_CRC16_Check_Sum( uint8_t *pchMessage, uint32_t dwLength ); 
void Append_CRC16_Check_Sum( uint8_t *pchMessage, uint32_t dwLength ); 

#endif
/*
********************************************************************************
*                        END
********************************************************************************
*/
