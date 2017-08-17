/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : BL_WKM2_LED_IAP.uvproj
  * @File Name          : bsp.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-07-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    Begginning of application   
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H__
#define __BSP_H__

/* Includes ------------------------------------------------------------------*/
#include "libdef.h"

/* public define -------------------------------------------------------------*/
/* for GPIO DIR */ 
/* 32bit Access */
#define  FIO0DIR							(*(volatile unsigned int *)(0x2009C000)) 
#define  FIO1DIR							(*(volatile unsigned int *)(0x2009C020))
#define  FIO2DIR							(*(volatile unsigned int *)(0x2009C040))
#define  FIO3DIR							(*(volatile unsigned int *)(0x2009C060))
#define  FIO4DIR							(*(volatile unsigned int *)(0x2009C080))
/* 8bit Access */																				 
#define  FIO2DIR0							(*(volatile unsigned char*)(0x2009C040)) 

/* for GPIO MASK */
#define  FIO0MASK							(*(volatile unsigned int *)(0x2009C010))
#define  FIO1MASK							(*(volatile unsigned int *)(0x2009C030))
#define  FIO2MASK							(*(volatile unsigned int *)(0x2009C050))
#define  FIO3MASK							(*(volatile unsigned int *)(0x2009C070))
#define  FIO4MASK							(*(volatile unsigned int *)(0x2009C090))

/* for GPIO value register */
/* 32bit Access */
#define  FIO0PIN							(*(volatile unsigned int *)(0x2009C014)) 
#define  FIO1PIN							(*(volatile unsigned int *)(0x2009C034))
#define  FIO2PIN							(*(volatile unsigned int *)(0x2009C054))
#define  FIO3PIN							(*(volatile unsigned int *)(0x2009C074))
#define  FIO4PIN							(*(volatile unsigned int *)(0x2009C094))
/* 8bit Access */
#define  FIO2PIN0							(*(volatile unsigned char*)(0x2009C054)) 

/* for GPIO SET */
#define  FIO0SET							(*(volatile unsigned int *)(0x2009C018))
#define  FIO1SET							(*(volatile unsigned int *)(0x2009C038))
#define  FIO2SET							(*(volatile unsigned int *)(0x2009C058))
#define  FIO3SET							(*(volatile unsigned int *)(0x2009C078))
#define  FIO4SET							(*(volatile unsigned int *)(0x2009C098))

/* for GPIO CLR */
#define  FIO0CLR							(*(volatile unsigned int *)(0x2009C01C))
#define  FIO1CLR							(*(volatile unsigned int *)(0x2009C03C))
#define  FIO2CLR							(*(volatile unsigned int *)(0x2009C05C))
#define  FIO3CLR							(*(volatile unsigned int *)(0x2009C07C))
#define  FIO4CLR							(*(volatile unsigned int *)(0x2009C09C))

/* for pin select register */
#define  PINSEL0							(*(volatile unsigned int *)(0x4002C000))
#define  PINSEL1							(*(volatile unsigned int *)(0x4002C004))
#define  PINSEL2							(*(volatile unsigned int *)(0x4002C008))
#define  PINSEL3							(*(volatile unsigned int *)(0x4002C00c))
#define  PINSEL4							(*(volatile unsigned int *)(0x4002C010))
#define  PINSEL5							(*(volatile unsigned int *)(0x4002C014))
#define  PINSEL6							(*(volatile unsigned int *)(0x4002C018))
#define  PINSEL7							(*(volatile unsigned int *)(0x4002C01c))
#define  PINSEL8							(*(volatile unsigned int *)(0x4002C020))
#define  PINSEL9							(*(volatile unsigned int *)(0x4002C024))
#define  PINSEL10							(*(volatile unsigned int *)(0x4002C028))

#define  PINMODE0							(*(volatile unsigned int *)(0x4002C040))
#define  PINMODE1							(*(volatile unsigned int *)(0x4002C044))
#define  PINMODE2							(*(volatile unsigned int *)(0x4002C048))
#define  PINMODE3							(*(volatile unsigned int *)(0x4002C04c))
#define  PINMODE4							(*(volatile unsigned int *)(0x4002C050))
#define  PINMODE7							(*(volatile unsigned int *)(0x4002C05C))
#define  PINMODE9							(*(volatile unsigned int *)(0x4002C064))

/* EXTERN Function Prototypes ------------------------------------------------*/
extern unsigned int CPU_SR_Save(void);
extern void CPU_SR_Restore(unsigned int cpu_sr);

/* public define -------------------------------------------------------------*/
#define  CPU_SR_ALLOC()        unsigned int  cpu_sr = (unsigned int)0;
#define  CPU_CRITICAL_ENTER()  {cpu_sr = CPU_SR_Save();}
#define  CPU_CRITICAL_EXIT()   {CPU_SR_Restore(cpu_sr);}

#endif
/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

