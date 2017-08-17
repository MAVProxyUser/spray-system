/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_timer_pwm.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPC17XX_TIMER_PWM_H__
#define __LPC17XX_TIMER_PWM_H__

/* Private define ------------------------------------------------------------*/
/* timer and pwm enable */
#define TIMER0_EN     0
#define TIMER1_EN     0
#define TIMER2_EN     0
#define TIMER3_EN     0
#define PWM0_EN       0
#define PWM1_EN       1

/* TIMER and PWM pin bit field */
#define TIMER1_CAP0_PIN    DEF_BIT_18        /* p1.18,p1.19 FUNT_4 -- cap1.0,cap1.1 */
#define TIMER1_CAP1_PIN    DEF_BIT_19
#define TIMER2_CAP0_PIN	   DEF_BIT_04		 /* p0.4,p0.5   FUNT_4 -- cap2.0,cap2.1 */
#define TIMER2_CAP1_PIN	   DEF_BIT_05
#define TIMER3_CAP0_PIN	   DEF_BIT_23		 /* p0.23,p0.24 FUNT_4 -- cap3.0,cap3.1 */
#define TIMER3_CAP1_PIN	   DEF_BIT_24
#define PWM1_CAP0_PIN	   DEF_BIT_28		 /* p1.28,p1.29 FUNT_3 -- pcap1.0,pcap1.1 */
#define PWM1_CAP1_PIN	   DEF_BIT_29

#define TIMER1_MAT1_PIN	   DEF_BIT_25		 /* p1.25 FUNT_4 -- MAT1.1 */

#define TIMER2_MAT0_PIN    DEF_BIT_06		 /* p0.6  FUNT_4 -- MAT2.1 */
#define TIMER2_MAT1_PIN	   DEF_BIT_07		 /* p0.7  FUNT_4 -- MAT2.1 */
#define TIMER2_MAT2_PIN	   DEF_BIT_08		 /* p0.8  FUNT_4 -- MAT2.1 */

#define TIMER3_MAT1_PIN	   DEF_BIT_11		 /* p0.11 FUNT_4 -- MAT3.1 */

#define PWM1_MAT1_PIN	   DEF_BIT_00		 /* p2.0,p2.1,p2.4,p2.5 FUNT_2 -- PWM1.1,PWM1.2,PWM1.5,PWM1.6 */
#define PWM1_MAT2_PIN	   DEF_BIT_01
#define PWM1_MAT3_PIN	   DEF_BIT_02
#define PWM1_MAT4_PIN	   DEF_BIT_03
#define PWM1_MAT5_PIN	   DEF_BIT_04
#define PWM1_MAT6_PIN	   DEF_BIT_05

/* IR Interrupt Flag for Timer and PWM */
#define IR_MR0_INT_FLAG    DEF_BIT_00 
#define IR_MR1_INT_FLAG    DEF_BIT_01
#define IR_MR2_INT_FLAG    DEF_BIT_02
#define IR_MR3_INT_FLAG    DEF_BIT_03
#define IR_CR0_INT_FLAG    DEF_BIT_04
#define IR_CR1_INT_FLAG    DEF_BIT_05            

#define IR_MR4_INT_FLAG    DEF_BIT_08        /* PWM only */ 
#define IR_MR5_INT_FLAG    DEF_BIT_09
#define IR_MR6_INT_FLAG    DEF_BIT_10

/* TCR  bit filed and bit set */
#define TCR_CNT_EN		   DEF_BIT_00
#define TCR_CNT_RESET	   DEF_BIT_01
#define TCR_PWM_EN		   DEF_BIT_03

/* PR value */
#define PR_VALUE_0         0u		     

/* MCR bit filed and bit set */
#define MCR_MR0_INT_EN	   DEF_BIT_00
#define MCR_MR0_RESET_EN   DEF_BIT_01
#define MCR_MR0_STOP_EN    DEF_BIT_02

#define MCR_MR1_INT_EN	   DEF_BIT_03
#define MCR_MR1_RESET_EN   DEF_BIT_04
#define MCR_MR1_STOP_EN    DEF_BIT_05

#define MCR_MR2_INT_EN	   DEF_BIT_06
#define MCR_MR2_RESET_EN   DEF_BIT_07
#define MCR_MR2_STOP_EN    DEF_BIT_08

#define MCR_MR3_INT_EN	   DEF_BIT_09
#define MCR_MR3_RESET_EN   DEF_BIT_10
#define MCR_MR3_STOP_EN    DEF_BIT_11

#define MCR_MR4_INT_EN	   DEF_BIT_12       /* PWM only */
#define MCR_MR4_RESET_EN   DEF_BIT_13
#define MCR_MR4_STOP_EN    DEF_BIT_14

#define MCR_MR5_INT_EN	   DEF_BIT_15
#define MCR_MR5_RESET_EN   DEF_BIT_16
#define MCR_MR5_STOP_EN    DEF_BIT_17

#define MCR_MR6_INT_EN	   DEF_BIT_18
#define MCR_MR6_RESET_EN   DEF_BIT_19
#define MCR_MR6_STOP_EN    DEF_BIT_20

/* TCCR */
#define TCCR_CAP0_RE_EN	   DEF_BIT_00
#define TCCR_CAP0_FE_EN	   DEF_BIT_01
#define TCCR_CAP0_INT_EN   DEF_BIT_02

#define TCCR_CAP1_RE_EN	   DEF_BIT_03
#define TCCR_CAP1_FE_EN	   DEF_BIT_04
#define TCCR_CAP1_INT_EN   DEF_BIT_05


/* PCR bit filed and bit set */
#define PCR_PWM_SEL2	   DEF_BIT_02     /* single&double edge select */
#define PCR_PWM_SEL3	   DEF_BIT_03
#define PCR_PWM_SEL4	   DEF_BIT_04
#define PCR_PWM_SEL5	   DEF_BIT_05
#define PCR_PWM_SEL6	   DEF_BIT_06

#define PCR_PWM_EN1		   DEF_BIT_09	 /* output EN */
#define PCR_PWM_EN2		   DEF_BIT_10
#define PCR_PWM_EN3		   DEF_BIT_11
#define PCR_PWM_EN4		   DEF_BIT_12
#define PCR_PWM_EN5		   DEF_BIT_13
#define PCR_PWM_EN6		   DEF_BIT_14

/* PWM LER bit field and bit set */
#define LER_MR0_LATCH	   DEF_BIT_00
#define LER_MR1_LATCH	   DEF_BIT_01
#define LER_MR2_LATCH	   DEF_BIT_02
#define LER_MR3_LATCH	   DEF_BIT_03
#define LER_MR4_LATCH      DEF_BIT_04
#define LER_MR5_LATCH	   DEF_BIT_05
#define LER_MR6_LATCH	   DEF_BIT_06

/* EMR bit field and bit set */
#define EMR_EM0_LOW 	   0
#define EMR_EM0_HIGH 	   DEF_BIT_00
#define EMR_EM1_LOW		   0
#define EMR_EM1_HIGH 	   DEF_BIT_01
#define EMR_EM2_LOW 	   0
#define EMR_EM2_HIGH 	   DEF_BIT_02
#define EMR_EM3_LOW 	   0
#define EMR_EM3_HIGH 	   DEF_BIT_03

#define EMR_EMC0_BIT_SHIFT 4u
#define EMR_EMC0_BIT_MASK  0x00000030
#define EMR_EMC0_DN        0x00000000
#define EMR_EMC0_CLR       0x00000010
#define EMR_EMC0_SET       0x00000020
#define EMR_EMC0_TOGGLE    0x00000030

#define EMR_EMC1_BIT_SHIFT 6u
#define EMR_EMC1_BIT_MASK  0x000000c0
#define EMR_EMC1_DN        0x00000000
#define EMR_EMC1_CLR       0x00000040
#define EMR_EMC1_SET       0x00000080
#define EMR_EMC1_TOGGLE    0x000000c0

#define EMR_EMC2_BIT_SHIFT 8u
#define EMR_EMC2_BIT_MASK  0x00000300
#define EMR_EMC2_DN        0x00000000
#define EMR_EMC2_CLR       0x00000100
#define EMR_EMC2_SET       0x00000200
#define EMR_EMC2_TOGGLE    0x00000300

#define EMR_EMC3_BIT_SHIFT 10u
#define EMR_EMC3_BIT_MASK  0x00000c00
#define EMR_EMC3_DN        0x00000000
#define EMR_EMC3_CLR       0x00000400
#define EMR_EMC3_SET       0x00000800
#define EMR_EMC3_TOGGLE    0x00000c00

/* match value */
#define T0_Value           45000u

#define TIMER_CLOCK        25000000u
#define T_SYS_VALUE 	   25000000u
#define T_Value_400HZ      (T_SYS_VALUE/50)

/* Exported variables --------------------------------------------------------*/
extern volatile CPU_INT32U PulseIn[8];        /* Input pulse Duty  */
extern volatile CPU_INT32U PulseOut[8];       /* Output pulse Duty */
extern volatile CPU_INT32U PulseIn_period[8];

/* Function declaration ------------------------------------------------------*/
#if TIMER0_EN > 0
void Timer0ISR_Handler (void);
void TIMER0_Init(CPU_INT32U cycle);
void MAT0_Start(void);
#endif

#if TIMER1_EN > 0
void Timer1ISR_Handler (void);
void TIMER1_Init(CPU_INT32U cycle);
void MAT1_Start(void);
void TIMER1_set_frequent( CPU_INT32U cycle );
void TIMER1_enable_pwmout(void);
void TIMER1_disable_pwmout(void);
#endif

#if TIMER2_EN > 0
void Timer2ISR_Handler (void);
void TIMER2_Init(CPU_INT32U cycle);
void MAT2_Start(void);

void TIMER2_set_frequent( INT32U cycle );
void TIMER2_enable_pwmout(void);
void TIMER2_disable_pwmout(void);
#endif

#if TIMER3_EN > 0
void Timer3ISR_Handler (void);
void TIMER3_Init(CPU_INT32U cycle);
void MAT3_Start(void);

void TIMER3_set_frequent( CPU_INT32U cycle );
void TIMER3_enable_pwmout(void);
void TIMER3_disable_pwmout(void);
#endif

#if PWM0_EN > 0
void PWM0_Init( CPU_INT32U cycle );
void PWM0_Start( void );

void PWM0_set_frequent( CPU_INT32U cycle);
void PWM0_enable_pwmout( void );
void PWM0_disable_pwmout( void );
#endif

#if PWM1_EN > 0
void PWM1_Handler(void);
void PWM1_Init( CPU_INT32U cycle, CPU_INT32U duty );
void PWM1_Start( void );

void PWM1_set_frequent( CPU_INT32U cycle);
void PWM1_enable_pwmout( void );
void PWM1_disable_pwmout( void );
#endif

/* add 2011-03-29 */
void tmp_ctl_enable(void); 

void TIMER0_Init( CPU_INT32U cycle );
void timer0_start ( void );

void cap_pwm_start( void );
void cap_pwm_init(CPU_INT32U sp_speed,CPU_INT32U rudder_speed);
void cap_pwm_test_frequent_init(void);	

void WritePusleOutBuf(CPU_INT16U* pOutBuf);
void PWM1_Init_Place(CPU_INT32U* pServoOut);
void set_servo_place_direct(int channel_num,CPU_INT16U  pulse_w);
void ServoOutPut(void);
void set_rudder_servoout_speed(CPU_INT32U rudder_speed);

/* add 2012-02-07 */
void TIMER2_Init( void );
void MAT2_Start( void );
void MAT2_Stop( void );

/*  */

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
#endif
