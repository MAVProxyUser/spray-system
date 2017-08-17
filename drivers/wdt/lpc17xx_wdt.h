#ifndef __LPC17XX_WDT_H__
#define __LPC17XX_WDT_H__

#include "..\drivers.h"

void WDT_Init( void );
void WDT_Start( uint32_t ms );
void WDT_Feed( void );
void WDT_UpdateTimeOut( uint32_t ms );
uint32_t WDT_GetCurrentCount( void );
uint8_t WDT_ResetCheck( void );
void WDT_Lock( void );
void WDT_UnLock( void );

#endif
