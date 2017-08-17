/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_ssp.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-02-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx ssp module,reference for nxp        
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPC17XX_SSP_H__
#define __LPC17XX_SSP_H__

/* Private define ------------------------------------------------------------*/
/* SSP pin bit field */
#define SSP0_SSEL0_PIN		    DEF_BIT_21		/* p1.21 FUNT_4 -- SSEL0 */
#define SSP0_SCK_PIN  	        DEF_BIT_20      /* p1.20 FUNT_4 -- SCK0 */
#define SSP0_MISO_PIN	        DEF_BIT_23      /* P1.23,P1.24 FUNT_4 -- MISO0,MOSI0 */
#define SSP0_MOSI_PIN			DEF_BIT_24

//#define SSP0_SSEL0_PIN		    DEF_BIT_16		/* p0.16 FUNT_3 -- SSEL0 */
//#define SSP0_SCK_PIN  	        DEF_BIT_15      /* p0.15 FUNT_3 -- SCK0 */
//#define SSP0_MISO_PIN	        DEF_BIT_17      /* P0.17,P0.18 FUNT_3 -- MISO0,MOSI0 */
//#define SSP0_MOSI_PIN			DEF_BIT_18

#define SSP1_SCK_PIN  	        DEF_BIT_07      /* p0.7 FUNT_3 -- SCK1 */
#define SSP1_MISO_PIN	        DEF_BIT_08      /* p0.8,p0.9 FUNT_3 -- MISO1,MOSI1 */
#define SSP1_MOSI_PIN  			DEF_BIT_09

/* SSP CR0 define */
#define CR0_DSS_BIT_SHIFT		0
#define CR0_DSS_BIT_NUMBERS		4u
#define CR0_DSS_8BITS		    DEF_BIT_MASK(0x07, CR0_DSS_BIT_SHIFT)

#define CR0_FRF_BIT_SHIFT		4u
#define CR0_FRF_BIT_NUMBERS		2u
#define CR0_FRF_SPI             0
#define CR0_FRF_TI				1u
#define CR0_FRF_MCROWIRE		2u

#define CR0_CPOL_BIT_SHIFT		6u
#define CR0_CPOL_BIT_NUMBERS	1u
#define CR0_CPOL_LO				0
#define CR0_CPOL_HI				DEF_BIT_06

#define CR0_CPHA_BIT_SHIFT		7u
#define CR0_CPHA_BIT_NUMBERS	1u
#define CR0_CPHA_FIRST			0
#define CR0_CPHA_SECOND			DEF_BIT_07

#define CR0_SCR_BIT_SHIFT       8u
#define CR0_SCR_BIT_NUMBERS		8u

#define SSP_CR0_BITMASK		    ((CPU_INT32U)(0xFFFF))

/* SSP CR1 define */
#define CR1_LBM_BIT_SHIFT		0
#define CR1_LBM_BIT_NUMBERS		1u
#define CR1_LBM_DIS			    0
#define CR1_LBM_EN			    DEF_BIT_00


#define CR1_SSE_BIT_SHIFT		1u
#define CR1_SSE_BIT_NUMBERS		1u
#define CR1_SSE_DIS		        0
#define CR1_SSE_EN		        DEF_BIT_01

#define CR1_MS_BIT_SHIFT		2u
#define CR1_MS_BIT_NUMBERS		1u
#define CR1_MS_DIS				DEF_BIT_02
#define CR1_MS_EN				0
#define SSP_MASTER_MODE			CR1_MS_EN
#define SSP_SLAVE_MODE			CR1_MS_DIS


#define CR1_SOD_BIT_SHIFT		3u
#define CR1_SOD_BIT_NUMBERS		1u

#define SSP_CR1_BITMASK		    ((CPU_INT32U)(0x0F))

/* status register */
#define SR_TFE_BIT_SHIFT		0
#define SR_TFE_BIT_NUMBERS		1
#define SR_TFE_BITMASK		    DEF_BIT_00

#define SR_TNF_BIT_SHIFT		1
#define SR_TNF_BIT_NUMBERS		1
#define SR_TNF_BITMASK		    DEF_BIT_01

#define SR_RNE_BIT_SHIFT		2
#define SR_RNE_BIT_NUMBERS		1
#define SR_RNE_BITMASK		    DEF_BIT_02

#define SR_RFF_BIT_SHIFT		3
#define SR_RFF_BIT_NUMBERS		1
#define SR_RFF_BITMASK		    DEF_BIT_03

#define SR_BSY_BIT_SHIFT		4
#define SR_BSY_BIT_NUMBERS		1
#define SR_BSY_BITMASK		    DEF_BIT_04

/* SSP CPSR */
#define CPSR_CPSDVSR_BIT_SHIFT	     0
#define CPSR_CPSDVSR_BIT_NUMBERS	 8u

#define SSP_CPSR_BITMASK	         ((CPU_INT32U)(0xFF))

/* SSP  DMACR */
#define SSP_DMACR_RXDMA_EN       	 DEF_BIT_00
#define SSP_DMACR_TXDMA_EN		     DEF_BIT_01
#define SSP_DMA_BITMASK		         ((CPU_INT32U)(0x03))

/* data register address define */
#define REG_SSP0DR   	        (*(volatile unsigned long *)0x40088008)
#define REG_SSP1DR		        (*(volatile unsigned long *)0x40030008)
#define REG_SSP0SR		        (*(volatile unsigned long *)0x4008800C)
#define REG_SSP1SR		        (*(volatile unsigned long *)0x4003000C)




/* CHECK PARAMETER DEFINITIONS -----------------------------------------------*/
/* Macro to determine if it is valid SSP port number */
#define PARAM_SSPx(n)	        ((((CPU_INT32U *)n)==((CPU_INT32U *)LPC_SSP0)) \
                                || (((CPU_INT32U *)n)==((CPU_INT32U *)LPC_SSP1)))


/* Exported types ------------------------------------------------------------*/
/* brief SSP configuration structure */
typedef struct {
	CPU_INT32U Databit; 		/** Databit number, should be SSP_DATABIT_x,
							where x is in range from 4 - 16 */
	CPU_INT32U CPHA;			/** Clock phase, should be:
							- SSP_CPHA_FIRST: first clock edge
							- SSP_CPHA_SECOND: second clock edge */
	CPU_INT32U CPOL;			/** Clock polarity, should be:
							- SSP_CPOL_HI: high level
							- SSP_CPOL_LO: low level */
	CPU_INT32U Mode;			/** SSP mode, should be:
							- SSP_MASTER_MODE: Master mode
							- SSP_SLAVE_MODE: Slave mode */
	CPU_INT32U FrameFormat;	/** Frame Format:
							- SSP_FRAME_SPI: Motorola SPI frame format
							- SSP_FRAME_TI: TI frame format
							- SSP_FRAME_MICROWIRE: National Microwire frame format */
	CPU_INT32U ClockRate;		/** Clock rate,in Hz */
} SSP_CFG_Type;

void SSP_Cmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_Init(LPC_SSP_TypeDef *SSPx,SSP_CFG_Type *SSP_ConfigStruct);
void SSP_DeInit(LPC_SSP_TypeDef* SSPx);
void SSP_LoopBackCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_DMACmd(LPC_SSP_TypeDef *SSPx, uint32_t DMAMode, FunctionalState NewState);
void SSP0_Init(void);
void SSP0_SendData(CPU_INT16U Data);
CPU_INT16U SSP0_ReceiveData(void);
CPU_INT16U SSP0_WriteRead(CPU_INT16U Data);
void SSP1_Init(void);

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
#endif
