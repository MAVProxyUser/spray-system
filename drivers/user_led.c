
#include "LPC17xx.h"
#include "user_led.h"

void LedInit( void )
{
	LPC_GPIO0->FIODIR |= 1 << 5;
	LED_GREEN_ON;
	//LED_GREEN_OFF;
}









