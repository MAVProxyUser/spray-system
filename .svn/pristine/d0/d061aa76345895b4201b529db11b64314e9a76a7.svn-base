#include "lpc17xx_wdt.h"

static uint8_t wdt_locked = 0;

void WDT_Init( void )
{
	//Set clock source
	LPC_WDT->WDCLKSEL &= ~( 3 << 0 );
	LPC_WDT->WDCLKSEL |= 0x01;
	//Set WDT mode
	LPC_WDT->WDMOD |= 0x02;
}

void WDT_Start( uint32_t ms )
{
	LPC_WDT->WDTC = ms * 6250;
	//enable watchdog
	LPC_WDT->WDMOD |= 0x01;
	WDT_Feed();
}

void WDT_UpdateTimeOut ( uint32_t ms )
{
	LPC_WDT->WDTC = ms * 6250;
	WDT_Feed();
}

void WDT_Feed( void )
{
	CPU_SR_ALLOC();
	if( wdt_locked ) return;
	CPU_CRITICAL_ENTER(); 
	LPC_WDT->WDFEED = 0xAA;
	LPC_WDT->WDFEED = 0x55;
	CPU_CRITICAL_EXIT();
}

uint32_t WDT_GetCurrentCount( void )
{
	return LPC_WDT->WDTV;
}

uint8_t WDT_ResetCheck( void )
{
	return ( LPC_WDT->WDMOD & 0x04 );
}

void WDT_Lock( void )
{
	wdt_locked = 1;
}

void WDT_UnLock( void )
{
	wdt_locked = 0;
}

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

