/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_gpdma.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-03 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx gpdma module,reference for nxp        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/**
 * @brief Lookup Table of Connection Type matched with
 * Peripheral Data (FIFO) register base address
 */
const void *GPDMA_LUTPerAddr[] = 
{
		((void *)&LPC_SSP0->DR),				// SSP0 Tx
		((void *)&LPC_SSP0->DR),				// SSP0 Rx
		((void *)&LPC_SSP1->DR),				// SSP1 Tx
		((void *)&LPC_SSP1->DR),				// SSP1 Rx
		((void *)&LPC_ADC->ADGDR),			// ADC
		((void *)&LPC_I2S->I2STXFIFO), 		// I2S Tx
		((void *)&LPC_I2S->I2SRXFIFO), 		// I2S Rx
		((void *)&LPC_DAC->DACR),				// DAC
		((void *)&LPC_UART0->/*RBTHDLR.*/THR),	// UART0 Tx
		((void *)&LPC_UART0->/*RBTHDLR.*/RBR),	// UART0 Rx
		((void *)&LPC_UART1->/*RBTHDLR.*/THR),	// UART1 Tx
		((void *)&LPC_UART1->/*RBTHDLR.*/RBR),	// UART1 Rx
		((void *)&LPC_UART2->/*RBTHDLR.*/THR),	// UART2 Tx
		((void *)&LPC_UART2->/*RBTHDLR.*/RBR),	// UART2 Rx
		((void *)&LPC_UART3->/*RBTHDLR.*/THR),	// UART3 Tx
		((void *)&LPC_UART3->/*RBTHDLR.*/RBR),	// UART3 Rx
		((void *)&LPC_TIM0->MR0),				// MAT0.0
		((void *)&LPC_TIM0->MR1),				// MAT0.1
		((void *)&LPC_TIM1->MR0),				// MAT1.0
		((void *)&LPC_TIM1->MR1),				// MAT1.1
		((void *)&LPC_TIM2->MR0),				// MAT2.0
		((void *)&LPC_TIM2->MR1),				// MAT2.1
		((void *)&LPC_TIM3->MR0),				// MAT3.0
		((void *)&LPC_TIM3->MR1),				// MAT3.1
};

/**
 * @brief Lookup Table of GPDMA Channel Number matched with
 * GPDMA channel pointer
 */
const LPC_GPDMACH_TypeDef *pGPDMACh[8] = 
{
		LPC_GPDMACH0,	// GPDMA Channel 0
		LPC_GPDMACH1,	// GPDMA Channel 1
		LPC_GPDMACH2,	// GPDMA Channel 2
		LPC_GPDMACH3,	// GPDMA Channel 3
		LPC_GPDMACH4,	// GPDMA Channel 4
		LPC_GPDMACH5,	// GPDMA Channel 5
		LPC_GPDMACH6,	// GPDMA Channel 6
		LPC_GPDMACH7,	// GPDMA Channel 7
};

/**
 * @brief Optimized Peripheral Source and Destination burst size
 */
const uint8_t GPDMA_LUTPerBurst[] = 
{
		GPDMA_BSIZE_4,				// SSP0 Tx
		GPDMA_BSIZE_4,				// SSP0 Rx
		GPDMA_BSIZE_4,				// SSP1 Tx
		GPDMA_BSIZE_4,				// SSP1 Rx
		GPDMA_BSIZE_4,				// ADC
		GPDMA_BSIZE_32, 			// I2S channel 0
		GPDMA_BSIZE_32, 			// I2S channel 1
		GPDMA_BSIZE_1,				// DAC
		GPDMA_BSIZE_1,				// UART0 Tx
		GPDMA_BSIZE_1,				// UART0 Rx
		GPDMA_BSIZE_1,				// UART1 Tx
		GPDMA_BSIZE_1,				// UART1 Rx
		GPDMA_BSIZE_1,				// UART2 Tx
		GPDMA_BSIZE_1,				// UART2 Rx
		GPDMA_BSIZE_1,				// UART3 Tx
		GPDMA_BSIZE_1,				// UART3 Rx
		GPDMA_BSIZE_1,				// MAT0.0
		GPDMA_BSIZE_1,				// MAT0.1
		GPDMA_BSIZE_1,				// MAT1.0
		GPDMA_BSIZE_1,				// MAT1.1
		GPDMA_BSIZE_1,				// MAT2.0
		GPDMA_BSIZE_1,				// MAT2.1
		GPDMA_BSIZE_1,				// MAT3.0
		GPDMA_BSIZE_1,				// MAT3.1
};

/**
 * @brief Optimized Peripheral Source and Destination transfer width
 */
const uint8_t GPDMA_LUTPerWid[] = 
{
		GPDMA_WIDTH_BYTE,				// SSP0 Tx
		GPDMA_WIDTH_BYTE,				// SSP0 Rx
		GPDMA_WIDTH_BYTE,				// SSP1 Tx
		GPDMA_WIDTH_BYTE,				// SSP1 Rx
		GPDMA_WIDTH_WORD,				// ADC
		GPDMA_WIDTH_WORD, 				// I2S channel 0
		GPDMA_WIDTH_WORD, 				// I2S channel 1
		GPDMA_WIDTH_BYTE,				// DAC
		GPDMA_WIDTH_BYTE,				// UART0 Tx
		GPDMA_WIDTH_BYTE,				// UART0 Rx
		GPDMA_WIDTH_BYTE,				// UART1 Tx
		GPDMA_WIDTH_BYTE,				// UART1 Rx
		GPDMA_WIDTH_BYTE,				// UART2 Tx
		GPDMA_WIDTH_BYTE,				// UART2 Rx
		GPDMA_WIDTH_BYTE,				// UART3 Tx
		GPDMA_WIDTH_BYTE,				// UART3 Rx
		GPDMA_WIDTH_WORD,				// MAT0.0
		GPDMA_WIDTH_WORD,				// MAT0.1
		GPDMA_WIDTH_WORD,				// MAT1.0
		GPDMA_WIDTH_WORD,				// MAT1.1
		GPDMA_WIDTH_WORD,				// MAT2.0
		GPDMA_WIDTH_WORD,				// MAT2.1
		GPDMA_WIDTH_WORD,				// MAT3.0
		GPDMA_WIDTH_WORD,				// MAT3.1
};

/* Function Prototypes -------------------------------------------------------*/
/********************************************************************//**
 * @brief 		Initialize GPDMA controller
 * @param 		None
 * @return 		None
 *********************************************************************/
void GPDMA_Init(void)
{
	/* Enable GPDMA clock */
	PerClkEn (PER_NBR_GPDMA);

	// Reset all channel configuration register
	LPC_GPDMACH0->DMACCConfig = 0;
	LPC_GPDMACH1->DMACCConfig = 0;
	LPC_GPDMACH2->DMACCConfig = 0;
	LPC_GPDMACH3->DMACCConfig = 0;
	LPC_GPDMACH4->DMACCConfig = 0;
	LPC_GPDMACH5->DMACCConfig = 0;
	LPC_GPDMACH6->DMACCConfig = 0;
	LPC_GPDMACH7->DMACCConfig = 0;

	/* Clear all DMA interrupt and error flag */
	LPC_GPDMA->DMACIntTCClear = 0xFF;
	LPC_GPDMA->DMACIntErrClr = 0xFF;

	/* Enable DMA channels, little endian */
	LPC_GPDMA->DMACConfig = GPDMA_DMACConfig_E;
	while (!(LPC_GPDMA->DMACConfig & GPDMA_DMACConfig_E));
}

/********************************************************************//**
 * @brief 		Setup GPDMA channel peripheral according to the specified
 *               parameters in the GPDMAChannelConfig.
 * @param[in]	GPDMAChannelConfig Pointer to a GPDMA_CH_CFG_Type
 * 									structure that contains the configuration
 * 									information for the specified GPDMA channel peripheral.
 * @return		ERROR if selected channel is enabled before
 * 				or SUCCESS if channel is configured successfully
 *********************************************************************/
Status GPDMA_Setup(GPDMA_Channel_CFG_Type *GPDMAChannelConfig)
{
	LPC_GPDMACH_TypeDef *pDMAch;
	uint32_t tmp1, tmp2;

	if (LPC_GPDMA->DMACEnbldChns & (GPDMA_DMACEnbldChns_Ch(GPDMAChannelConfig->ChannelNum))) {
		// This channel is enabled, return ERROR, need to release this channel first
		uart_printf(0,"E1\r\n");
		return ERROR;
	}

	// Get Channel pointer
	pDMAch = (LPC_GPDMACH_TypeDef *) pGPDMACh[GPDMAChannelConfig->ChannelNum];

	// Reset the Interrupt status
	LPC_GPDMA->DMACIntTCClear = GPDMA_DMACIntTCClear_Ch(GPDMAChannelConfig->ChannelNum);
	LPC_GPDMA->DMACIntErrClr = GPDMA_DMACIntErrClr_Ch(GPDMAChannelConfig->ChannelNum);

	// Clear DMA configure
	pDMAch->DMACCControl = 0x00;
	pDMAch->DMACCConfig = 0x00;

	/* Assign Linker List Item value */
	pDMAch->DMACCLLI = GPDMAChannelConfig->DMALLI;

	/* Set value to Channel Control Registers */
	switch (GPDMAChannelConfig->TransferType)
	{
	// Memory to memory
	case GPDMA_TRANSFERTYPE_M2M:
		// Assign physical source and destination address
		pDMAch->DMACCSrcAddr = GPDMAChannelConfig->SrcMemAddr;
		pDMAch->DMACCDestAddr = GPDMAChannelConfig->DstMemAddr;
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize(GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_32) \
						| GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_32) \
						| GPDMA_DMACCxControl_SWidth(GPDMAChannelConfig->TransferWidth) \
						| GPDMA_DMACCxControl_DWidth(GPDMAChannelConfig->TransferWidth) \
						| GPDMA_DMACCxControl_SI \
						| GPDMA_DMACCxControl_DI \
						| GPDMA_DMACCxControl_I;
		break;
	// Memory to peripheral
	case GPDMA_TRANSFERTYPE_M2P:
		// Assign physical source
		pDMAch->DMACCSrcAddr = GPDMAChannelConfig->SrcMemAddr;
		// Assign peripheral destination address
		pDMAch->DMACCDestAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->DstConn];
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SI \
						| GPDMA_DMACCxControl_I;
		break;
	// Peripheral to memory
	case GPDMA_TRANSFERTYPE_P2M:
		// Assign peripheral source address
		pDMAch->DMACCSrcAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->SrcConn];
		// Assign memory destination address
		pDMAch->DMACCDestAddr = GPDMAChannelConfig->DstMemAddr;
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DI \
						| GPDMA_DMACCxControl_I;
		break;
	// Peripheral to peripheral
	case GPDMA_TRANSFERTYPE_P2P:
		// Assign peripheral source address
		pDMAch->DMACCSrcAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->SrcConn];
		// Assign peripheral destination address
		pDMAch->DMACCDestAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->DstConn];
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_I;
		break;
	// Do not support any more transfer type, return ERROR
	default:
		return ERROR;
	}

	/* Re-Configure DMA Request Select for source peripheral */
	if (GPDMAChannelConfig->SrcConn > 15)
	{
		DMAREQSEL |= (1<<(GPDMAChannelConfig->SrcConn - 16));
	} else {
		DMAREQSEL &= ~(1<<(GPDMAChannelConfig->SrcConn - 8));
	}

	/* Re-Configure DMA Request Select for Destination peripheral */
	if (GPDMAChannelConfig->DstConn > 15)
	{
		DMAREQSEL |= (1<<(GPDMAChannelConfig->DstConn - 16));
	} else {
		DMAREQSEL &= ~(1<<(GPDMAChannelConfig->DstConn - 8));
	}

	/* Enable DMA channels, little endian */
	LPC_GPDMA->DMACConfig = GPDMA_DMACConfig_E;
	while (!(LPC_GPDMA->DMACConfig & GPDMA_DMACConfig_E));

	// Calculate absolute value for Connection number
	tmp1 = GPDMAChannelConfig->SrcConn;
	tmp1 = ((tmp1 > 15) ? (tmp1 - 8) : tmp1);
	tmp2 = GPDMAChannelConfig->DstConn;
	tmp2 = ((tmp2 > 15) ? (tmp2 - 8) : tmp2);

	// Configure DMA Channel, enable Error Counter and Terminate counter
	pDMAch->DMACCConfig = GPDMA_DMACCxConfig_IE | GPDMA_DMACCxConfig_ITC /*| GPDMA_DMACCxConfig_E*/ \
		| GPDMA_DMACCxConfig_TransferType((uint32_t)GPDMAChannelConfig->TransferType) \
		| GPDMA_DMACCxConfig_SrcPeripheral(tmp1) \
		| GPDMA_DMACCxConfig_DestPeripheral(tmp2);

	return SUCCESS;
}


/*********************************************************************//**
 * @brief		Enable/Disable DMA channel
 * @param[in]	channelNum	GPDMA channel, should be in range from 0 to 7
 * @param[in]	NewState	New State of this command, should be:
 * 					- ENABLE.
 * 					- DISABLE.
 * @return		None
 **********************************************************************/
void GPDMA_ChannelCmd(uint8_t channelNum, FunctionalState NewState)
{
	LPC_GPDMACH_TypeDef *pDMAch;

	// Get Channel pointer
	pDMAch = (LPC_GPDMACH_TypeDef *) pGPDMACh[channelNum];

	if (NewState == ENABLE) {
		pDMAch->DMACCConfig |= GPDMA_DMACCxConfig_E;
	} else {
		pDMAch->DMACCConfig &= ~GPDMA_DMACCxConfig_E;
	}
}

/*********************************************************************//**
 * @brief		Check if corresponding channel does have an active interrupt
 * 				request or not
 * @param[in]	type		type of status, should be:
 * 					- GPDMA_STAT_INT: 		GPDMA Interrupt Status
 * 					- GPDMA_STAT_INTTC: 	GPDMA Interrupt Terminal Count Request Status
 * 					- GPDMA_STAT_INTERR:	GPDMA Interrupt Error Status
 * 					- GPDMA_STAT_RAWINTTC:	GPDMA Raw Interrupt Terminal Count Status
 * 					- GPDMA_STAT_RAWINTERR:	GPDMA Raw Error Interrupt Status
 * 					- GPDMA_STAT_ENABLED_CH:GPDMA Enabled Channel Status
 * @param[in]	channel		GPDMA channel, should be in range from 0 to 7
 * @return		IntStatus	status of DMA channel interrupt after masking
 * 				Should be:
 * 					- SET: the corresponding channel has no active interrupt request
 * 					- RESET: the corresponding channel does have an active interrupt request
 **********************************************************************/
IntStatus GPDMA_IntGetStatus(GPDMA_Status_Type type, uint8_t channel)
{
	CHECK_PARAM(PARAM_GPDMA_STAT(type));
	CHECK_PARAM(PARAM_GPDMA_CHANNEL(channel));

	switch (type)
	{
	case GPDMA_STAT_INT: //check status of DMA channel interrupts
		if (LPC_GPDMA->DMACIntStat & (GPDMA_DMACIntStat_Ch(channel)))
			return SET;
		return RESET;
	case GPDMA_STAT_INTTC: // check terminal count interrupt request status for DMA
		if (LPC_GPDMA->DMACIntTCStat & GPDMA_DMACIntTCStat_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_INTERR: //check interrupt status for DMA channels
		if (LPC_GPDMA->DMACIntErrStat & GPDMA_DMACIntTCClear_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_RAWINTTC: //check status of the terminal count interrupt for DMA channels
		if (LPC_GPDMA->DMACRawIntErrStat & GPDMA_DMACRawIntTCStat_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_RAWINTERR: //check status of the error interrupt for DMA channels
		if (LPC_GPDMA->DMACRawIntTCStat & GPDMA_DMACRawIntErrStat_Ch(channel))
			return SET;
		return RESET;
	default: //check enable status for DMA channels
		if (LPC_GPDMA->DMACEnbldChns & GPDMA_DMACEnbldChns_Ch(channel))
			return SET;
		return RESET;
	}
}

/*********************************************************************//**
 * @brief		Clear one or more interrupt requests on DMA channels
 * @param[in]	type		type of interrupt request, should be:
 * 					- GPDMA_STATCLR_INTTC: 	GPDMA Interrupt Terminal Count Request Clear
 * 					- GPDMA_STATCLR_INTERR: GPDMA Interrupt Error Clear
 * @param[in]	channel		GPDMA channel, should be in range from 0 to 7
 * @return		None
 **********************************************************************/
void GPDMA_ClearIntPending(GPDMA_StateClear_Type type, uint8_t channel)
{
	CHECK_PARAM(PARAM_GPDMA_STATCLR(type));
	CHECK_PARAM(PARAM_GPDMA_CHANNEL(channel));

	if (type == GPDMA_STATCLR_INTTC) // clears the terminal count interrupt request on DMA channel
		LPC_GPDMA->DMACIntTCClear = GPDMA_DMACIntTCClear_Ch(channel);
	else // clear the error interrupt request
		LPC_GPDMA->DMACIntErrClr = GPDMA_DMACIntErrClr_Ch(channel);
}
/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/

