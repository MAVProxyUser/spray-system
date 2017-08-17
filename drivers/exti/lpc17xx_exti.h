/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_exti.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-09-13 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	            
  */

#ifndef __LPC17XX_EXTI_H__
#define __LPC17XX_EXTI_H__

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/* exti interrupt pin define */
#define EXINT_PORTS             GPIO_PORT2_FAST     
 
#define EINT0_PIN               DEF_BIT_10          /* p2.10,FNCT2 -- EXIT0 pin */  
#define EINT1_PIN               DEF_BIT_11          /* p2.11,FNCT2 -- EXIT1 pin */
#define EINT2_PIN               DEF_BIT_12          /* p2.12,FNCT2 -- EXIT2 pin */
#define EINT3_PIN               DEF_BIT_13          /* p2.13,FNCT2 -- EXIT3 pin */


/*********************************************************************//**
 * Macro defines for EXTI  control register
 **********************************************************************/
#define EXTI_EINT0_BIT_MARK 	0x01
#define EXTI_EINT1_BIT_MARK 	0x02
#define EXTI_EINT2_BIT_MARK 	0x04
#define EXTI_EINT3_BIT_MARK 	0x08

/* EXTINT regester bit field difine */
#define EXTINT_EINT0            DEF_BIT_00
#define EXTINT_EINT1            DEF_BIT_01
#define EXTINT_EINT2            DEF_BIT_02
#define EXTINT_EINT3            DEF_BIT_03

/* Private Macros ------------------------------------------------------------- */

/**
 * @brief EXTI external interrupt line option
 */
typedef enum
{
	EXTI_EINT0, /*!<  External interrupt 0, P2.10 */
	EXTI_EINT1, /*!<  External interrupt 0, P2.11 */
	EXTI_EINT2, /*!<  External interrupt 0, P2.12 */
	EXTI_EINT3 	/*!<  External interrupt 0, P2.13 */
} EXTI_LINE_ENUM;

/**
 * @brief EXTI mode option
 */
typedef enum
{
	EXTI_MODE_LEVEL_SENSITIVE, 	/*!< Level sensitivity is selected */
	EXTI_MODE_EDGE_SENSITIVE  	/*!< Edge sensitivity is selected */
} EXTI_MODE_ENUM;

/**
 * @brief EXTI polarity option
 */
typedef enum
{
	EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE,	/*!< Low active or falling edge sensitive
												depending on pin mode */
	EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE	/*!< High active or rising edge sensitive
												depending on pin mode */
} EXTI_POLARITY_ENUM;

/**
 * @brief EXTI Initialize structure
 */
typedef struct
{
	EXTI_LINE_ENUM EXTI_Line; /*!<Select external interrupt pin (EINT0, EINT1, EINT 2, EINT3) */

	EXTI_MODE_ENUM EXTI_Mode; /*!< Choose between Level-sensitivity or Edge sensitivity */

	EXTI_POLARITY_ENUM EXTI_polarity; /*!< 	If EXTI mode is level-sensitive: this element use to select low or high active level
											if EXTI mode is polarity-sensitive: this element use to select falling or rising edge */

}EXTI_InitTypeDef;

/* Public Functions ----------------------------------------------------------- */
void EXTI0_Init(void);

void EXTI_Init(EXTI_InitTypeDef *EXTICfg);
void EXTI_DeInit(void);

void EXTI_Config(EXTI_InitTypeDef *EXTICfg);
void EXTI_SetMode(EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode);
void EXTI_SetPolarity(EXTI_LINE_ENUM EXTILine, EXTI_POLARITY_ENUM polarity);
void EXTI_ClearEXTIFlag(EXTI_LINE_ENUM EXTILine);



#ifdef __cplusplus
}
#endif

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif /* __LPC17XX_EXTI_H__ */
