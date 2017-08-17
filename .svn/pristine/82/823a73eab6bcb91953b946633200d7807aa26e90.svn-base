/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_flash_iap.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-17 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx flash iap,reference for xp40        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"
#define __FLASH_IAP_DEBUG

CPU_INT32U param_table[5];
CPU_INT32U result_table[5];

static void iap_entry ( CPU_INT32U param_tab[],CPU_INT32U result_tab[] )
{
    typedef void (*IAP)(CPU_INT32U [],CPU_INT32U []);
    IAP iap_t;

    iap_t = (IAP)IAP_ADDR;
    iap_t(param_tab,result_tab);
}

static void prepare_sector( CPU_INT32U start_sector,\
                            CPU_INT32U end_sector )
{
    param_table[0] = PREPARE_SECTOR_FOR_WRITE;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = IAP_CCLK;
    iap_entry(param_table,result_table);

	if(result_table[0] != 0)
	{
		uart_printf(0,"prepare failed.\r\n");
	}
}

static void erase_sector(CPU_INT32U start_sector,CPU_INT32U end_sector)
{
    CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();    
	param_table[0] = ERASE_SECTOR;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = IAP_CCLK;
    iap_entry(param_table,result_table);
	CPU_CRITICAL_EXIT();
	
	if(result_table[0] != 0)
	{
		uart_printf(0,"erase failed.\r\n");
	}
}

static CPU_INT08U write_data( CPU_INT32U flash_addr,\
                              CPU_INT32U *buf, CPU_INT32U length )
{
    CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();
    param_table[0] = COPY_RAM_TO_FLASH;
    param_table[1] = flash_addr;
    param_table[2] = (CPU_INT32U)buf;
    param_table[3] = length;
    param_table[4] = IAP_CCLK;
    iap_entry(param_table,result_table);
	CPU_CRITICAL_EXIT();
	
	if( result_table[0] == 0 )//cmd excute success
	{
	    return (DEF_TRUE);
	}
	else
	{
		uart_printf(0,"write failed.\r\n");
	    return (DEF_FALSE);
	}
}

static read_data( CPU_INT32U flash_addr,
                             CPU_INT08U *buf, CPU_INT32U length )
{
	CPU_INT08U *des = (CPU_INT08U *)flash_addr;
	while(length--)
	{
	    *buf++ = *des++;     
	}
}

static read_data32( CPU_INT32U flash_addr,
                             CPU_INT32U *buf, CPU_INT32U length )
{
   CPU_INT32U *des = (CPU_INT32U *)flash_addr;
   while(length)
   {
       *buf++ = *des++;
	   length -= 4;     
   }
}

CPU_INT32U user_code_present( CPU_INT32U start_sector,CPU_INT32U end_sector )
{
    param_table[0] = BLANK_CHECK_SECTOR;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    iap_entry(param_table,result_table);

	if( result_table[0] == 0 )//IAP_CMD_SUCCESS = 0
	{
	    return (DEF_FALSE);
	}
	else
	{
	    return (DEF_TRUE);
	}
}

CPU_INT32U compare_data( CPU_INT32U flash_addr,\
                         CPU_INT32U ram_addr,CPU_INT32U length )
{
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
	param_table[0] = COMPARE;
    param_table[1] = flash_addr;
    param_table[2] = ram_addr;
	param_table[3] = length;
    iap_entry(param_table,result_table);
	CPU_CRITICAL_EXIT();
	
	if(result_table[0] != 0)
    {
        return DEF_FALSE; 
    }
	return DEF_TRUE;
}

static CPU_INT32U wrBuf[1024] __attribute__ ((section("ETHERNET_RAM"),aligned));//存储数量大时，增加缓冲区数量

CPU_INT08U write_flash_data ( CPU_INT32U OffSet, 
                              CPU_INT08U* buf, CPU_INT32U length )
{
	CPU_INT32U  flash_sector;
	CPU_INT32U  cnt;
	
	if( (OffSet+length) > MAX_LENGTH )
	{
	    return DEF_FALSE;
	}
	
	if(OffSet < sector_size)//FIRST SECTOR
	{
	    flash_sector = DATA_SECTOR_START;
		cnt = (OffSet+length)<sector_size ? length : (sector_size - OffSet);
		
		read_data32(FLASH_DATA_ADDR_START,&wrBuf[0],1024); //1024*4
		memcpy((CPU_INT08U *)&wrBuf[0]+OffSet,buf,cnt);

		prepare_sector(flash_sector,flash_sector);   //select sector
	    erase_sector(flash_sector,flash_sector);	   //erase sector for write
	
	    prepare_sector(flash_sector,flash_sector);   //select sector
	    
		if(write_data(FLASH_DATA_ADDR_START,&wrBuf[0],4096) != DEF_TRUE )
		{
		    return DEF_FALSE;
		}

		length -= cnt;
		buf += cnt;
		OffSet += cnt;
		OffSet -= sector_size;
		
		uart_printf(0,"h1\r\n");
		uart_printf(0,"length: %d\r\n",length); 
	}
    
	if(length > 0)//SECOND SECTOR
	{
	    flash_sector = DATA_SECTOR_END;
		cnt = length;
		
		read_data32(FLASH_DATA_ADDR_SEC,&wrBuf[0],1024);
		memcpy((CPU_INT08U *)&wrBuf[0]+OffSet,buf,cnt);

		prepare_sector(flash_sector,flash_sector);   //select sector
	    erase_sector(flash_sector,flash_sector);	   //erase sector for write
	
	    prepare_sector(flash_sector,flash_sector);   //select sector
		if(write_data(FLASH_DATA_ADDR_SEC,&wrBuf[0],4096) != DEF_TRUE )
		{
		    return DEF_FALSE;
		}
	}
	return DEF_TRUE;
}


CPU_INT08U read_flash_data(CPU_INT32U OffSet,
                           CPU_INT08U* buf ,CPU_INT32U length)
{	
	if( (OffSet+length) > MAX_LENGTH )
	{
	    return DEF_FALSE;
	}

    read_data(FLASH_DATA_ADDR_START+OffSet,buf,length);
	return DEF_TRUE;
}

 

/*
 * param[in]   addr: address of flash
 * param[in]   buf : buf to be write
 * param[in]   length: 
 * Return      true or false
 * Notes       Setup clock rate for I2C peripheral
 */
CPU_INT08U write_cali_data(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length )
{
    CPU_INT32U 	OffSet;

	if((CPU_INT32U)addr < FLASH_DATA_ADDR_START) return DEF_FALSE;
	OffSet = (CPU_INT32U)addr - FLASH_DATA_ADDR_START;

	return write_flash_data(OffSet,buf,length);
}

CPU_INT08U set_cali_value ( CPU_INT08U *addr, 
                             CPU_INT08U value, CPU_INT32U length )
{
	CPU_INT32U  flash_sector;
	CPU_INT32U  cnt;

    if( ((CPU_INT32U)addr < FLASH_DATA_ADDR_START) ||
	    (((CPU_INT32U)addr + length) > FLASH_DATA_ADDR_END) ) return DEF_FALSE;	
	
	if((CPU_INT32U)addr < FLASH_DATA_ADDR_SEC)//FIRST SECTOR
	{
	    flash_sector = DATA_SECTOR_START;
		cnt = ((CPU_INT32U)addr+length)<FLASH_DATA_ADDR_SEC ? length : (FLASH_DATA_ADDR_SEC - (CPU_INT32U)addr);
		
		read_data32(FLASH_DATA_ADDR_START,&wrBuf[0],1024); //1024*4
		memset((CPU_INT08U *)&wrBuf[0]+((CPU_INT32U)addr-FLASH_DATA_ADDR_START),value,cnt);

		prepare_sector(flash_sector,flash_sector);   //select sector
	    erase_sector(flash_sector,flash_sector);	   //erase sector for write
	
	    prepare_sector(flash_sector,flash_sector);   //select sector
	    
		if(write_data(FLASH_DATA_ADDR_START,&wrBuf[0],4096) != DEF_TRUE )
		{
		    return DEF_FALSE;
		}

		length -= cnt;
		addr += cnt;
		
	}
    
	if(length > 0)//SECOND SECTOR
	{
	    flash_sector = DATA_SECTOR_END;
		cnt = length;
		
		read_data32(FLASH_DATA_ADDR_SEC,&wrBuf[0],1024);
		memset((CPU_INT08U *)&wrBuf[0]+((CPU_INT32U)addr-FLASH_DATA_ADDR_SEC),value,cnt);

		prepare_sector(flash_sector,flash_sector);   //select sector
	    erase_sector(flash_sector,flash_sector);	   //erase sector for write
	
	    prepare_sector(flash_sector,flash_sector);   //select sector
		if(write_data(FLASH_DATA_ADDR_SEC,&wrBuf[0],4096) != DEF_TRUE )
		{
		    return DEF_FALSE;
		}
		uart_printf(0,"h2\r\n");
	}
	return DEF_TRUE;
}

  
CPU_INT08U write_flash_ram(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length,CPU_INT32U CMD)
{
    CPU_INT32U  flash_sector;
	CPU_INT32U  cnt;

    if( ((CPU_INT32U)addr < FLASH_DATA_ADDR_START) ||
	    (((CPU_INT32U)addr + length) > FLASH_DATA_ADDR_END) ) return DEF_FALSE;
		
	
	if((CPU_INT32U)addr < FLASH_DATA_ADDR_SEC)//FIRST SECTOR
	{
	    flash_sector = DATA_SECTOR_START;
		cnt = ((CPU_INT32U)addr+length)<FLASH_DATA_ADDR_SEC ? length : (FLASH_DATA_ADDR_SEC - (CPU_INT32U)addr);
		
		if(CMD == FLASHIAP_CMD_W_FLASH_START)
		{
		    read_data32(FLASH_DATA_ADDR_START,&wrBuf[0],1024); //1024*4
//			uart_printf(0,"sector 1 start\r\n");
		}
//		uart_printf(0,"cpy 1\r\n");
		memcpy((CPU_INT08U *)&wrBuf[0]+((CPU_INT32U)addr-FLASH_DATA_ADDR_START),buf,cnt);
//		uart_printf(0,"cpy 2\r\n");
		if( (CMD == FLASHIAP_CMA_W_FLASH_END) || ((CPU_INT32U)addr+length) >= FLASH_DATA_ADDR_SEC)
		{
//			uart_printf(0,"m0\r\n");
			prepare_sector(flash_sector,flash_sector);   //select sector
		    erase_sector(flash_sector,flash_sector);	   //erase sector for write
//			uart_printf(0,"m1\r\n");
		    prepare_sector(flash_sector,flash_sector);   //select sector
//		    uart_printf(0,"m2\r\n");
			if(write_data(FLASH_DATA_ADDR_START,&wrBuf[0],4096) != DEF_TRUE )
			{
//				uart_printf(0,"fail\r\n");
			    return DEF_FALSE;
			}

//			uart_printf(0,"sector 1 end\r\n");
		}
		length -= cnt;
		addr += cnt;
		buf += cnt;

//		uart_printf(0,"h1\r\n");
//		uart_printf(0,"length: %d\r\n",length); 
	}
	
	if(length > 0)//SECOND SECTOR
	{
	    flash_sector = DATA_SECTOR_END;
		cnt = length;
		
		if( (CMD == FLASHIAP_CMD_W_FLASH_START) || ((INT32U)addr == FLASH_DATA_ADDR_SEC) )
		{
		    read_data32(FLASH_DATA_ADDR_SEC,&wrBuf[0],1024); //1024*4
//			uart_printf(0,"sector 2 start\r\n");
		}

		memcpy((CPU_INT08U *)&wrBuf[0]+((CPU_INT32U)addr-FLASH_DATA_ADDR_SEC),buf,cnt);

		if( (CMD == FLASHIAP_CMA_W_FLASH_END) || ((CPU_INT32U)addr+length) == FLASH_DATA_ADDR_END)
		{
		    prepare_sector(flash_sector,flash_sector);   //select sector
	        erase_sector(flash_sector,flash_sector);	   //erase sector for write
	
	        prepare_sector(flash_sector,flash_sector);   //select sector
		    if(write_data(FLASH_DATA_ADDR_SEC,&wrBuf[0],4096) != DEF_TRUE )
		    {
		        return DEF_FALSE;
		    }
//			uart_printf(0,"sector 2 end\r\n");
		}
//		uart_printf(0,"h2\r\n");
	}
//	uart_printf(0,"ok\r\n");
	return DEF_TRUE;	  
}

CPU_INT08U write_cfg_flash_ram(CPU_INT08U *addr, 
                           CPU_INT08U *buf, CPU_INT32U length,CPU_INT32U CMD)
{
    CPU_INT32U  flash_sector;
	CPU_INT32U  cnt;

    if( ((CPU_INT32U)addr < FLASH_FIR_CFG_ADDR_START) ||
	    (((CPU_INT32U)addr + length) > FLASH_FIR_CFG_ADDR_END) ) return DEF_FALSE;
		
	flash_sector = FIR_CFG_SECTOR_START;
	
    cnt = length;
	if(CMD == FLASHIAP_CMD_W_FLASH_START)
	{
	    read_data32(FLASH_FIR_CFG_ADDR_START,&wrBuf[0],1024); //1024*4
	//			uart_printf(0,"sector 1 start\r\n");
	}
	//		uart_printf(0,"cpy 1\r\n");
	memcpy((CPU_INT08U *)&wrBuf[0]+((CPU_INT32U)addr-FLASH_FIR_CFG_ADDR_START),buf,cnt);
	//		uart_printf(0,"cpy 2\r\n");
	if( (CMD == FLASHIAP_CMA_W_FLASH_END) || ((CPU_INT32U)addr+length) >= FLASH_FIR_CFG_ADDR_END)
	{
	//			uart_printf(0,"m0\r\n");
		prepare_sector(flash_sector,flash_sector);   //select sector
	    erase_sector(flash_sector,flash_sector);	   //erase sector for write
	//			uart_printf(0,"m1\r\n");
	    prepare_sector(flash_sector,flash_sector);   //select sector
	//		    uart_printf(0,"m2\r\n");
		if(write_data(FLASH_FIR_CFG_ADDR_START,&wrBuf[0],4096) != DEF_TRUE )
		{
	//				uart_printf(0,"fail\r\n");
		    return DEF_FALSE;
		}
	
	//			uart_printf(0,"sector 1 end\r\n");

	//		uart_printf(0,"h1\r\n");
	//		uart_printf(0,"length: %d\r\n",length); 
	}
	
	return DEF_TRUE;	  
}
/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/

