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
#ifndef __LPC17XX_GPIO_H__
#define __LPC17XX_GPIO_H__

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
#define     __IO    volatile

/* Exported types ------------------------------------------------------------*/  
typedef  unsigned  int         CPU_INT32U;
typedef  volatile  CPU_INT32U  CPU_REG32;

/*         GPIO related register         */
/* GPIO PINSEL REGISTER GROUP */
#define  GPIO_REG_PINSEL_BASE_ADDR                             (CPU_INT32U  )(0x4002C000)

#define  GPIO_REG_PINSEL0                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x00)
#define  GPIO_REG_PINSEL1                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x04)
#define  GPIO_REG_PINSEL2                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x08)
#define  GPIO_REG_PINSEL3                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x0C)
#define  GPIO_REG_PINSEL4                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x10)
#define  GPIO_REG_PINSEL5                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x14)
#define  GPIO_REG_PINSEL6                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x18)
#define  GPIO_REG_PINSEL7                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x1C)
#define  GPIO_REG_PINSEL8                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x20)
#define  GPIO_REG_PINSEL9                                      (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x24)
#define  GPIO_REG_PINSEL10                                     (*(CPU_REG32 *)GPIO_REG_PINSEL_BASE_ADDR + 0x28)

/* GPIO PINMODE REGISTER GROUP */
#define  GPIO_REG_PINMODE_BASE_ADDR                            (CPU_INT32U  )(0x4002C040)

#define  GPIO_REG_PINMODE0                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x00)
#define  GPIO_REG_PINMODE1                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x04)
#define  GPIO_REG_PINMODE2                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x08)
#define  GPIO_REG_PINMODE3                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x0C)
#define  GPIO_REG_PINMODE4                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x10)
#define  GPIO_REG_PINMODE5                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x14)
#define  GPIO_REG_PINMODE6                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x18)
#define  GPIO_REG_PINMODE7                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x1C)
#define  GPIO_REG_PINMODE8                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x20)
#define  GPIO_REG_PINMODE9                                     (*(CPU_REG32 *)GPIO_REG_PINMODE_BASE_ADDR + 0x24)

/* GPIO PORTx_FAST REGISTER GROUP */
#define  GPIO_REG_PORTx_FAST_BASE_ADDR                         ((CPU_INT32U)0x2009C000)
#define  GPIO_REG_PORT0_FAST_BASE_ADDR                         ((CPU_INT32U)(GPIO_REG_PORTx_FAST_BASE_ADDR + 0x00))
#define  GPIO_REG_PORT1_FAST_BASE_ADDR                         ((CPU_INT32U)(GPIO_REG_PORTx_FAST_BASE_ADDR + 0x20))
#define  GPIO_REG_PORT2_FAST_BASE_ADDR                         ((CPU_INT32U)(GPIO_REG_PORTx_FAST_BASE_ADDR + 0x40))
#define  GPIO_REG_PORT3_FAST_BASE_ADDR                         ((CPU_INT32U)(GPIO_REG_PORTx_FAST_BASE_ADDR + 0x60))
#define  GPIO_REG_PORT4_FAST_BASE_ADDR                         ((CPU_INT32U)(GPIO_REG_PORTx_FAST_BASE_ADDR + 0x80))

#define  GPIO_REG_PINMODE_ODx_BASE_ADDR                        ((CPU_INT32U)GPIO_REG_PINMODE_BASE_ADDR + 0x28)
#define  GPIO_REG_PINMODE_OD0_BASE_ADDR                        ((CPU_INT32U)(GPIO_REG_PINMODE_ODx_BASE_ADDR + 0x00))
#define  GPIO_REG_PINMODE_OD1_BASE_ADDR                        ((CPU_INT32U)(GPIO_REG_PINMODE_ODx_BASE_ADDR + 0x04))
#define  GPIO_REG_PINMODE_OD2_BASE_ADDR                        ((CPU_INT32U)(GPIO_REG_PINMODE_ODx_BASE_ADDR + 0x08))
#define  GPIO_REG_PINMODE_OD3_BASE_ADDR                        ((CPU_INT32U)(GPIO_REG_PINMODE_ODx_BASE_ADDR + 0x0C))
#define  GPIO_REG_PINMODE_OD4_BASE_ADDR                        ((CPU_INT32U)(GPIO_REG_PINMODE_ODx_BASE_ADDR + 0x10))

/* for GPIO DIR */ 
/* 32bit Access */
#define  FIO0DIR							(*(__IO CPU_INT32U *)(0x2009C000)) 
#define  FIO1DIR							(*(__IO CPU_INT32U *)(0x2009C020))
#define  FIO2DIR							(*(__IO CPU_INT32U *)(0x2009C040))
#define  FIO3DIR							(*(__IO CPU_INT32U *)(0x2009C060))
#define  FIO4DIR							(*(__IO CPU_INT32U *)(0x2009C080))
/* 8bit Access */																				 
#define  FIO2DIR0							(*(__IO CPU_INT08U *)(0x2009C040)) 

/* for GPIO MASK */
#define  FIO0MASK							(*(__IO CPU_INT32U *)(0x2009C010))
#define  FIO1MASK							(*(__IO CPU_INT32U *)(0x2009C030))
#define  FIO2MASK							(*(__IO CPU_INT32U *)(0x2009C050))
#define  FIO3MASK							(*(__IO CPU_INT32U *)(0x2009C070))
#define  FIO4MASK							(*(__IO CPU_INT32U *)(0x2009C090))

/* for GPIO value register */
/* 32bit Access */
#define  FIO0PIN							(*(__IO CPU_INT32U *)(0x2009C014)) 
#define  FIO1PIN							(*(__IO CPU_INT32U *)(0x2009C034))
#define  FIO2PIN							(*(__IO CPU_INT32U *)(0x2009C054))
#define  FIO3PIN							(*(__IO CPU_INT32U *)(0x2009C074))
#define  FIO4PIN							(*(__IO CPU_INT32U *)(0x2009C094))
/* 8bit Access */
#define  FIO2PIN0							(*(__IO CPU_INT08U *)(0x2009C054)) 

/* for GPIO SET */
#define  FIO0SET							(*(__IO CPU_INT32U *)(0x2009C018))
#define  FIO1SET							(*(__IO CPU_INT32U *)(0x2009C038))
#define  FIO2SET							(*(__IO CPU_INT32U *)(0x2009C058))
#define  FIO3SET							(*(__IO CPU_INT32U *)(0x2009C078))
#define  FIO4SET							(*(__IO CPU_INT32U *)(0x2009C098))

/* for GPIO CLR */
#define  FIO0CLR							(*(__IO CPU_INT32U *)(0x2009C01C))
#define  FIO1CLR							(*(__IO CPU_INT32U *)(0x2009C03C))
#define  FIO2CLR							(*(__IO CPU_INT32U *)(0x2009C05C))
#define  FIO3CLR							(*(__IO CPU_INT32U *)(0x2009C07C))
#define  FIO4CLR							(*(__IO CPU_INT32U *)(0x2009C09C))

/* for pin select register */
#define  PINSEL0							(*(__IO CPU_INT32U *)(0x4002C000))
#define  PINSEL1							(*(__IO CPU_INT32U *)(0x4002C004))
#define  PINSEL2							(*(__IO CPU_INT32U *)(0x4002C008))
#define  PINSEL3							(*(__IO CPU_INT32U *)(0x4002C00c))
#define  PINSEL4							(*(__IO CPU_INT32U *)(0x4002C010))
#define  PINSEL5							(*(__IO CPU_INT32U *)(0x4002C014))
#define  PINSEL6							(*(__IO CPU_INT32U *)(0x4002C018))
#define  PINSEL7							(*(__IO CPU_INT32U *)(0x4002C01c))
#define  PINSEL8							(*(__IO CPU_INT32U *)(0x4002C020))
#define  PINSEL9							(*(__IO CPU_INT32U *)(0x4002C024))
#define  PINSEL10							(*(__IO CPU_INT32U *)(0x4002C028))


/* for GPIO inttrupt */
#define  REG_IOIntStatus				    (*(__IO CPU_INT32U *)  (0x40028080))

#define  REG_IO0IntEnR				    	(*(__IO CPU_INT32U *)  (0x40028090))
#define  REG_IO2IntEnR                      (*(__IO CPU_INT32U *)  (0x400280B0)) 	 
#define  REG_IO0IntEnF				    	(*(__IO CPU_INT32U *)  (0x40028094))
#define  REG_IO2IntEnF				    	(*(__IO CPU_INT32U *)  (0x400280B4))

#define  REG_IO0IntStatR				    (*(__IO CPU_INT32U *)  (0x40028084))
#define  REG_IO2IntStatR				    (*(__IO CPU_INT32U *)  (0x400280A4))
#define  REG_IO0IntStatF				    (*(__IO CPU_INT32U *)  (0x40028088))
#define  REG_IO2IntStatF				    (*(__IO CPU_INT32U *)  (0x400280A8))

#define  REG_IO0IntClr				    	(*(__IO CPU_INT32U *)  (0x4002808C))
#define  REG_IO2IntClr				    	(*(__IO CPU_INT32U *)  (0x400280AC))



/* Function Prototypes -------------------------------------------------------*/





/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/    
#endif                                                          /* End of module include.                               */
