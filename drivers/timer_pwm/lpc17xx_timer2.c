/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_timer2.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-02-17 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */
/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"
#include "../../app/wireless/WL_inc.h"
/* Exported variables --------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/        
static  CPU_INT32U Last_T2CR1   = 0; 

/* Imported variables --------------------------------------------------------*/

/*
 * Name			  :	void Timer2ISR_Handler(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */
void Timer2ISR_Handler  ( void )
{			                        
    CPU_INT32U Reg_T2IR;
    CPU_INT32U Reg_T2CCR;
	
	Reg_T2IR  = LPC_TIM2->IR;               /* get the interrupt flag */
    Reg_T2CCR = LPC_TIM2->CCR;              /* get the T2CCR value    */ 		
	           
    if( Reg_T2IR & IR_CR1_INT_FLAG )        /* CAP2.1 */
    {
        LPC_TIM2->IR = IR_CR1_INT_FLAG;     /* clear CR1 interrupt flag in time for nested */    

        if( Reg_T2CCR & 0x00000008 )        /* caculate period */
        {
            Last_T2CR1 = LPC_TIM2->CR1;		/* register rising edge value */				      
        }else if( Reg_T2CCR & 0x00000010 )
        {
			WL_LT2510_RSSI_value = (LPC_TIM2->CR1 - Last_T2CR1)&0xFFFFFFFF; /* 定时器是32位自然溢出 */
			WL_LT2510_link_cnt   = 0;				/* clear link counter */      
        }        
        Reg_T2CCR ^= 0x00000018;            /* change CR1 Triggle edge */
    } /*end if( RegT2IR & CR1_INT )*/
    LPC_TIM2->CCR = Reg_T2CCR;              /* CAP Control Register */    
}

/*
 * Name			  :	void TIMER2_Init(CPU_INT32U cycle);
 * Input          : none
 * Output         : none
 * Caller(s)      : application.
 * Return         : none
 * Notes          :	none.
 *
 */
void TIMER2_Init( void )
{
	PerClkEn(PER_NBR_TMR2);	        /* enable timer2 clock source */ 
    
	GPIO_Cfg(GPIO_PORT0_FAST,TIMER2_CAP1_PIN,GPIO_OPT_FNCT_4);	                     /* p0.4,p0.5   FUNT_4 -- cap2.0,cap2.1 */

    /* module initialize */
    LPC_TIM2->TCR = TCR_CNT_RESET;  /* Reset the counter */
    LPC_TIM2->PR  = PR_VALUE_0;     /* = Pclock/1 */
	LPC_TIM2->IR  = 0x3F;			/* clear interrupt flag */

    /* control register set */
	LPC_TIM2->CCR = (TCCR_CAP1_RE_EN|TCCR_CAP1_INT_EN);/* rising edge,enable interrupt cap2.1*/  
    
    BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR2,(CPU_FNCT_VOID)Timer2ISR_Handler);
	//BSP_IntPrioSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR2,1);
	BSP_IntEn(BSP_INT_SRC_NBR_TMR2);
}

/**
**************************************************************************
** Function name: MAT_Start
**
** Descriptions: Enable PWM by setting the PCR, PTCR registers
**
** parameters: channel number
** Returned value: None
** 
**************************************************************************
**/
void MAT2_Start( void )
{
    LPC_TIM2->TCR = 1; /* Enable the counter */
}

void MAT2_Stop( void )
{
	LPC_TIM2->TCR = 0;
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
