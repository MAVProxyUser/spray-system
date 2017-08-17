/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_adc.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-08-25 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx adc module,reference for nxp        
  */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_adc.h"
//#include "lpc17xx_gpio.h"
//#define __ADC_DEBUG__


/*********************************************************************//**
* @brief 		Get Result conversion from A/D data register
* @param[in]	channel number which want to read back the result
* @return 		Result of conversion
*********************************************************************/
//CPU_INT32S ADC0_GetData(CPU_INT32U channel,CPU_INT16U *pvl)
//{
//    CPU_INT32U adc_value;
//
//	adc_value = *(CPU_INT32U *)((&LPC_ADC->ADDR0) + channel);
//
//	if(adc_value& ADC_DR_DONE_FLAG)
//	{
//	    *pvl = ADC_GDR_RESULT(adc_value); 
//		return 1;
//	}else
//	{
//		return 0;
//	}
//	
//}

void sensor_adc_init(void)
{
	
	PINSEL1 &= ~(3<<14);            
	PINSEL1 |=   1<<14 ;			  /* ADC0.0 pin select */
	PINSEL1 &= ~(3<<16);            
	PINSEL1 |=  (1<<16);			/* ADC0.1 pin select press0*/
	
	PINSEL1 &= ~(3<<18); 			/* ADC0.2 pin select temperature 0*/
	PINSEL1 |=   1<<18 ;	
	PINSEL1 &= ~(3<<20);			/* ADC0.3 pin select temperature 1*/
	PINSEL1 |=  (1<<20);
	
	PINSEL3 &= ~(3<<28);      /* ADC0.4 pin select  water_level*/
	PINSEL3 |=   3<<28 ;
	
	PINSEL3 &= ~(3<<30);      /* ADC0.5 pin select  resaved */
	PINSEL3 |=   3<<30;
	
	ADC_Init(LPC_ADC,1000); // ²ÉÑùÆµÂÊ 2000HZ
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN3,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN4,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN5,DISABLE);
	
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);	
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);	
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_4,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_5,ENABLE);
	
	ADC_BurstCmd(LPC_ADC, ENABLE);
	ADC_StartCmd(LPC_ADC,ADC_START_CONTINUOUS);
	
}

unsigned int press0_sensor_get(void)
{
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
}

unsigned int press1_sensor_get(void)
{
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
}

unsigned int  temperature0_sensor_get(void)
{
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_2);
}

unsigned int  temperature1_sensor_get(void)
{
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3);
}

unsigned int water_level_get(void)
{
//	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_4);
	return ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_5);
}

CPU_INT16U ADC0_GetData(CPU_INT32U channel)
{
	return (CPU_INT16U)ADC_GDR_RESULT(*(CPU_INT32U *)((&LPC_ADC->ADDR0) + channel));
}
/*
 * param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
 * param[in]	rate ADC conversion rate, should be <=200KHz
 * return 		None
 */
/*********************************************************************//**
 * @brief 		Initial for ADC
 * 					+ Set bit PCADC
 * 					+ Set clock for ADC
 * 					+ Set Clock Frequency
 * @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
 * @param[in]	rate ADC conversion rate, should be <=200KHz
 * @return 		None
 **********************************************************************/
extern uint32_t SystemCoreClock; 
	uint32_t ADCPClk, temp, tmp;
void ADC_Init(LPC_ADC_TypeDef *ADCx, uint32_t rate)
{

	
	// Turn on power and clock
  LPC_SC->PCONP |= 1<<12;	          /* enable ADC clock source */
	ADCx->ADCR = 0;
	//Enable PDN bit
	tmp = ADC_CR_PDN;
	// Set clock frequency
	ADCPClk = SystemCoreClock/4;
	/* The APB clock (PCLK_ADC0) is divided by (CLKDIV+1) to produce the clock for
	 * A/D converter, which should be less than or equal to 13MHz.
	 * A fully conversion requires 65 of these clocks.
	 * ADC clock = PCLK_ADC0 / (CLKDIV + 1);
	 * ADC rate = ADC clock / 65;
	 */
	temp = rate * 65;
	temp = (ADCPClk * 2 + temp)/(2 * temp) - 1; //get the round value by fomular: (2*A + B)/(2*B)
	tmp |=  ADC_CR_CLKDIV(temp);
	ADCx->ADCR = tmp;
}


/*********************************************************************//**
* @brief 		Close ADC
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @return 		None
**********************************************************************/
void ADC_DeInit(LPC_ADC_TypeDef *ADCx)
{
//	CHECK_PARAM(PARAM_ADCx(ADCx));

	// Clear PDN bit
	ADCx->ADCR &= ~ADC_CR_PDN;

	// Turn off power and clock
    if(ADCx == LPC_ADC)
    {
//	     PerClkDis(PER_NBR_AD0);	              /* disable ADC0 clock source */
    }else
    {
        return;
	}
}

/*********************************************************************//**
* @brief 		Set start mode for ADC
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	start_mode Start mode choose one of modes in
* 				'ADC_START_OPT' enumeration type definition, should be:
* 				- ADC_START_CONTINUOUS
* 				- ADC_START_NOW
* 				- ADC_START_ON_EINT0
* 				- ADC_START_ON_CAP01
*				- ADC_START_ON_MAT01
*				- ADC_START_ON_MAT03
*				- ADC_START_ON_MAT10
*				- ADC_START_ON_MAT11
* @return 		None
*********************************************************************/
void ADC_StartCmd(LPC_ADC_TypeDef *ADCx, CPU_INT08U start_mode)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));
	//CHECK_PARAM(PARAM_ADC_START_OPT(start_mode));

	ADCx->ADCR &= ~ADC_CR_START_MASK;
	ADCx->ADCR |=ADC_CR_START_MODE_SEL((CPU_INT32U)start_mode);
}


/*********************************************************************//**
* @brief 		ADC Burst mode setting
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	NewState
* 				-	1: Set Burst mode
* 				-	0: reset Burst mode
* @return 		None
**********************************************************************/
void ADC_BurstCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));

	ADCx->ADCR &= ~ADC_CR_BURST;
	if (NewState){
		ADCx->ADCR |= ADC_CR_BURST;
	}
}

/*********************************************************************//**
* @brief 		Set AD conversion in power mode
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	NewState
* 				-	1: AD converter is optional
* 				-	0: AD Converter is in power down mode
* @return 		None
**********************************************************************/
void ADC_PowerdownCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));

	ADCx->ADCR &= ~ADC_CR_PDN;
	if (NewState){
		ADCx->ADCR |= ADC_CR_PDN;
	}
}

/*********************************************************************//**
* @brief 		Set Edge start configuration
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	EdgeOption is ADC_START_ON_RISING and ADC_START_ON_FALLING
* 					0:ADC_START_ON_RISING
* 					1:ADC_START_ON_FALLING
* @return 		None
**********************************************************************/
void ADC_EdgeStartConfig(LPC_ADC_TypeDef *ADCx, CPU_INT08U EdgeOption)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));
	//CHECK_PARAM(PARAM_ADC_START_ON_EDGE_OPT(EdgeOption));

	ADCx->ADCR &= ~ADC_CR_EDGE;
	if (EdgeOption){
		ADCx->ADCR |= ADC_CR_EDGE;
	}
}

/*********************************************************************//**
* @brief 		ADC interrupt configuration
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	IntType: type of interrupt, should be:
* 				- ADC_ADINTEN0: Interrupt channel 0
* 				- ADC_ADINTEN1: Interrupt channel 1
* 				...
* 				- ADC_ADINTEN7: Interrupt channel 7
* 				- ADC_ADGINTEN: Individual channel/global flag done generate an interrupt
* @param[in]	NewState:
* 					- SET : enable ADC interrupt
* 					- RESET: disable ADC interrupt
* @return 		None
**********************************************************************/
void ADC_IntConfig (LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));
	//CHECK_PARAM(PARAM_ADC_TYPE_INT_OPT(IntType));

	ADCx->ADINTEN &= ~ADC_INTEN_CH(IntType);
	if (NewState){
		ADCx->ADINTEN |= ADC_INTEN_CH(IntType);
	}
}

/*********************************************************************//**
* @brief 		Enable/Disable ADC channel number
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	Channel channel number
* @param[in]	NewState Enable or Disable
*
* @return 		None
**********************************************************************/
void ADC_ChannelCmd (LPC_ADC_TypeDef *ADCx, CPU_INT08U Channel, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));
	//CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(Channel));

	if (NewState == ENABLE) {
		ADCx->ADCR |= ADC_CR_CH_SEL(Channel);
	} else {
        if (ADCx->ADCR & ADC_CR_START_MASK) //need to stop START bits before disable channel
		   ADCx->ADCR &= ~ADC_CR_START_MASK;
		ADCx->ADCR &= ~ADC_CR_CH_SEL(Channel);
	}
}

/*********************************************************************//**
* @brief 		Get ADC result
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	channel: channel number, should be 0...7
* @return 		Data conversion
**********************************************************************/
CPU_INT16U ADC_ChannelGetData(LPC_ADC_TypeDef *ADCx, CPU_INT08U channel)
{
	CPU_INT32U adc_value;

	//CHECK_PARAM(PARAM_ADCx(ADCx));
	//CHECK_PARAM(PARAM_ADC_CHANNEL_SELECTION(channel));

	adc_value = *(CPU_INT32U *) ((&ADCx->ADDR0) + channel);
	return ADC_DR_RESULT(adc_value);
}

/*********************************************************************//**
* @brief 		Get ADC Chanel status from ADC data register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	channel: channel number, should be 0..7
* @param[in]  	StatusType
*              		 	0:Burst status
*               		1:Done 	status
* @return 		SET / RESET
**********************************************************************/
FlagStatus ADC_ChannelGetStatus(LPC_ADC_TypeDef *ADCx, CPU_INT08U channel, CPU_INT32U StatusType)
{
	CPU_INT32U temp;


	temp =  *(CPU_INT32U *) ((&ADCx->ADDR0) + channel);
	if (StatusType) {
		temp &= ADC_DR_DONE_FLAG;
	}else{
		temp &= ADC_DR_OVERRUN_FLAG;
	}
	if (temp) {
		return SET;
	} else {
		return RESET;
	}

}

/*********************************************************************//**
* @brief 		Get ADC Data from AD Global register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @return 		Result of conversion
**********************************************************************/
CPU_INT32U ADC_GlobalGetData(LPC_ADC_TypeDef *ADCx)
{
//	CHECK_PARAM(PARAM_ADCx(ADCx));

	return ((CPU_INT32U)(ADCx->ADGDR));
}

/*********************************************************************//**
* @brief 		Get ADC Chanel status from AD global data register
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]  	StatusType
*              		 	0:Burst status
*               		1:Done 	status
* @return 		SET / RESET
**********************************************************************/
FlagStatus ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx, CPU_INT32U StatusType)
{
	CPU_INT32U temp;

	temp =  ADCx->ADGDR;
	if (StatusType){
		temp &= ADC_DR_DONE_FLAG;
	}else{
		temp &= ADC_DR_OVERRUN_FLAG;
	}
	if (temp){
		return SET;
	}else{
		return RESET;
	}
}

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

