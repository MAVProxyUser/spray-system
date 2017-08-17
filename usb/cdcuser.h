/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    cdcuser.h
 * Purpose: USB Communication Device Class User module Definitions
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

#ifndef __CDCUSER_H__
#define __CDCUSER_H__

/* CDC in/out buffer handling */
extern int CDC_RdOutBuf        (char *buffer, const int *length);
extern     CDC_WrOutBuf        (const char *buffer, int *length);
extern     CDC_OutBufAvailChar (int *availChar);

extern int CDC_RdInBuf         (char *buffer, const int *length);
extern int CDC_WrInBuf         (const char *buffer, int *length);

/* CDC Data In/Out Endpoint Address */
#define CDC_DEP_IN       0x82
#define CDC_DEP_OUT      0x02

/* CDC Communication In Endpoint Address */
#define CDC_CEP_IN       0x81

/* CDC Requests Callback Functions */
extern BOOL CDC_SendEncapsulatedCommand  (void);
extern BOOL CDC_GetEncapsulatedResponse  (void);
extern BOOL CDC_SetCommFeature           (unsigned short wFeatureSelector);
extern BOOL CDC_GetCommFeature           (unsigned short wFeatureSelector);
extern BOOL CDC_ClearCommFeature         (unsigned short wFeatureSelector);
extern BOOL CDC_GetLineCoding            (void);
extern BOOL CDC_SetLineCoding            (void);
extern BOOL CDC_SetControlLineState      (unsigned short wControlSignalBitmap);
extern BOOL CDC_SendBreak                (unsigned short wDurationOfBreak);

/* CDC Bulk Callback Functions */
extern void CDC_StartOfFrame             (void);
extern void CDC_BulkIn                   (void);
extern void CDC_BulkOut                  (void);

/* CDC Notification Callback Function */
extern void CDC_NotificationIn           (void);

/* CDC Initializtion Function */
extern void CDC_Init (void);

/* CDC prepare the SERAIAL_STATE */
extern unsigned short CDC_GetSerialState (void);

/* flow control */
extern unsigned short volatile CDC_DepInEmpty;         /* DataEndPoint IN empty */

#endif  /* __CDCUSER_H__ */

