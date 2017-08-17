
#ifndef __USER_INTERRUPT_H__
#define __USER_INTERRUPT_H__
#include <LPC17xx.h>
#include "stdint.h"

void Interrupt_Init( void );


extern uint32_t g_u32PluseCnt1;
extern uint32_t g_u32PluseCnt2;

//extern uint16_t g_Distance ;
extern uint32_t g_SonarGetTime;
#include "cfg_decoder.h"
bool radar_upack( cfg_decoder_object_t *obj, uint8_t data );

void flow_cali_enble(void);
void flow_cali_clear(void);
uint32_t get_cali_flow_pulses_total(void);
uint16_t get_cali_flow_freq(void);
uint32_t get_cail_flow_time(void);
#endif
