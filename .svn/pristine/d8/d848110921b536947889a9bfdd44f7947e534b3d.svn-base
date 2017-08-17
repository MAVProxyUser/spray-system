/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_clkpwr_ctl.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx clock and power control        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* Private define ------------------------------------------------------------*/

/* Maximum register timeout */
#define VAL_MAX_TO                         (CPU_INT16U)(0xFFFF)

/* PLL FEED MACRO */
#define PLL_FEED_SEQ(pll_nbr)              {    CPU_CRITICAL_ENTER();                             \
                                                                                                       \
                                                REG_PLLFEED(pll_nbr) = BIT_PLLFEED_VAL0;  \
                                                REG_PLLFEED(pll_nbr) = BIT_PLLFEED_VAL1;  \
                                                                                                       \
                                                CPU_CRITICAL_EXIT();                              \
                                           }


/*
 * Name			  :	void CPU_Init (void);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	CPU Clock Init,Power Control Set,APB Clock Set,Flash Accelerator config.
 *                  (1) The PLL0 Frequency is determined by:
 *
 *                       Fcco = (2 x M x Fin) / N
 *
 *                       where M   = PLL0 Multipler
 *                             N   = PLL0 Pre-dividier
 *                            Fin  = PLL0 Input Frequency (Main oscillator).
 *                                     
 *                  (2) PLL0 settings must meet the following:
 *                       Fin is in the range of 32Khz to 50 Mhz.
 *                       Fcco is in the range of 275 Mhz to 550 Mhz
 *
 *                  (3) The LPC17xx CPU frequency is determined by:
 *
 *                       CPU_freq = Fcc0 / CPU_Div                            
 *
 *                  (4) The USB clock frequency is determined by:
 *
 *                       USB_Clk = M x Fosc x P, or USB_clk = Fcco / (2 x P)
 *
 *                       Fcco    =  Fosc x 2 x M x 2 x P
 *
 *                       where Fcco = PLL1 output frequency.
 *                             M    = PLL1 multiplier.
 *                             P    = PLL1 pre-divider.
 *                             Fosc = Oscialltor  frequency.
 * 
 *                  (5) The PLL1 inputs and settings must meet the following criteria:
 *                       Fosc    is in the range of 10 Mhz to 25 Mhz.
 *                       USBClk  is 48 Mhz
 *                       Fcco    is in the range of 156 Mhz to 320 Mhz
 *
 *                  (6) In this example the LPC1768 operates:
 *
 *                       PLL0_Fcco = 2 x 25 x 12 / 2
 *                       PLL0_Fcco = 300mhz
 *
 *                       CPU_freq  =  300 Mhz  / 3 
 *                                 =  100 Mhz
 *
 *                       PLL1_Fcc0 = 12 x 4 x 2 x 2
 *                       USB_Clk   = 48 Mhz
*/
CPU_BOOLEAN  LPC17xx_System_Init (void)
{
    CPU_INT16U    reg_to;
    CPU_INT32U    reg_val;
    CPU_SR_ALLOC();
        
    /* MAIN OSCILLATOR INITIALIZATION */
    DEF_BIT_SET(REG_SCS, BIT_SCS_OSCRANGE);             /* Set the main oscillator range                        */
    
    
    reg_to = VAL_MAX_TO;
    
    DEF_BIT_SET(REG_SCS, BIT_SCS_OSCEN);                /* Enable the Main Oscillator                           */
    
                                                                /* Wait until the main oscillator is enabled.           */    
    while (DEF_BIT_IS_CLR(REG_SCS, BIT_SCS_OSCSTAT) &&
          (reg_to > 0)) {
        reg_to--;
    }
    
    if (reg_to == 0) {                                          /* Configuration fail                                   */
        return (DEF_FAIL);
    }

    /* Periphral clock must be selected 
	 * before PLL0 enabling and connecting,  
	 * according errata.lpc1765-8 February.2011
	 */
	REG_PCLKSEL0 = DEF_BIT_NONE;                        /* All peripheral clock runrs at CPU_Clk / 4 = 25 Mhz   */
    REG_PCLKSEL1 = DEF_BIT_NONE;    
    
	REG_FLASHCFG |= REG_FLASHCFG_CLK_5;                  /* Set 5 cycles to acces the Flash memory               */            
        
    /* PLL0 CONFIGURATION */
    reg_val  = ((25u - 1u) <<  0) & DEF_BIT_FIELD(15,  0)       /* PLL0 values M = 25 & N = 2 (see note #6)             */ 
             | (( 2u - 1u) << 16) & DEF_BIT_FIELD( 8, 16);    
                                                                /* 1. Disconnect PLL0 with one feed sequence if PLL ... */
                                                                /* ... already connected.                               */
    if (DEF_BIT_IS_SET(REG_PLLSTAT(0), BIT_PLLSTAT_PLLC0_STAT)) {        
        DEF_BIT_CLR(REG_PLLCTRL(0), BIT_PLLCTRL_PLLC);   
        PLL_FEED_SEQ(0);        
    }
    
    DEF_BIT_CLR(REG_PLLCTRL(0), BIT_PLLCTRL_PLLE);          /* 2. Disable PLL0 with one feed sequence               */
    PLL_FEED_SEQ(0);        
    
    REG_CCLKCFG   = (1u - 1u);                              /* 3. Change the CPU clock divider setting to speed ... */
                                                            /* ... operation without PLL0                           */

    REG_CLKSRCSEL = 0x01;                                   /* 4. Select the main osc. as the PLL0 clock source     */
    
    REG_PLLCFG(0) = reg_val;                                /* 5. Write to the PLLCFG and make it effective with... */
    PLL_FEED_SEQ(0);                                        /* ... one one feed sequence                            */
    
    DEF_BIT_SET(REG_PLLCTRL(0), BIT_PLLCTRL_PLLE);          /* 6. Enable PLL0 with one feed sequence                */
    PLL_FEED_SEQ(0);                                        

    REG_CCLKCFG   = (3u - 1u);                              /* 7. Change the CPU clock divider setting for ...      */
                                                                /* ... operation with PLL0                              */

    reg_to = VAL_MAX_TO;                                    /* 8. Wait for PLL0 to achieve lock by monitoring ...   */
                                                            /* ... the PLOCK0 bit in the PLL0STAT                   */
    while (DEF_BIT_IS_CLR(REG_PLLSTAT(0), BIT_PLLSTAT_PLOCK0) &&
          (reg_to > 0)) {
        reg_to--;
    }
           
    if (reg_to == 0) {
        return (DEF_FAIL);        
    }

    DEF_BIT_SET(REG_PLLCTRL(0), BIT_PLLCTRL_PLLC);          /* 9. Connect PLL0 with one feed sequence               */
    PLL_FEED_SEQ(0);                                            
                                                            /* ------------------ PLL1 CONFIGURATION -------------- */
    reg_val  = ((4u - 1u) <<  0) & DEF_BIT_FIELD(4, 0)      /* PLL1 values M = 4; P = 2 coded as '01' (see note #6) */ 
             | ((0x01   ) <<  5) & DEF_BIT_FIELD(2, 5);    

    /* PLL1 CONFIGURATION */
	DEF_BIT_CLR(REG_PLLCTRL(1), BIT_PLLCTRL_PLLC);          /* 1. Disconnect PLL1 with one feed sequence            */
    PLL_FEED_SEQ(1);        

    DEF_BIT_CLR(REG_PLLCTRL(1), BIT_PLLCTRL_PLLE);          /* 2. Disable PLL1 with one feed sequence               */
    PLL_FEED_SEQ(1);        

    REG_PLLCFG(1) = reg_val;                                /* 3. Write to the PLLCFG and make it effective with... */
    PLL_FEED_SEQ(1);                                        /* ... one one feed sequence                            */
    
    DEF_BIT_SET(REG_PLLCTRL(1), BIT_PLLCTRL_PLLE);          /* 4. Enable PLL1 with one feed sequence                */
    PLL_FEED_SEQ(1);                                        

    reg_to = VAL_MAX_TO;                                    /* 5. Wait for PLL1 to achieve lock by monitoring ...   */
                                                            /* ... the PLOCK1 bit in the PLL1STAT                   */
    while (DEF_BIT_IS_CLR(REG_PLLSTAT(1), BIT_PLLSTAT_PLOCK1) &&
          (reg_to > 0)) {
        reg_to--;
    }

    if (reg_to == 0) {
        return (DEF_FAIL);        
    }    

    
    DEF_BIT_SET(REG_PLLCTRL(1), BIT_PLLCTRL_PLLC);          /* 6. Connect PLL1 with one feed sequence               */
    PLL_FEED_SEQ(1);                                        
           

            
    return (DEF_OK);
}




/* Peripheral clock Control --------------------------------------------------*/

/*
 * Name			  :	void CPU_FreqGet (void);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	This function return the CPU clk frequency
 *
*/
CPU_INT32U CPU_FreqGet (void)
{
    CPU_INT32U  cpu_freq;
    CPU_INT08U  cpu_div;
    CPU_INT16U  pll_mul;
    CPU_INT08U  pll_div;
    CPU_INT32U  reg_val;


    reg_val =  REG_CLKSRCSEL & DEF_BIT_FIELD(2, 0);
 
    switch(reg_val) {
        case 0x00:
             cpu_freq =  OSC_INT_RC_FREQ_HZ;
             break;
    
        case 0x01:
             cpu_freq =  OSC_MAIN_OSC_FREQ_HZ;
             break;

        case 0x02:
             cpu_freq =  OSC_RTC_FREQ_HZ;
             break;
                  
        default:
             return (0);
    }
 
    pll_mul =   (( REG_PLLSTAT(0) &  BIT_PLLSTAT_PLL0_MSEL_MASK)         + 1);
    pll_div =   (((( REG_PLLSTAT(0) &  BIT_PLLSTAT_PLL0_PSEL_MASK)) >> 16) + 1);

    if (DEF_BIT_IS_SET( REG_PLLSTAT(0), ( BIT_PLLSTAT_PLLE0_STAT | 
                                          BIT_PLLSTAT_PLLC0_STAT |
                                          BIT_PLLSTAT_PLOCK0 ))) {
        cpu_freq = ((cpu_freq * 2 * pll_mul) / pll_div);
    }
        
    cpu_div =  REG_CCLKCFG & DEF_BIT_FIELD(8, 0); 
    cpu_div++;
    
    if (cpu_div != 0) {
        cpu_freq /= cpu_div;
    }
    
    return (cpu_freq);
}

/*
 * Name			  :	void PerClkDis (CPU_INT08U  per_nbr);
 * Input          : per_nbr    The peripheral identifier number (defined in 'lpc17xx_clkpwr_ctl.h')
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Disable peripheral clock.
 *
*/
void PerClkDis (CPU_INT08U  per_nbr)
{
    switch (per_nbr) {
        case  PER_NBR_RESERVED0:
        case  PER_NBR_RESERVED1:
        case  PER_NBR_RESERVED2:
        case  PER_NBR_RESERVED3:
        case  PER_NBR_RESERVED4:
             return ;
    
        default:
             break;
    }
    
    DEF_BIT_CLR( REG_PCONP, DEF_BIT(per_nbr));
}

/*
 * Name			  :	void PerClkEn (CPU_INT08U  per_nbr);
 * Input          : per_nbr    The peripheral identifier number (defined in 'lpc17xx_clkpwr_ctl.h')
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Enable peripheral clock.
 *
*/
void PerClkEn (CPU_INT08U  per_nbr)
{
    switch (per_nbr) {
        case  PER_NBR_RESERVED0:
        case  PER_NBR_RESERVED1:
        case  PER_NBR_RESERVED2:
        case  PER_NBR_RESERVED3:
        case  PER_NBR_RESERVED4:
             return;
    
        default:
             break;
    }
    
    DEF_BIT_SET( REG_PCONP, DEF_BIT(per_nbr));
}

/*
 * Name			  :	void PerClkFreqGet (CPU_INT08U  per_nbr);
 * Input          : per_nbr    The peripheral identifier number (defined in 'lpc17xx_clkpwr_ctl.h')
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Get peripheral clock frequency.
 *
*/
CPU_INT32U PerClkFreqGet  (CPU_INT08U  per_nbr)                                       
{
    CPU_INT32U  per_clk_freq;
    CPU_INT32U  per_clk_div = 0;
    CPU_INT32U  reg_val;
    
                                                                /* ----------------- ARGUMENTS CHECKING ------------- */
    switch (per_nbr) {
        case  PER_NBR_PCLKSEL_RESERVED0:
        case  PER_NBR_PCLKSEL_RESERVED1:
        case  PER_NBR_PCLKSEL_RESERVED2:        
             return (DEF_FAIL);
    
        default:
             break;
    }
    
    if (per_nbr <  PER_NBR_RIT) {
        reg_val = ( REG_PCLKSEL0 >> ((per_nbr      ) * 2u)) & DEF_BIT_FIELD(2, 0);
    } else {
        reg_val = ( REG_PCLKSEL1 >> ((per_nbr - 16u) * 2u)) & DEF_BIT_FIELD(2, 0);
    }
    
    switch (reg_val) {
        case 0x01:            
             per_clk_div = 1u;
             break;
             
        case 0x02:
             per_clk_div = 2u;
             break;
             
        case 0x00:            
             per_clk_div = 4u;
             break;

        case 0x03:            
             if ((per_nbr ==  PER_NBR_CAN1       ) ||
                 (per_nbr ==  PER_NBR_CAN2       ) ||
                 (per_nbr ==  PER_NBR_PCLKSEL_ACF)) {
                 per_clk_div = 6u;
             } else {
                 per_clk_div = 8u;
             }
             break;

        default:
             break;
    }
    
    per_clk_freq  =  CPU_FreqGet();
    per_clk_freq /= per_clk_div;
    
    return (per_clk_freq);
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/

