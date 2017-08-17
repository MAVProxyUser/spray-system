/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_timer_pwm.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */

/* Includes ------------------------------------------------------------------*/
#include "../drivers.h"
#include "../../app/sys/sys_inc.h"

/* Exported variables --------------------------------------------------------*/
volatile CPU_INT32U PulseIn[8]  = {0};  /* Input pulse Duty */
volatile CPU_INT32U PulseOut[8] = {27360,27360,27360,27360,27360,27360,27360,27360}; /* Output pulse Duty */
volatile CPU_INT32U PulseIn_period[8] = {0};

/*
 * Name			  : void cap_pwm_init(INT32U sp_speed,INT32U rudder_speed);
 * Input          : 
 * Output         : None
 * Caller(s)      : application.
 * Return         : None
 * Notes          :	(填写使用此函数的注意事项)
 *
 */
void cap_pwm_init(CPU_INT32U sp_speed,CPU_INT32U rudder_speed)
{   
#if TIMER1_EN > 0	
	TIMER1_Init( T_SYS_VALUE/rudder_speed ); 
#endif
    
#if TIMER2_EN > 0
	//TIMER2_Init( T_SYS_VALUE*100/sp_speed ); // 4 7 5 out
	TIMER2_Init( T_SYS_VALUE/sp_speed );
#endif

#if TIMER3_EN > 0    
	//TIMER3_Init( T_SYS_VALUE*100/sp_speed);  // 6 out for rudder
	TIMER3_Init( T_SYS_VALUE/sp_speed);	
#endif

#if PWM0_EN > 0	
	//PWM0_Init( T_SYS_VALUE*100/sp_speed); 
#endif

#if PWM1_EN > 0	
	//PWM1_Init( T_SYS_VALUE*100/(sp_speed*2)); // 1,2 for swash plate
	PWM1_Init( T_SYS_VALUE/(sp_speed*2) );
#endif
}

void cap_pwm_start( void )
{   
    CPU_INT32U i;

#if TIMER1_EN > 0		
	MAT1_Start( );
	i = 1000; 		 /* 避免定时器同时启动 */
    while(i--){}
#endif

#if TIMER2_EN > 0
	MAT2_Start( );
    i = 1000; 
    while(i--){}
#endif

#if TIMER3_EN > 0
    MAT3_Start( );
	i = 1000; 
    while(i--){}
#endif

#if PWM1_EN > 0
	PWM1_Start( );
#endif

#if PWM0_EN > 0
	PWM0_Start();
#endif
}


void WritePusleOutBuf(CPU_INT16U* pOutBuf)
{	
    CPU_INT08U i;

	//pOutBuf++;
	for(i=0; i<C_HARDWARE_SERVO_IN_OUT_NUM; i++)
	{
		if( i == SERVOIN_CHANNEL_RUDDER || i == SERVOIN_CHANNEL_THROTTLE )
		{
			pOutBuf++;
			continue;
	   	}
		PulseOut[i] =  (CPU_INT32U)(*pOutBuf++);// MID_PITCH;
	}
}
 
void set_servo_place_direct(int channel_num,INT16U  pulse_w)
{
	PulseOut[channel_num] = (CPU_INT32U)pulse_w;
}

void PWM1_Init_Place(CPU_INT32U* pServoOut)
{
	int i = 0;
	for( ; i < C_HARDWARE_SERVO_IN_OUT_NUM; i++)
	{
		if(i == SERVOIN_CHANNEL_RUDDER)
		{
			set_servo_place_direct(i,ServoMid_RUDDER);
		}else
		{
			set_servo_place_direct(i,pServoOut[i]);	
		}
	}
}


#if TIMER3_EN > 0
void set_rudder_servoout_speed(CPU_INT32U rudder_speed)
{
	LPC_TIM3->TCR = 0;
 	TIMER3_Init( T_SYS_VALUE/rudder_speed );
	LPC_TIM3->TCR = 1;
}
#endif

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
