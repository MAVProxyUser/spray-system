/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_timer3.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* Exported variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static CPU_INT32U cycle_timer3 = 0;

static CPU_INT32U Last_T3CR0  = 0;
static CPU_INT08U T3CAP0_Flag = 0;        /* CR0 the first rising edging */
static volatile CPU_INT32U T3CAP0_OT = 0; /* T3CAP0 over times */            

static CPU_INT32U Last_T3CR1  = 0;
static CPU_INT08U T3CAP1_Flag = 0;        /* CR1 the first rising edging */
static volatile CPU_INT32U T3CAP1_OT = 0; /* T3CAP0 over times */

// ÖÜÆÚ²âÁ¿
static CPU_INT32U last_rising_edge_T3CR0 = 0;
static CPU_INT32U last_rising_edge_T3CR1 = 0;
static CPU_INT08U period_cap_T3CR0_overflow_count = 0;
static CPU_INT08U period_cap_T3CR1_overflow_count = 0;

/* Imported variables --------------------------------------------------------*/


/*
 * Name			  :	void Timer3ISR_Handler(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */
void Timer3ISR_Handler (void)
{			                        
	CPU_INT32U Reg_T3IR;
    CPU_INT32U Reg_T3CCR;

	CPU_INT08U t3_timeout_flag = 0;

    Reg_T3IR  = LPC_TIM3->IR;                     /* get the interrupt flag */
    Reg_T3CCR = LPC_TIM3->CCR;                    /* get the T3CCR value */
    
    if( Reg_T3IR & IR_MR0_INT_FLAG )             /* T1MR3 match event */
    {
        /* write 1 clear the flag */
        LPC_TIM3->IR = IR_MR0_INT_FLAG;                 /* clear MR3 match interrupt flag in time for nested */      
		
		if( (PulseOut[TIM3_OUT_R1_CHANNEL] != 0) && (LPC_TIM3->EMR != 0))
		{
			LPC_TIM3->EMR  |= ( 1 << 1 );       /* mat output high,MAT3.1 */
			LPC_TIM3->MR1 = PulseOut[TIM3_OUT_R1_CHANNEL] *25/18 + LPC_TIM3->TC;/* set the T3MR0 value */	
		}
		t3_timeout_flag = 1;
		
		period_cap_T3CR0_overflow_count ++;
		period_cap_T3CR1_overflow_count ++;
			
		if( T3CAP0_Flag )
        {            
            T3CAP0_OT++;                 /* CAP0 over timers  */   
            if( T3CAP0_OT > 5 )          /* CAP0CR0 no input */
            { 
                T3CAP0_Flag = 0;
                
				PulseIn[ TIM3_IN_C0_CHANNEL ] = 0;

                LPC_TIM3->IR =   IR_CR0_INT_FLAG;   /* clear the CR0 interrupt */
                Reg_T3IR     &= ~IR_CR0_INT_FLAG;

                Reg_T3CCR &= ~( 3 );     /* clear the trigger edge */ 
                Reg_T3CCR |=    5;       /* enable the cpature interrupt and rasing edge */       
            }
        }
        if( T3CAP1_Flag )
        { 
			T3CAP1_OT++;               /* CAP0CR1 over timers */  
            if( T3CAP1_OT > 5 )        /* CAP0CR1 no input */
            { 
                T3CAP1_Flag = 0;
                
             	PulseIn[ TIM3_IN_C1_CHANNEL ] = 0;

                LPC_TIM3->IR   =  IR_CR1_INT_FLAG;  /* clear the CR1 interrupt */
                Reg_T3IR      &= ~IR_CR1_INT_FLAG; 
                Reg_T3CCR &= ~( 3<<3 ); /* clear the trigger edge */
                Reg_T3CCR |=  ( 5<<3 ); /* enable the cpature interrupt and rasing edge */ 
            }/* MATCH PART */ 
        }

		if( period_cap_T3CR0_overflow_count > 20)
		{
			PulseIn_period[TIM3_IN_C0_CHANNEL] = 0;	
		}

		if( period_cap_T3CR1_overflow_count	> 20)
		{
		   PulseIn_period[TIM3_IN_C1_CHANNEL] = 0;	
		}
    }
    
    if( Reg_T3IR & IR_CR0_INT_FLAG )         /* CAP3.0 */
    {       
        LPC_TIM3->IR = IR_CR0_INT_FLAG;             /* clear CR0 interrupt flag in time for nested */ 
       
	    if( Reg_T3CCR & 0x00000001 ) /* caculate period */
        {
            Last_T3CR0 = LPC_TIM3->CR0;		/* register rising edge value */
			T3CAP0_OT  = 0;			/* clear the overtimers */
			
			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR0 > 2000) )
			{
				T3CAP0_OT++;	
				period_cap_T3CR0_overflow_count --;
			}
						
			if( !T3CAP0_Flag )       /* the first edge */
            {
                T3CAP0_Flag = 1;
            }
			
			PulseIn_period[TIM3_IN_C0_CHANNEL] = cycle_timer3 * period_cap_T3CR0_overflow_count + Last_T3CR0 - last_rising_edge_T3CR0;
			last_rising_edge_T3CR0 = Last_T3CR0;
			
			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR0 > 2000) )
			{
				period_cap_T3CR0_overflow_count = 1;
			}else
			{
				period_cap_T3CR0_overflow_count = 0;
			}      
        }else if( Reg_T3CCR & 0x00000002 )
        {
			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR0 > 2000) )
			{
				T3CAP0_OT--;	
			}
			
		    PulseIn[TIM3_IN_C0_CHANNEL] =  (cycle_timer3*T3CAP0_OT + LPC_TIM3->CR0 - Last_T3CR0) * 18/25;
			
			T3CAP0_OT   = 0;		/* clear the overtimers */
			T3CAP0_Flag = 0;
			
        } 
        Reg_T3CCR ^= 0x00000003;    /* change CR0 Triggle edge */       
    }
   
    if( Reg_T3IR & IR_CR1_INT_FLAG )        /* CAP3.1 */
    {
        LPC_TIM3->IR = IR_CR1_INT_FLAG;     /* clear CR1 interrupt flag in time for nested */ 
               
        if( Reg_T3CCR & 0x00000008 )        /* caculate period */
        {
            Last_T3CR1 = LPC_TIM3->CR1;		/* register rising edge value */
			T3CAP1_OT  = 0;			        /* clear the overtimers */
			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR1 > 2000) )
			{
				T3CAP1_OT++;
				period_cap_T3CR1_overflow_count --;	
			}
			if( !T3CAP1_Flag )       /* the first edging */
            {
                T3CAP1_Flag = 1;
            }
			
			PulseIn_period[TIM3_IN_C1_CHANNEL] = cycle_timer3 * period_cap_T3CR1_overflow_count + Last_T3CR1 - last_rising_edge_T3CR1;
			last_rising_edge_T3CR1 = Last_T3CR1;

			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR1 > 2000) )
			{
				period_cap_T3CR1_overflow_count = 1;	
			}else
			{
				period_cap_T3CR1_overflow_count = 0;
			}      
        }else if( Reg_T3CCR & 0x00000010 )
        {
			if( ( t3_timeout_flag == 1) &&
				( LPC_TIM3->CR1 > 1000) )
			{
				T3CAP1_OT--;	
			}

			PulseIn[ TIM3_IN_C1_CHANNEL ]  = (cycle_timer3*T3CAP1_OT + LPC_TIM3->CR1 - Last_T3CR1)  * 18/25;
			
			T3CAP1_OT   = 0;	    /* clear the overtimers */ 
			T3CAP1_Flag = 0;
			
        }
        Reg_T3CCR ^= 0x00000018;    /* change CR1 Triggle edge */
    } 
    LPC_TIM3->CCR = Reg_T3CCR;      /* CAP Control Register */ 
}

/*
 * Name			  :	void TIMER3_Init(CPU_INT32U cycle);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */
void TIMER3_Init( CPU_INT32U cycle )
{
	cycle_timer3 = cycle;

    PerClkEn(PER_NBR_TMR3);	 /* enable timer3 clock source */ 
    
	GPIO_Cfg(GPIO_PORT0_FAST,TIMER3_CAP0_PIN | TIMER3_CAP1_PIN,GPIO_OPT_FNCT_4);	 /* p0.23,p0.24 FUNT_4 -- cap3.0,cap3.1 */
	GPIO_Cfg(GPIO_PORT0_FAST,TIMER3_MAT1_PIN,GPIO_OPT_FNCT_4);						 /* p0.11 FUNT_4 -- MAT3.1 */

    /* module initialize */
    LPC_TIM3->TCR = TCR_CNT_RESET;  /* Reset the counter */
    LPC_TIM3->PR  = PR_VALUE_0;     /* = Pclock/1 */ 

    /* control register set */
    LPC_TIM3->MCR = (MCR_MR0_INT_EN | MCR_MR0_RESET_EN); /* when T3MR0 match,reset and interrupt */
    
	LPC_TIM3->MR0 = cycle;    /* T3MR0 control the period */
	LPC_TIM3->MR1 = cycle/2;
	LPC_TIM3->EMR = 0;        /* MAT default output low */

	LPC_TIM3->CCR = (TCCR_CAP0_RE_EN|TCCR_CAP0_INT_EN) | (TCCR_CAP1_RE_EN|TCCR_CAP1_INT_EN);/* rising edge,enable interrupt cap3.0 cap3.1*/  
    

	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR3,(CPU_FNCT_VOID)Timer3ISR_Handler);
	//BSP_IntPrioSet();
	BSP_IntEn(BSP_INT_SRC_NBR_TMR3);
}

/*
 * Name			  :	void MAT3_Start(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */
void MAT3_Start( void )
{
    LPC_TIM3->TCR = 1; /* Enable the counter */
}

void MAT3_Stop( void )
{
	LPC_TIM3->TCR = 0;
}

void TIMER3_set_frequent( CPU_INT32U cycle )
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();


	MAT3_Stop();
	LPC_TIM3->TCR = 2;
	cycle_timer3 = cycle;      // will be used for  cap calculate 
   	LPC_TIM3->MR0 = cycle;     /* T3MR0 control the period */
	MAT3_Start();

	OS_EXIT_CRITICAL();
}

void TIMER3_enable_pwmout(void)
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();

	LPC_TIM3->EMR = EMR_EMC1_CLR; 	/* mat output high,MAT3.1 */

	OS_EXIT_CRITICAL();
}

void TIMER3_disable_pwmout(void)
{
	 CPU_SR_ALLOC();

	 OS_ENTER_CRITICAL();

	 LPC_TIM3->EMR =  0;

	 OS_EXIT_CRITICAL();
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
