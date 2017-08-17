#ifndef __LPC17XX_TIMER_H
#define __LPC17XX_TIMER_H

#include "lpc17xx.h"

void TIMER0_Init( void );
void TIMER0_Start( void );
void TIMER0_Stop( void );
void TIMER0_Reset( void );

uint8_t TIMER0_1Ms_Timeout( void );

void TIMER1_Init( void );
void TIMER1_Start( void );
void TIMER1_Stop( void );

void TIMER2_Init( void );

#define TIMER2_START  LPC_TIM2->TC = 0 ; LPC_TIM2->TCR = 0x01
#define TIMER2_STOP 	LPC_TIM2->TCR &= ~0x01

#endif
