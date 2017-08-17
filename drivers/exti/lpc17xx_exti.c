/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_exti.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-09-13 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	            
  */

/* Includes ------------------------------------------------------------------- */
#include "..\drivers.h"

/* Public Functions ----------------------------------------------------------- */
//extern volatile CPU_INT32U ctm_int_flag;
void EXIT0_ISRHandler(void)
{
    /*如果是在电平触发模式,且电平为低电平,清除该位要等到高电平时*/
//	LPC_SC->EXTINT = EXTINT_EINT0;	
//	BSP_IntDis (BSP_INT_SRC_NBR_EINT0); /* 禁止中断，一次完整的按键结束后再开启中断 */
//
//	ctm_int_flag   = 1;
}

void EXTI0_Init(void)
{
//    EXTI_InitTypeDef EXTI0_Cfg;
//
//	EXTI0_Cfg.EXTI_Line     = EXTI_EINT0;
//	EXTI0_Cfg.EXTI_Mode     = EXTI_MODE_EDGE_SENSITIVE;
//	EXTI0_Cfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
//
//    EXTI_Init(&EXTI0_Cfg);
//
//   	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_EINT0,(CPU_FNCT_VOID)EXIT0_ISRHandler); 
//    //BSP_IntPrioSet((CPU_INT08U)BSP_INT_SRC_NBR_EINT0,0);
//	BSP_IntEn(BSP_INT_SRC_NBR_EINT0); 
}


/*********************************************************************//**
 * @brief 		Initial for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR registers to default value
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EXTI_Init(EXTI_InitTypeDef *EXTICfg)
{
	LPC_SC->EXTINT   = 0xF;
	LPC_SC->EXTMODE  = 0x0;
	LPC_SC->EXTPOLAR = 0x0;

    EXTI_Config(EXTICfg);

	if(EXTICfg->EXTI_Line == EXTI_EINT0)
	{
        GPIO_Cfg(GPIO_PORT2_FAST,EINT0_PIN,GPIO_OPT_FNCT_2);  /* p2.10,FNCT2 -- EXIT0 pin */   
	}else if(EXTICfg->EXTI_Line == EXTI_EINT1)
	{
        GPIO_Cfg(GPIO_PORT2_FAST,EINT1_PIN,GPIO_OPT_FNCT_2);  /* p2.11,FNCT2 -- EXIT1 pin */ 
	}else if(EXTICfg->EXTI_Line == EXTI_EINT2)
	{
        GPIO_Cfg(GPIO_PORT2_FAST,EINT2_PIN,GPIO_OPT_FNCT_2);  /* p2.12,FNCT2 -- EXIT2 pin */ 
	}else if(EXTICfg->EXTI_Line == EXTI_EINT3)
	{
        GPIO_Cfg(GPIO_PORT2_FAST,EINT3_PIN,GPIO_OPT_FNCT_2);  /* p2.13,FNCT2 -- EXIT3 pin */ 
	}
}

/*********************************************************************//**
* @brief 		Close EXT
* @param[in]	None
* @return 		None
**********************************************************************/
void EXTI_DeInit(void)
{
	;
}

/*********************************************************************//**
 * @brief 		Configuration for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR register
 * @param[in]	EXTICfg	Pointer to a EXTI_InitTypeDef structure
 *              that contains the configuration information for the
 *              specified external interrupt
 * @return 		None
 **********************************************************************/
void EXTI_Config(EXTI_InitTypeDef *EXTICfg)
{
	EXTI_SetMode(EXTICfg->EXTI_Line, EXTICfg->EXTI_Mode);
	EXTI_SetPolarity(EXTICfg->EXTI_Line, EXTICfg->EXTI_polarity);
}

/*********************************************************************//**
* @brief 		Set mode for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	mode 	external mode, should be:
* 				- EXTI_MODE_LEVEL_SENSITIVE
* 				- EXTI_MODE_EDGE_SENSITIVE
* @return 		None
*********************************************************************/
void EXTI_SetMode(EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode)
{
	if(mode == EXTI_MODE_EDGE_SENSITIVE)
	{
		LPC_SC->EXTMODE |= (1 << EXTILine);
	}
	else if(mode == EXTI_MODE_LEVEL_SENSITIVE)
	{
		LPC_SC->EXTMODE &= ~(1 << EXTILine);
	}
}

/*********************************************************************//**
* @brief 		Set polarity for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	polarity	 external polarity value, should be:
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* @return 		None
*********************************************************************/
void EXTI_SetPolarity(EXTI_LINE_ENUM EXTILine, EXTI_POLARITY_ENUM polarity)
{
	if(polarity == EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE)
	{
		LPC_SC->EXTPOLAR |= (1 << EXTILine);
	}
	else if(polarity == EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE)
	{
		LPC_SC->EXTPOLAR &= ~(1 << EXTILine);
	}
}

/*********************************************************************//**
* @brief 		Clear External interrupt flag
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @return 		None
*********************************************************************/
void EXTI_ClearEXTIFlag(EXTI_LINE_ENUM EXTILine)
{
	LPC_SC->EXTINT |= (1 << EXTILine);
}
/* --------------------------------- End Of File ------------------------------ */

