/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_timer1.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */

/* Includes ------------------------------------------------------------------*/
#include "../drivers.h"

/* Exported variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static CPU_INT32U cycle_timer1 = 0;

static CPU_INT32U Last_T1CR0   = 0;
static CPU_INT08U T1CAP0_Flag  = 0;         /* cap0上升沿 标志 */
static volatile CPU_INT08U T1CAP0_OT = 0;   /* cap0上升沿之后下跳沿之前 定时器溢出次数 */ 
 
static CPU_INT32U Last_T1CR1  = 0;
static CPU_INT08U T1CAP1_Flag = 0;          /* cap0上升沿 标志 */
static volatile CPU_INT08U T1CAP1_OT = 0;   /* cap1上升沿之后下跳沿之前 定时器溢出次数 */

static CPU_INT32U last_rising_edge_T1CR0 = 0;
static CPU_INT32U last_rising_edge_T1CR1 = 0;
static CPU_INT08U period_cap_T1CR0_overflow_count = 0;
static CPU_INT08U period_cap_T1CR1_overflow_count = 0;

/* Imported variables --------------------------------------------------------*/
extern volatile CPU_INT32U PulseIn[8];        /* Input pulse Duty  */
extern volatile CPU_INT32U PulseOut[8];       /* Output pulse Duty */
extern volatile CPU_INT32U PulseIn_period[8];

/*
 * Name			  :	void Timer1ISR_Handler(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */
void Timer1ISR_Handler (void)
{    
    CPU_INT32U Reg_T1IR;
    CPU_INT32U Reg_T1CCR;
	CPU_INT08U t1_timeout_flag  = 0;

    Reg_T1IR  = LPC_TIM1->IR;                      /* T1 INT flag */
    Reg_T1CCR = LPC_TIM1->CCR;                     /* T1CCR value */
        
    if( Reg_T1IR & IR_MR0_INT_FLAG )               /* T1MR3 match event */
    {
        /* write 1 clear the flag */
        LPC_TIM1->IR = IR_MR0_INT_FLAG;            /* clear MR3 match interrupt flag in time for nested */                     
		
		if( (PulseOut[TIM1_OUT_R1_CHANNEL] != 0) && (LPC_TIM1->EMR != 0)){
			LPC_TIM1->EMR  |= ( 1 << 1 );          /* mat output high, */
			LPC_TIM1->MR1 = PulseOut[TIM1_OUT_R1_CHANNEL]*25/18 + LPC_TIM1->TC; /* set the T1MR1 value */	
		}
		
		t1_timeout_flag = 1;
       
		period_cap_T1CR0_overflow_count ++;
		period_cap_T1CR1_overflow_count ++;

		/* CAP PART */
        if( T1CAP0_Flag )
        {
			T1CAP0_OT++;                   /* MR3 over timers  */   
            if( T1CAP0_OT > 5 )            /* CAP0CR0 no input */
            { 
                T1CAP0_Flag = 0;
            	PulseIn[TIM1_IN_C0_CHANNEL] = 0;                

				LPC_TIM1->IR  =  IR_CR0_INT_FLAG;     /* clear the CR0 interrupt */
                Reg_T1IR     &= ~IR_CR0_INT_FLAG;
       
                Reg_T1CCR &= ~( 3 );       /* clear the trigger edge */
                Reg_T1CCR |=    5;         /* enable the cpature interrupt and rasing edge */       
            }
        }
        if( T1CAP1_Flag )
        {		
			T1CAP1_OT++;                   /* CAP0CR1 over timers */  
            if( T1CAP1_OT > 5 )            /* CAP0CR1 no input */
            { 
                T1CAP1_Flag = 0;

             	PulseIn[TIM1_IN_C1_CHANNEL] = 0;  

                LPC_TIM1->IR  =   IR_CR1_INT_FLAG;      /* clear the CR1 interrupt */
                Reg_T1IR     &=  ~IR_CR1_INT_FLAG; 
            
                Reg_T1CCR &= ~( 3<<3 );    /* clear the trigger edge */
                Reg_T1CCR |=  ( 5<<3 );    /* enable the cpature interrupt and rasing edge */ 
            }
        }/* MATCH PART */   
    	
		// 没有输入脉冲 周期设为 0
		if( period_cap_T1CR0_overflow_count > 20)
		{
			PulseIn_period[TIM1_IN_C0_CHANNEL] = 0; 	
		}

		if( period_cap_T1CR1_overflow_count > 20)
		{
			PulseIn_period[TIM1_IN_C1_CHANNEL] = 0;
		}
	}
    
    if( Reg_T1IR & IR_CR0_INT_FLAG )      /* CAP1.0 */
    {
        LPC_TIM1->IR = IR_CR0_INT_FLAG;   /* clear CR0 interrupt flag in time for nested */         
                
        if( Reg_T1CCR & 0x00000001 )      /* caculate period */
        {  
            Last_T1CR0 = LPC_TIM1->CR0;	  /* register rising edge value */
			T1CAP0_OT  =0;				  /* clear the overtimers */

			if( ( t1_timeout_flag == 1) &&	 // 如果上跳沿和定时器到时中断一起发生的处理
				( LPC_TIM1->CR0 > 2000) )
			{
				T1CAP0_OT++;	
				period_cap_T1CR0_overflow_count --;
			}
			
			if( !T1CAP0_Flag )			  /* the first edge */
            {
                T1CAP0_Flag = 1;
            }
			
			/*add for period cap*/
			PulseIn_period[TIM1_IN_C0_CHANNEL] = cycle_timer1 *period_cap_T1CR0_overflow_count + Last_T1CR0 - last_rising_edge_T1CR0;
			last_rising_edge_T1CR0 = Last_T1CR0;
			
			if( ( t1_timeout_flag == 1) &&	 // 如果上跳沿和定时器到时中断一起发生的处理
				( LPC_TIM1->CR0 > 2000) )
			{
				period_cap_T1CR0_overflow_count = 1;	
			
			}else
			{
				period_cap_T1CR0_overflow_count = 0;
			}
			/*end add*/      

        }else if( Reg_T1CCR & 0x00000002 )
        {  
			if( ( t1_timeout_flag == 1) &&
				( LPC_TIM1->CR0 > 2000) )
			{
				T1CAP0_OT--;	
			}
            PulseIn[TIM1_IN_C0_CHANNEL] = (cycle_timer1 *T1CAP0_OT + LPC_TIM1->CR0 - Last_T1CR0) * 18/25;  
			T1CAP0_OT   =0;				  /* clear the overtimers */ 
		    T1CAP0_Flag = 0;
        }
        Reg_T1CCR ^= 0x00000003;     /* change CR0 Triggle edge */    
    }

    if( Reg_T1IR & IR_CR1_INT_FLAG )         /* CAP1.1 */
    {
        LPC_TIM1->IR = IR_CR1_INT_FLAG;             /* clear CR1 interrupt flag in time for nested */ 
       
        if( Reg_T1CCR & 0x00000008 ) /* caculate period */
        {
            Last_T1CR1 = LPC_TIM1->CR1;	    /* register rising edge value */
			T1CAP1_OT  = 0;		    /* clear the overtimers */
			
			if( ( t1_timeout_flag == 1) &&
				( LPC_TIM1->CR1 > 2000) )
			{
				T1CAP1_OT++;
				period_cap_T1CR1_overflow_count --;	
			}

			if( !T1CAP1_Flag )		/* the first edging */
            {             
                T1CAP1_Flag = 1;
            }
			
			PulseIn_period[TIM1_IN_C1_CHANNEL] = cycle_timer1 *period_cap_T1CR1_overflow_count + Last_T1CR1 - last_rising_edge_T1CR1;
			last_rising_edge_T1CR1 = Last_T1CR1;			


			if( ( t1_timeout_flag == 1) &&
				( LPC_TIM1->CR1 > 2000) )
			{
				period_cap_T1CR1_overflow_count = 1;	
			}else
			{
				period_cap_T1CR1_overflow_count = 0;	
			}
			      
        }else if( Reg_T1CCR & 0x00000010 )
        {
			if( ( t1_timeout_flag == 1) &&
				( LPC_TIM1->CR1 > 2000) )
			{
				T1CAP1_OT--;	
			}

            PulseIn[TIM1_IN_C1_CHANNEL] =(cycle_timer1 *T1CAP1_OT + LPC_TIM1->CR1 - Last_T1CR1)  * 18/25; 
			T1CAP1_OT   =0;		    /* clear the overtimers */  
        	T1CAP1_Flag = 0;
		} 
        Reg_T1CCR ^= 0x00000018;     /* change CR1 Triggle edge */
    }
	 
    LPC_TIM1->CCR = Reg_T1CCR;               /* CAP Control Register */
}

/*
 * Name			  :	void TIMER1_Init(CPU_INT32U cycle);
 * Input          : none
 * Output         : none
 * Caller(s)      : application.
 * Return         : none
 * Notes          :	none.
 *
 */
void TIMER1_Init( INT32U cycle )
{
	cycle_timer1 = cycle;	 

    PerClkEn(PER_NBR_TMR1);	 /* enable timer1 clock source */   
	
	GPIO_Cfg(GPIO_PORT1_FAST,TIMER1_CAP0_PIN | TIMER1_CAP1_PIN,GPIO_OPT_FNCT_4);	 /* p1.18,p1.19 FUNT_4 -- cap1.0,cap1.1 */
	GPIO_Cfg(GPIO_PORT1_FAST,TIMER1_MAT1_PIN,GPIO_OPT_FNCT_4);                       /* p1.25 FUNT_4 -- MAT1.1 */

    /* module initialize */
    LPC_TIM1->TCR = TCR_CNT_RESET;      /* Reset the counter */
    LPC_TIM1->PR  = PR_VALUE_0;         /* = Pclock/1 */
    
	LPC_TIM1->MCR = (MCR_MR0_INT_EN | MCR_MR0_RESET_EN); /* when T1MR0 match,reset and interrupt */
	LPC_TIM1->MR0 = cycle;              /* T1MR0 control the period */
	LPC_TIM1->MR1 = cycle/2;            /* T1MR1 control MAT1.1 pulse width */ 
	LPC_TIM1->EMR = 0;                  /* MAT default output low */

	/* cap1.0 cap1.1 ,rising edge and interrupt enable */
	LPC_TIM1->CCR = (TCCR_CAP0_RE_EN|TCCR_CAP0_INT_EN) | (TCCR_CAP1_RE_EN|TCCR_CAP1_INT_EN);  
    
	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR1,(CPU_FNCT_VOID)Timer1ISR_Handler);
	//BSP_IntPrioSet();
	BSP_IntEn(BSP_INT_SRC_NBR_TMR1);
}

/*
 * Name			  :	void MAT1_Start( void );
 * Input          : none
 * Output         : none
 * Caller(s)      : application.
 * Return         : none
 * Notes          :	none.
 *
 */
void MAT1_Start( void )
{
    LPC_TIM1->TCR = 1; /* Enable the counter */
}

void MAT1_Stop(void)
{
	LPC_TIM1->TCR = 0;
}

void TIMER1_set_frequent( CPU_INT32U cycle )
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();

	MAT1_Stop();
	LPC_TIM1->TCR = 2;
	cycle_timer1 = cycle;      // will be used for  cap calculate 
   	LPC_TIM1->MR0 = cycle;     /* T1MR0 control the period */
	MAT1_Start();

	OS_EXIT_CRITICAL();
}

void TIMER1_enable_pwmout(void)
{
	 CPU_SR_ALLOC();

	 OS_ENTER_CRITICAL();

	 LPC_TIM1->EMR = EMR_EMC1_CLR;

	 OS_EXIT_CRITICAL();
}

void TIMER1_disable_pwmout(void)
{
	 CPU_SR_ALLOC();

	 OS_ENTER_CRITICAL();

	 LPC_TIM1->EMR = 0;

	 OS_EXIT_CRITICAL();
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
