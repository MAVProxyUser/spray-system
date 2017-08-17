/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_clkpwr_ctl.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx clock and power control        
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPC17XX_CLKPWR_CTL_H__
#define __LPC17XX_CLKPWR_CTL_H__

/* Exported constants --------------------------------------------------------*/

/* Clock Source Defines */
#define  OSC_MAIN_OSC_FREQ_HZ               12000000L
#define  OSC_INT_RC_FREQ_HZ                  4000000L
#define  OSC_RTC_FREQ_HZ                        32768

/* CPU Power Modes */
#define  CPU_MODE_NORMAL                            0
#define  CPU_MODE_PWR_DOWN                          1
#define  CPU_MODE_IDLE                              2
#define  CPU_MODE_SLEEP                             3

/* Peripheral Number in PCONP and PCLKSEL */
/*
 * Note(s) : (1) Some peripheral bit numbers doesn't match between the Power Coontrol for periphreal (PCONP) 
 *               register and the Peripheral Clock Selection register (PCLKSEL0/1)
 *
 *               For example the bit 15 in the PCONP register is for GPIO clock, but in the PCLKSEL0 register bits 30:31 
 *               (uses two bit per peripheral) is for CAN acceptance filtering (ACF). 
 *
 *               To solve this inconsistency SYS_CTRL_PER_NBR_GPIO and  SYS_CTRL_PER_NBR_PCLKSEL_ACF are defined
 *               to the same value. 
 *
 *               - SYS_CTRL_PER_NBR_GPIO  should be used with SysCtrl_PerClkDis() and SysCtrl_PerClkEn() 
 *                 functions.
 *
 *               - SYS_CTRL_PER_NBR_PCLKSEL_ACF should be used with SysCtrl_PerClkFreqCfg() and 
 *                 SysCtrl_PerClkFreqCfg() functions.
 */
#define  PER_NBR_WDT                                0   /* Watchdog Timer                                     */
#define  PER_NBR_RESERVED0                          0   /* Reserved                                           */

#define  PER_NBR_TMR0                               1   /* Timer 0                                            */
#define  PER_NBR_TMR1                               2   /* Timer 1                                            */
#define  PER_NBR_UART0                              3   /* UART 0                                             */
#define  PER_NBR_UART1                              4   /* UART 1                                             */

#define  PER_NBR_RESERVED1                          5   /* Reserved                                           */
#define  PER_NBR_PCLKSEL_RESERVED0                  5   /* Reserved (Peripheral Clock Selection)              */

#define  PER_NBR_PWM1                               6   /* PWM1                                               */
#define  PER_NBR_I2C0                               7   /* I2C 0                                              */
#define  PER_NBR_SPI0                               8   /* SPI 0                                              */

#define  PER_NBR_RTC                                9   /* RTC                                                */
#define  PER_NBR_PCLKSEL_RESERVED1                  9   /* Reserved (Peripheral Clock Selection)              */

#define  PER_NBR_SSP1                              10   /* SSP 1                                              */
#define  PER_NBR_RESERVED2                         11   /* Reserved                                           */
#define  PER_NBR_AD0                               12   /* A/D converter                                      */
#define  PER_NBR_CAN1                              13   /* CAN 1                                              */
#define  PER_NBR_CAN2                              14   /* CAN 2                                              */

#define  PER_NBR_GPIO                              15   /* GPIO                                               */
#define  PER_NBR_PCLKSEL_ACF                       15   /* CAN filtering. (Peripheral Clock Selection)        */

#define  PER_NBR_RIT                               16   /* Repetitive Interrupt Timer                         */
#define  PER_NBR_PCLKSEL_QEI                       16   /* QEI (Peripheral Clock Selection)                   */

#define  PER_NBR_MCPWM                             17   /* Motor Control PWM                                  */
#define  PER_NBR_PCLKSEL_GPIOINT                   17   /* QEI (Peripheral Clock Selection)                   */

#define  PER_NBR_QEI                               18   /* Quadrature Encoder Interface                       */
#define  PER_NBR_PCLKSEL_PCB                       18   /* Pin Connect Block (Peripheral Clock Selection)     */

#define  PER_NBR_I2C1                              19   /* I2C1                                               */

#define  PER_NBR_RESERVED3                         20   /* Reserved                                           */
#define  PER_NBR_PCLKSEL_RESERVED2                 20   /* Reserved (Peripheral Clock Selection)              */

#define  PER_NBR_SSP0                              21   /* SSP0                                               */
#define  PER_NBR_TMR2                              22   /* TIMER 2                                            */
#define  PER_NBR_TMR3                              23   /* TIMER 3                                            */
#define  PER_NBR_UART2                             24   /* UART 2                                             */
#define  PER_NBR_UART3                             25   /* UART 3                                             */
#define  PER_NBR_I2C2                              26   /* I2C 2                                              */
#define  PER_NBR_I2S                               27   /* I2S                                                */
#define  PER_NBR_RESERVED4                         28   /* Reserved                                           */
#define  PER_NBR_PCLKSEL_RESERVED3                 28   /* Reserved (Peripheral Clock Selection)              */


#define  PER_NBR_GPDMA                             29   /* GP DMA function                                    */
#define  PER_NBR_PCLKSEL_RIT                       29   /* RIT (Peripheral Clock Selection)                   */

#define  PER_NBR_ENET                              30   /* Ethernet Controller                                */
#define  PER_NBR_PCLKSEL_SYSCON                    30   /* Sytem Control Block (Peripheral Clock Selection)   */

#define  PER_NBR_USB                               31   /* USB                                                */
#define  PER_NBR_PCLKSEL_MCPWM                     31   /* Motor Control PWM (Peripheral Clock Selection)     */

/* SYSTEM CONTROL AND STATUS REGISTER & BIT DEFINES --------------------------*/
/* SYSTEM CONTROL REGISTER BASE ADDRESS DEFINES */
#define  REG_SCS                             (*(CPU_REG32 *)(0x400FC1A0))

#define  BIT_SCS_OSCRANGE                    DEF_BIT_04
#define  BIT_SCS_OSCEN                       DEF_BIT_05
#define  BIT_SCS_OSCSTAT                     DEF_BIT_06

/* CLOCK DIVIDERS REGISTER & BIT DEFINES */
#define  REG_CCLKCFG                         (*(CPU_REG32 *)(0x400FC104))
#define  REG_USBCLKCFG                       (*(CPU_REG32 *)(0x400FC108))
#define  REG_IRCTRIM                         (*(CPU_REG32 *)(0x400FC1A4))
#define  REG_PCLKSEL0                        (*(CPU_REG32 *)(0x400FC1A8))
#define  REG_PCLKSEL1                        (*(CPU_REG32 *)(0x400FC1AC))
/* CLOCK SOURCE SELECT REGISTER DEFINE */
#define  REG_CLKSRCSEL                       (*(CPU_REG32 *)(0x400FC10C))

/* PLL REGISTER AND POWER CONTROL --------------------------------------------*/
/* PLL REGISTER BASE ADDRESS DEFINES */
#define  REG_PLL_BASE_ADDR                   (CPU_INT32U )0x400FC080

#define  REG_PLLCTRL(pll_id)                 (*(CPU_REG32 *)(REG_PLL_BASE_ADDR + (0x20 * pll_id) + 0x00))
#define  REG_PLLCFG(pll_id)                  (*(CPU_REG32 *)(REG_PLL_BASE_ADDR + (0x20 * pll_id) + 0x04))
#define  REG_PLLSTAT(pll_id)                 (*(CPU_REG32 *)(REG_PLL_BASE_ADDR + (0x20 * pll_id) + 0x08))
#define  REG_PLLFEED(pll_id)                 (*(CPU_REG32 *)(REG_PLL_BASE_ADDR + (0x20 * pll_id) + 0x0C))

/* for PLL Control */
#define  BIT_PLLCTRL_PLLE                    DEF_BIT_00     /* PLL enable                                           */
#define  BIT_PLLCTRL_PLLC                    DEF_BIT_01     /* PLL connect                                          */

/* for PLL0 State  */
#define  BIT_PLLSTAT_PLL0_MSEL_MASK          DEF_BIT_FIELD(15,  0)
#define  BIT_PLLSTAT_PLL0_PSEL_MASK          DEF_BIT_FIELD(8 , 16)
#define  BIT_PLLSTAT_PLLE0_STAT              DEF_BIT_24     /* Read-back for the PLL0 enable bit                     */
#define  BIT_PLLSTAT_PLLC0_STAT              DEF_BIT_25     /* Read-back for the PLL0 connect bit                    */
#define  BIT_PLLSTAT_PLOCK0                  DEF_BIT_26     /* Refkect the PLL0 lock status                          */

/* for PLL1 State*/
#define  BIT_PLLSTAT_PLLE1_STAT              DEF_BIT_08     /* Read-back for the PLL1 enable bit                     */
#define  BIT_PLLSTAT_PLLC1_STAT              DEF_BIT_09     /* Read-back for the PLL1 connect bit                    */
#define  BIT_PLLSTAT_PLOCK1                  DEF_BIT_10     /* Refkect the PLL1 lock status                          */

/* PLL FEED VALUE */
#define  BIT_PLLFEED_VAL0                   (CPU_INT32U)(0x000000AA)
#define  BIT_PLLFEED_VAL1                   (CPU_INT32U)(0x00000055)

/* POWER CONTROL REGISTER DEFINES */
#define  REG_PCON                            (*(CPU_REG32 *)(0x400FC0C0))
#define  REG_PCONP                           (*(CPU_REG32 *)(0x400FC0C4))

/* APB DIVIDER REGISTER BIT DEFINES */
#define  BIT_APBDIV_MASK                     DEF_BIT_FIELD(2, 0)
#define  BIT_APBDIV_4                        DEF_BIT_NONE
#define  BIT_APBDIV_1                        DEF_BIT_MASK(1, 0)
#define  BIT_APBDIV_2                        DEF_BIT_MASK(2, 0)

/* --- FLASH ACCELERATOR CFG REGISTER & BIT DEFINES --- */
#define  REG_FLASHCFG                        (*(CPU_REG32 *)(0x400FC000))      

#define  REG_FLASHCFG_CLK_1                  DEF_BIT_MASK(1, 12)
#define  REG_FLASHCFG_CLK_2                  DEF_BIT_MASK(2, 12)
#define  REG_FLASHCFG_CLK_3                  DEF_BIT_MASK(3, 12)
#define  REG_FLASHCFG_CLK_4                  DEF_BIT_MASK(4, 12)
#define  REG_FLASHCFG_CLK_5                  DEF_BIT_MASK(5, 12)
#define  REG_FLASHCFG_CLK_6                  DEF_BIT_MASK(6, 12)

/* Function Prototypes -------------------------------------------------------*/
CPU_BOOLEAN  LPC17xx_System_Init (void);

CPU_INT32U   CPU_FreqGet    (void);

void         PerClkEn       (CPU_INT08U  per_nbr);
void         PerClkDis      (CPU_INT08U  per_nbr);
CPU_INT32U   PerClkFreqGet  (CPU_INT08U  per_nbr);
/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
#endif 
