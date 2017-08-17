#ifndef __USER_PWM_H__
#define __USER_PWM_H__

#include <LPC17xx.h>
#include "stdint.h"

#define MIN_CNT   940
#define MAX_CNT   2000

//#define Get_Spary_Limit_Value(MIN_SPRAY_ID)    1230
//#define MAX_SPARY    1360

#define MAX_FLOW 127
#define MIN_FLOW 20

#define STOP_PWM  100000
#define MIN_PWM   115000
#define MAX_PWM   200000

void User_PWM_Init(void);
void Set_Pwm_Value(uint8_t persent);

//void SetPWM(uint8_t num , uint32_t persent);

#define WAIT_SET_H2L_TIMEOUT(time)  ( (SetPWMFrom_H2L > 0 )&& (g_current_tick - SetPWMFrom_H2L > time) )
#define WAIT_SET_FROM_ZERO( time )  ( /*(SetPWMFrom_Zero > 0 )&&*/ (g_current_tick - SetPWMFrom_Zero > time) )
#endif
