/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_flash_iap.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-14 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx flash iap,reference for xp40        
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __LPC17XX_FLASH_IAP_H__
#define  __LPC17XX_FLASH_IAP_H__

/*
**  enum const 
*/
typedef enum 
{
	PREPARE_SECTOR_FOR_WRITE=50,
	COPY_RAM_TO_FLASH=51,
	ERASE_SECTOR=52,
	BLANK_CHECK_SECTOR=53,
	READ_PART_ID=54,
	READ_BOOT_VER=55,
	COMPARE=56,
	REINVOKE_ISP=57,
	READ_DEVICE_SN=58
}IAP_Command_Code;

#define  IAP_CCLK                 100000u       /* 100,000 KHz for lpc17xx */
#define  IAP_ADDR                 0x1FFF1FF1    /* IAP FUNTION ENTRY,m3 thumb only */
//#define  IAP_CMD_SUCCESS          0

/* flash area manage */
#define  FLASH_DATA_ADDR_START    0x00001000    /* FLASH DATA area start address */
#define  FLASH_DATA_ADDR_SEC      0x00002000    /* SECOND FLASH DATA ADDR */
#define  FLASH_DATA_ADDR_END      0x00002FFF	/* FLASH DATA END ADDR */
#define  MAX_LENGTH				  8192u
#define  sector_size			  4096u

#define  DATA_SECTOR_START        1u
#define  DATA_SECTOR_END          2u

#define FLASHIAP_CMD_W_FLASH_START    0x01
#define FLASHIAP_CMD_W_FLASH_RAM      0x02 
#define FLASHIAP_CMA_W_FLASH_END		 0x03

/* for fir cof */
#define  FLASH_FIR_CFG_ADDR_START    0x00003000    /* FLASH DATA area start address */
#define  FLASH_FIR_CFG_ADDR_END      0x00003FFF	/* FLASH DATA END ADDR */
#define  FIR_CFG_SECTOR_START           3u

/* LPC17XX flash erea map */
  		
/* Function declaration ------------------------------------------------------*/
CPU_INT08U write_cali_data(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length );

CPU_INT08U write_flash_ram(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length,CPU_INT32U CMD);

CPU_INT08U write_cfg_flash_ram(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length,CPU_INT32U CMD);

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
#endif

