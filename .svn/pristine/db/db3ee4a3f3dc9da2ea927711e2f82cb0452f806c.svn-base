/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_ssp.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-02-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx ssp module,reference for nxp        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* Function Prototypes -------------------------------------------------------*/

/*
 * Name			  :	static void SSP_clock_set (LPC_SSP_TypeDef *SSPx, uint32_t target_clock);
 * Input          : none
 * Output         : none
 * Caller(s)      : app
 * Return         : none
 * Notes          :	
 */
static void SSP_clock_set (LPC_SSP_TypeDef *SSPx, CPU_INT32U target_clock)
{
    CPU_INT32U prescale, cr0_div, cmp_clk, ssp_clk;
	CHECK_PARAM(PARAM_SSPx(SSPx));
	
	if (SSPx == LPC_SSP0)
	{
    	ssp_clk = PerClkFreqGet (PER_NBR_SSP0);
    } else if (SSPx == LPC_SSP1) 
	{
    	ssp_clk = PerClkFreqGet (PER_NBR_SSP1);
    } else 
	{
    	return;
    }

	/*  Find divider: 
	 *  2 =< CPSDVSR <= 254,(even numbers only);
	 *	ssp_clock = PCLK / (CPSDVSR × [SCR+1]);
	 */
	cr0_div  = 0;
	cmp_clk  = 0xFFFFFFFF;
	prescale = 2;
	while(cmp_clk > target_clock)
	{
	    cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
		if(cmp_clk > target_clock)
		{
		    cr0_div++;
			if( cr0_div > 0xFF)
			{
			    cr0_div   = 0;
				prescale += 2;
			}
		}
	}

	/* write computed CPSDVSR and SCR */
    SSPx->CR0 &= ~DEF_BIT_MASK(0xFF,CR0_SCR_BIT_SHIFT);
    SSPx->CR0 |= (DEF_BIT_MASK(cr0_div,CR0_SCR_BIT_SHIFT)) & SSP_CR0_BITMASK;
    SSPx->CPSR =  prescale & SSP_CPSR_BITMASK;
}

/*
 * Name			  :	void SSP_Cmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
 * Input          : none
 * Output         : none
 * Caller(s)      : app.
 * Return         : none
 * Notes          :	SSP enable and disable
 */
void SSP_Cmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->CR1 |= CR1_SSE_EN;
	}
	else
	{
		SSPx->CR1 &= (~CR1_SSE_EN) & SSP_CR1_BITMASK;
	}
}

/*
 * Name			  :	void SSP_Init(LPC_SSP_TypeDef *SSPx,SSP_CFG_Type *SSP_ConfigStruct);
 * Input          : none
 * Output         : none
 * Caller(s)      : app
 * Return         : none
 * Notes          :	
 */
void SSP_Init(LPC_SSP_TypeDef *SSPx,SSP_CFG_Type *SSP_ConfigStruct)
{
    uint32_t tmp;

	CHECK_PARAM(PARAM_SSPx(SSPx));
	
	if (SSPx == LPC_SSP0)
	{
    	PerClkEn (PER_NBR_SSP0);
		GPIO_Cfg(GPIO_PORT1_FAST,SSP0_SCK_PIN | SSP0_MISO_PIN | SSP0_MOSI_PIN,GPIO_OPT_FNCT_4);	 /* P1.20,P1.23,P1.24 FUNT_4 -- SCK0,MISO0,MOSI0 */
    } else if (SSPx == LPC_SSP1) 
	{
    	PerClkEn (PER_NBR_SSP1);
		GPIO_Cfg(GPIO_PORT0_FAST,SSP1_SCK_PIN | SSP1_MISO_PIN | SSP1_MOSI_PIN,GPIO_OPT_FNCT_3);	 /* P0.7,p0.8,p0.9 FUNT_3 -- MISO1,MOSI1 */
    } else 
	{
    	return;
    }
	
    /* config cr0 */
	tmp =  ((SSP_ConfigStruct->CPHA) | (SSP_ConfigStruct->CPOL) 
	    |   (SSP_ConfigStruct->FrameFormat) | (SSP_ConfigStruct->Databit)) & SSP_CR0_BITMASK;
	SSPx->CR0 = tmp;
	
	tmp = SSP_ConfigStruct->Mode & SSP_CR1_BITMASK;
	SSPx->CR1 = tmp;
	
	SSP_clock_set(SSPx, SSP_ConfigStruct->ClockRate); 
	
    // Enable SSP peripheral
	SSP_Cmd(LPC_SSP0, ENABLE);   
}

void SSP_DeInit(LPC_SSP_TypeDef* SSPx)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));

	if (SSPx == LPC_SSP0)
	{
    	PerClkDis(PER_NBR_SSP0);
    } else if (SSPx == LPC_SSP1) 
	{
    	PerClkDis(PER_NBR_SSP1);
    } else 
	{
    	return;
    }
}

/*
 * Name			  :	void SSP_LoopBackCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
 * Input          : none
 * Output         : none
 * Caller(s)      : app.
 * Return         : none
 * Notes          :	SSP LoopBack Mode enable and disable
 */
void SSP_LoopBackCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->CR1 |= CR1_LBM_EN;
	}
	else
	{
		SSPx->CR1 &= (~CR1_LBM_EN) & SSP_CR1_BITMASK;
	}
}

void SSP_DMACmd(LPC_SSP_TypeDef *SSPx, uint32_t DMAMode, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_DMA(DMAMode));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->DMACR |= DMAMode;
	}
	else
	{
		SSPx->DMACR &= (~DMAMode) & SSP_DMA_BITMASK;
	}
}

/*
 * Name			  :	void SSP0_Init(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : app
 * Return         : none
 * Notes          :	
 */
void SSP0_Init(void)
{
    SSP_CFG_Type SSP0_InitStruct;

    /* config init struct */
	SSP0_InitStruct.CPHA = CR0_CPHA_SECOND;
	SSP0_InitStruct.CPOL = CR0_CPOL_HI;
	SSP0_InitStruct.ClockRate = 1000000;
	SSP0_InitStruct.Databit = CR0_DSS_8BITS;
	SSP0_InitStruct.Mode = SSP_MASTER_MODE;
	SSP0_InitStruct.FrameFormat = CR0_FRF_SPI;

	SSP_Init(LPC_SSP0,&SSP0_InitStruct);
}

__asm CPU_INT32U W_REVERS(CPU_INT32U W)
{
    RBIT R0,R0
	bx lr
}
//不用结构体,直接定义寄存器地址,提高效率
void SSP0_SendData(CPU_INT16U Data)
{
	CPU_INT08U tmp = ((W_REVERS(Data)>>24) & 0x0000ff);
//	uart_printf(0,"revers:%.2x\r\n",tmp);
//    CPU_INT08U tmp = 0xff & Data; 
//
//	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55) << 1);
//    tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33) << 2);
//    tmp = ((tmp & 0xf0) >> 4) | ((tmp & 0x0f) << 4);
	
	while(!(REG_SSP0SR & SR_TNF_BITMASK));
	REG_SSP0DR = tmp;
	while(!(REG_SSP0SR & SR_TFE_BITMASK));   	
}

CPU_INT16U SSP0_ReceiveData(void)
{
    while(!(REG_SSP0SR & SR_RNE_BITMASK));
	return REG_SSP0DR;    
}

CPU_INT16U SSP0_WriteRead(CPU_INT16U Data)
{
	CPU_INT08U tmp;
	SSP0_SendData(Data);
	tmp = ((W_REVERS(SSP0_ReceiveData())>>24) & 0x0000ff);

	return tmp;
}

void SSP1_Init(void)
{
    SSP_CFG_Type SSP1_InitStruct;

    /* config init struct */
	SSP1_InitStruct.CPHA = CR0_CPHA_FIRST;
	SSP1_InitStruct.CPOL = CR0_CPOL_HI;
	SSP1_InitStruct.ClockRate = 1000000;
	SSP1_InitStruct.Databit = CR0_DSS_8BITS;
	SSP1_InitStruct.Mode = SSP_MASTER_MODE;
	SSP1_InitStruct.FrameFormat = CR0_FRF_SPI;

	SSP_Init(LPC_SSP1,&SSP1_InitStruct);
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/

