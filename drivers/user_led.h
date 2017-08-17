
#ifndef __USER_LED_H__
#define __USER_LED_H__
#define LED_GREEN_TOGGLE   LPC_GPIO0->FIOPIN^=1<< 5
#define LED_GREEN_ON       LPC_GPIO0->FIOCLR|=1<< 5
#define LED_GREEN_OFF      LPC_GPIO0->FIOSET|=1<< 5

//#define LED_RED_TOGGLE   LPC_GPIO2->FIOPIN^=1<< 11
//#define LED_RED_ON       LPC_GPIO2->FIOCLR|=1<< 11
//#define LED_RED_OFF      LPC_GPIO2->FIOSET|=1<< 11

void LedInit( void );

#endif
