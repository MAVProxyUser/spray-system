/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       £ºReciever.uvproj
  * @File Name          : lpc17xx_pwm1.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-12-21 
  * @Version            : 2.00
  ******************************************************************************
  * @Description
  *	    lpc17xx timer and pwm module        
  */

/* Includes ------------------------------------------------------------------*/
#include "../drivers.h"


/* Exported variables --------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static CPU_INT32U   cycle_pwm1 = 0;

/* Imported variables --------------------------------------------------------*/


/*
 * Function name  : PWM1ISR_Handler
 * Descriptions   : PWM1 interrupt handler For now 
 * parameters     : None
 * Returned value : None
 */
void PWM1ISR_Handler (void) 
{
    CPU_INT32U Reg_PWM1IR;
					 
    Reg_PWM1IR  = LPC_PWM1->IR;                  /* get the interrupt flag */
	
	//uart_printf(0,"h1!\r\n");
    if( Reg_PWM1IR & IR_MR0_INT_FLAG )           /* PWM1MR0 match event */
    {
        /* write 1 clear the flag */
        LPC_PWM1->IR = IR_MR0_INT_FLAG;          /* clear MR3 match interrupt flag in time for nested */
		
//		LPC_PWM1->MR1 = pulse_out[0];	 /* set PWM duty  */
//		LPC_PWM1->MR2 = pulse_out[1]; 
//		LPC_PWM1->MR3 = pulse_out[2];
//		LPC_PWM1->MR4 = pulse_out[3]; 
//		LPC_PWM1->MR5 = pulse_out[4]; 
//		LPC_PWM1->LER = LER_MR1_LATCH | LER_MR2_LATCH \
//		              | LER_MR3_LATCH | LER_MR4_LATCH | LER_MR5_LATCH;              
    }
}

/*
 *  Function name : PWM_Init
 *  Descriptions  : PWM initialization, setup all GPIOs to PWM0~6,reset counter, 
 *                  all latches are enabled,interrupt on PWMMR0, install PWM interrupt to 
 *                  the VIC table.			
 *  parameters    :	Duty cycle
 *  Returned value: true or fase, if VIC table is full, return false
 */
void PWM1_Init( CPU_INT32U cycle, CPU_INT32U duty)
{   
	cycle_pwm1 = cycle;

    PerClkEn(PER_NBR_PWM1);	       /* enable PWM1 clock source */ 
    
	//GPIO_Cfg(GPIO_PORT2_FAST,PWM1_MAT3_PIN,GPIO_OPT_FNCT_2);  /* p2.3 FUNT_2 -- PWM1.3 */
	GPIO_Cfg(GPIO_PORT3_FAST,DEF_BIT_26,GPIO_OPT_FNCT_4);  /* p3.26 FUNT_4 -- PWM1.3 */
	   
    LPC_PWM1->TCR = TCR_CNT_RESET; /* Counter Reset */ 	
    LPC_PWM1->PR  = PR_VALUE_0;    /* count frequency:Fpclk */
    				
    /* match set */
    //LPC_PWM1->MCR  =  MCR_MR0_INT_EN | MCR_MR0_RESET_EN;
	LPC_PWM1->MCR = MCR_MR0_RESET_EN;
      
    LPC_PWM1->MR0 = cycle;         /* set PWM cycle */
	LPC_PWM1->MR3 = duty;	       /* set PWM duty  */
    LPC_PWM1->LER = LER_MR0_LATCH | LER_MR3_LATCH ;   	
	
	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_PWM1,(CPU_FNCT_VOID)PWM1ISR_Handler);
	//BSP_IntPrioSet();
	BSP_IntEn(BSP_INT_SRC_NBR_PWM1);

	//PWM1_Start();	 
}

/*
 * Function name : PWM_Start
 * Descriptions  : Enable PWM by setting the PCR, PTCR registers
 * parameters    : channel number
 * Returned value: None
 */
void PWM1_Start( void )
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();

    LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;	/* counter enable, PWM enable */

	LPC_PWM1->PCR = PCR_PWM_EN3;
	OS_EXIT_CRITICAL();	
}

void PWM1_Stop( void )
{
	OS_CPU_SR  cpu_sr = 0;

	OS_ENTER_CRITICAL();	

	LPC_PWM1->TCR = 0;

	OS_EXIT_CRITICAL();
}

void PWM1_set_frequent( CPU_INT32U cycle)
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();

	PWM1_Stop();
	LPC_PWM1->TCR = 2;
	cycle_pwm1    = cycle;
	LPC_PWM1->MR0 = cycle;
	PWM1_Start();

	OS_EXIT_CRITICAL();
}

void PWM1_enable_pwmout( void )
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();
	LPC_PWM1->PCR = PCR_PWM_EN1 | PCR_PWM_EN2 | PCR_PWM_EN3 | PCR_PWM_EN4 | PCR_PWM_EN5 | PCR_PWM_EN6;
	OS_EXIT_CRITICAL();
}

void PWM1_disable_pwmout( void )
{
	CPU_SR_ALLOC();

	OS_ENTER_CRITICAL();
	LPC_PWM1->PCR = 0;
	OS_EXIT_CRITICAL();
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
