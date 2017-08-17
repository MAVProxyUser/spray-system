/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_timer1.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-23 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */				 

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"
#include "..\..\hmi\HMI_inc.h"
#include "../../dsp/fir/fir.h"

/* Exported variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/*
 * Name			  :	void Timer0ISR_Handler(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : ISR.
 * Return         : none
 * Notes          :	ISR.
 */

void Timer0ISR_Handler (void)
{    
    CPU_INT32U Reg_T0IR;

    Reg_T0IR  = LPC_TIM0->IR;                      /* T0 INT flag */
        
    if( Reg_T0IR & IR_MR0_INT_FLAG )               /* T0MR0 match event */
    {
        /* write 1 clear the flag */
        LPC_TIM0->IR = IR_MR0_INT_FLAG;            /* clear MR0 match interrupt flag in time for nested */                     
		
		/* Ò¡¸Ë²ÉÑù,300HZ*/
		STICK_RING_BUF_WR(stick_ring_buf_ail,ADC0_GetData(ADC_CH_AIL));
		STICK_RING_BUF_WR(stick_ring_buf_ele,ADC0_GetData(ADC_CH_ELE));
		STICK_RING_BUF_WR(stick_ring_buf_thr,ADC0_GetData(ADC_CH_THR));
		STICK_RING_BUF_WR(stick_ring_buf_rud,ADC0_GetData(ADC_CH_RUD));
//        uart_printf(0,"%d\r\n",ADC0_GetData(ADC_CH_AIL));

//	   cur_ail = (__FIR_ASM(fir_stick_cof, stick_ring_buf_ail.pBuf, stick_ring_buf_ail.wrIdx&stick_ring_buf_ail.mask) + 16384)/32768;
//       uart_printf(0,"%d\r\n",cur_ail);

//		buzzer_song_handler();
//		
//	   {
//			//WDT_Feed();            /* feed watchdog */
//
//		}
	}		
}

/*
 * Name			  :	void TIMER0_Init(CPU_INT32U cycle);
 * Input          : none
 * Output         : none
 * Caller(s)      : application.
 * Return         : none
 * Notes          :	none.
 *
 */
void TIMER0_Init( CPU_INT32U cycle )
{
    PerClkEn(PER_NBR_TMR0);	       /* enable timer0 clock source */   
	
    /* module initialize */
    LPC_TIM0->TCR = TCR_CNT_RESET; /* Reset the counter */
    LPC_TIM0->PR  = PR_VALUE_0;    /* = Pclock/1        */
    
	LPC_TIM0->MCR = (MCR_MR0_INT_EN | MCR_MR0_RESET_EN); /* when T1MR0 match,reset and interrupt */
	LPC_TIM0->MR0 = T_SYS_VALUE / cycle;                 /* T0MR0 control the period */

	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR0,(CPU_FNCT_VOID)Timer0ISR_Handler);
	//BSP_IntPrioSet((CPU_INT08U)BSP_INT_SRC_NBR_TMR0,0); //same level as DMA
	BSP_IntEn(BSP_INT_SRC_NBR_TMR0);
}

/*
 * Name			  :	void TIMER0_EN( void );
 * Input          : none
 * Output         : none
 * Caller(s)      : application.
 * Return         : none
 * Notes          :	none.
 *
 */
void timer0_start ( void )
{
    LPC_TIM0->TCR = 1; /* Enable the counter */
}

void timer0_stop ( void )
{
	LPC_TIM0->TCR = 0;
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
