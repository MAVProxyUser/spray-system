/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_i2c.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-01 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx i2c module,reference for nxp        
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPC17XX_I2C_H__
#define __LPC17XX_I2C_H__

/* Private define ------------------------------------------------------------*/
/* I2C PORT PIN define */
#define I2C0_PORT         (GPIO_PORT0_FAST)
#define I2C0_PINS         (DEF_BIT_27 | DEF_BIT_28)	   /*p0.27,p0.28,FNCT2 --SDA0,SCL0*/


/* I2C pin bit field */
/* I2CONSET define */
#define I2CONSET_AA       DEF_BIT_02
#define I2CONSET_SI       DEF_BIT_03
#define I2CONSET_STO      DEF_BIT_04
#define I2CONSET_STA      DEF_BIT_05
#define I2CONSET_I2EN     DEF_BIT_06

/* I2CONCLR define */
#define I2CONCLR_AAC      DEF_BIT_02
#define I2CONCLR_SIC      DEF_BIT_03
#define I2CONCLR_STAC     DEF_BIT_05
#define I2CONCLR_I2ENC    DEF_BIT_06

/* I2C status values */
#define I2C_SETUP_STATUS_ARBF   (1<<8)	/**< Arbitration false */
#define I2C_SETUP_STATUS_NOACKF (1<<9)	/**< No ACK returned */
#define I2C_SETUP_STATUS_DONE   (1<<10)	/**< Status DONE */

/********************************************************************//**
 * I2C Status Code definition (I2C Status register)
 *********************************************************************/
/* Return Code in I2C status register */
#define I2C_STAT_CODE_BITMASK		((0xF8))

/* I2C return status code definitions ----------------------------- */

/** No relevant information */
#define I2C_I2STAT_NO_INF						((0xF8))

/* Master transmit mode -------------------------------------------- */

#define I2C_I2STAT_M_TX_START					((0x08))   /* A start condition has been transmitted */

#define I2C_I2STAT_M_TX_RESTART					((0x10))   /* A repeat start condition has been transmitted */

#define I2C_I2STAT_M_TX_SLAW_ACK				((0x18))   /* SLA+W has been transmitted, ACK has been received */

#define I2C_I2STAT_M_TX_SLAW_NACK				((0x20))   /* SLA+W has been transmitted, NACK has been received */

#define I2C_I2STAT_M_TX_DAT_ACK					((0x28))   /* Data has been transmitted, ACK has been received */

#define I2C_I2STAT_M_TX_DAT_NACK				((0x30))   /* Data has been transmitted, NACK has been received */

#define I2C_I2STAT_M_TX_ARB_LOST				((0x38))   /* Arbitration lost in SLA+R/W or Data bytes */

/* Master receive mode -------------------------------------------- */

#define I2C_I2STAT_M_RX_START					((0x08))    /* A start condition has been transmitted */

#define I2C_I2STAT_M_RX_RESTART					((0x10))    /* A repeat start condition has been transmitted */

#define I2C_I2STAT_M_RX_ARB_LOST				((0x38))    /* Arbitration lost */

#define I2C_I2STAT_M_RX_SLAR_ACK				((0x40))    /* SLA+R has been transmitted, ACK has been received */

#define I2C_I2STAT_M_RX_SLAR_NACK				((0x48))    /* SLA+R has been transmitted, NACK has been received */

#define I2C_I2STAT_M_RX_DAT_ACK					((0x50))    /* Data has been received, ACK has been returned */

#define I2C_I2STAT_M_RX_DAT_NACK				((0x58))    /* Data has been received, NACK has been return */

/* Slave receive mode -------------------------------------------- */

#define I2C_I2STAT_S_RX_SLAW_ACK				((0x60))    /* Own slave address has been received, ACK has been returned */


#define I2C_I2STAT_S_RX_ARB_LOST_M_SLA			((0x68))    /* Arbitration lost in SLA+R/W as master */

//#define I2C_I2STAT_S_RX_SLAW_ACK				((0x68))    /* Own SLA+W has been received, ACK returned */


#define I2C_I2STAT_S_RX_GENCALL_ACK				((0x70))    /* General call address has been received, ACK has been returned */


#define I2C_I2STAT_S_RX_ARB_LOST_M_GENCALL		((0x78))    /* Arbitration lost in SLA+R/W (GENERAL CALL) as master */

//#define I2C_I2STAT_S_RX_GENCALL_ACK				((0x78)) /** General call address has been received, ACK has been returned */



#define I2C_I2STAT_S_RX_PRE_SLA_DAT_ACK			((0x80))    /* Previously addressed with own SLV address,Data has been received, ACK has been return */

 
#define I2C_I2STAT_S_RX_PRE_SLA_DAT_NACK		((0x88))	/* Previously addressed with own SLA, Data has been received and NOT ACK has been return */

 		
#define I2C_I2STAT_S_RX_PRE_GENCALL_DAT_ACK		((0x90))	/* Previously addressed with General Call, Data has been received and ACK has been return */


#define I2C_I2STAT_S_RX_PRE_GENCALL_DAT_NACK	((0x98))	/* Previously addressed with General Call, Data has been received and NOT ACK has been return */
/** A STOP condition or repeated START condition has
 * been received while still addressed as SLV/REC
 * (Slave Receive) or SLV/TRX (Slave Transmit) */
#define I2C_I2STAT_S_RX_STA_STO_SLVREC_SLVTRX	((0xA0))

/** Slave transmit mode */

#define I2C_I2STAT_S_TX_SLAR_ACK				((0xA8))    /* Own SLA+R has been received, ACK has been returned */


#define I2C_I2STAT_S_TX_ARB_LOST_M_SLA			((0xB0))	/* Arbitration lost in SLA+R/W as master */

//#define I2C_I2STAT_S_TX_SLAR_ACK				((0xB0))	/* Own SLA+R has been received, ACK has been returned */

		
#define I2C_I2STAT_S_TX_DAT_ACK					((0xB8))	/* Data has been transmitted, ACK has been received */

#define I2C_I2STAT_S_TX_DAT_NACK				((0xC0))	/* Data has been transmitted, NACK has been received */

#define I2C_I2STAT_S_TX_LAST_DAT_ACK			((0xC8))	/* Last data byte in I2DAT has been transmitted (AA = 0),ACK has been received */

#define I2STAT_CODE_BITMASK   ((0xF8))

#define I2DAT_BITMASK		  ((0xFF))						/* Mask for I2DAT register*/

/* Time out in case of using I2C slave mode */
#define I2C0_CLOCK                 100000u
#define I2C0_MAX_TIMEOUT           10*I2C0_CLOCK
   


/* Macro to determine if it is valid SSP port number */
#define PARAM_I2Cx(n)	((((CPU_INT32U *)n)==((CPU_INT32U *)LPC_I2C0)) \
|| (((CPU_INT32U *)n)==((CPU_INT32U *)LPC_I2C1)) \
|| (((CPU_INT32U *)n)==((CPU_INT32U *)LPC_I2C2)))

/* public function declaration -----------------------------------------------*/
void I2C0_Init(void);
CPU_INT32U I2C0_SendByte (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U dat);
CPU_INT32U I2C0_RcvByte (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *pdat);
CPU_INT32U I2C0_SendBytes (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *s, CPU_INT32U num);
CPU_INT32U I2C0_RcvBytes (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *s, CPU_INT32U num);

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
#endif
