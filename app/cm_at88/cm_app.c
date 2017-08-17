/**
  ******************** (C) COPYRIGHT 2013 DJI **********************************
  *
  * @Project Name       : BL_WKM2_LED_IAP.uvproj
  * @File Name          : cm_app.c
  * @Environment        : keil mdk4.12/LPC1768/100M cclock
  * @Author&Date        : 2013-02-19 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	      
  */
#include <LPC17XX.H>                        /* LPC17xx definitions */

#include "../../cstartup/type.h"
#include "cm_lib.h"
#include "cm_i2c.h"
#include "cm_gpa.h"
#include "cm_app.h"

#define SNOffsetA			 	0x0050			//
#define SNOffsetA_End			0x00AF

/*
** extern varible
*/
extern unsigned char package_buf[256];
extern unsigned char package_length;

//ProductId buffer
unsigned char ProductId_t[11];
//SerialNumber buffer
unsigned char SerialNumber_t[33];

/*
** AT88SC secure chip initial
*/
void cm_init( void )
{
	// p0.27,p0.28 as gpio
	CM_CLK_GPIO;
    CM_DATA_GPIO;
	// Power on  AT88SC初始化时钟
	cm_FullPowerOn();
}

//add 2010-10-20
void get_key_at88(unsigned char *key_data)
{
	unsigned char *key_address;
	unsigned int   i;
	
	i = *((unsigned int *)0x02FC);
	if(i == 0x87654321 || i == 0x12345678 || i == 0x43218765)	//CRP1 或者 CRP2,CRP3
	{
		key_address = (unsigned char *)0x10007F60;
		for(i=0;i<8;i++)
		{
			*key_data++ = *key_address++;
		}
	}
}

void Active_At88sc(void)
{
   	unsigned char ucData[8];
	get_key_at88(ucData); 
    if(cm_ActiveSecurity(3, ucData, 1) != SUCCESS)
    {

    }
}

//add 2010-10-29
int get_ProductId(unsigned char *ProductId)
{
	unsigned int num,i;
	if(cm_ReadConfigZone(AT88SC_NC_ADDR,(unsigned char *)&num,4) != 0)
	{
//	  	uart_printf( 0, "read ProductId fail!\r\n" );
		return 0;
	}

	for(i=0;i<10;i++)
	{
	    ProductId_t[9-i] = num%10 + 48;
		num = num/10; 
	}
	ProductId_t[0]  = '1';
	ProductId_t[10] = '\0';
		
	return 1;
}

//add 2010-11-26
int get_SerialNumber(unsigned char *SerialNumber,unsigned short int length)
{
   if( cm_read_data(SerialNumber,SNOffsetA,length) != __TRUE)
   {
   		return __FALSE;
   }
   return __TRUE;
}

unsigned int g_version_loader,g_version_loader_reverse;
void read_loader_version(void)
{
	g_version_loader         = *(unsigned int*)0x10007F70;
	g_version_loader_reverse = *(unsigned int*)0x10007F74;

	if(g_version_loader != (~g_version_loader_reverse))
	{
		g_version_loader = 0x00000001;
	}	
}

unsigned char cm_read_data(unsigned char*BufferIn,unsigned short offset,unsigned short len)
{
    unsigned short start_zone,zone_offset,page_offset,read_num,tmp_read_num,onetime_read_num;//,i;
	
	if((offset + len > AT88SC6416_LEN)||(len == 0) ) return FALSE;
    start_zone = offset / AT88SC6416_ZONE_LEN ; //get start zone

	for(;len;len -= read_num,offset += read_num,start_zone++)  
    {
        zone_offset = offset%AT88SC6416_ZONE_LEN;
		read_num = AT88SC6416_ZONE_LEN - zone_offset;
		if(len < read_num)
		{
	    	read_num = len;
	    	//uart_printf( 0, "len < read_num!\r\n" );
		}
				
		if(cm_SetUserZone(start_zone, 0) != SUCCESS )
		{
			//uart_printf( 0, "cm Set User Zone Fail !\r\n" );
			return FALSE;
		}

        tmp_read_num = read_num;  //为offset += read_num,暂存
		for(;tmp_read_num;tmp_read_num -= onetime_read_num,zone_offset += onetime_read_num,BufferIn += onetime_read_num )
		{
			page_offset = zone_offset % AT88SC6416_PAGE_SIZE;
			onetime_read_num = AT88SC6416_PAGE_SIZE - page_offset;
			if(tmp_read_num < onetime_read_num)
			{
				onetime_read_num = tmp_read_num;
			}		
			if(cm_ReadSmallZone(zone_offset,BufferIn,onetime_read_num) != SUCCESS )
			{
				//uart_printf( 0, "cm_Read Zone %d !\r\n", start_zone );  
            	return __FALSE;
			}
		}
    }	
    return __TRUE;
}


/*
 *
 */
unsigned char ucData[16];
	//display config zone
unsigned char display_configzone(void)
{
	unsigned char tmp_addr = 0,i,j;
	for(i=0; i<15; i++)//0xF0
	{
		if(cm_ReadConfigZone(tmp_addr,ucData,0x10) != SUCCESS)
		{
			//VCOM_printf("cm_Read all fail!\r\n");
			return 1;    //err
		}
		for(j=0;j<16;j++)
		{
			//VCOM_printf( "%.2x ", ucData[j] );	
		}
		//VCOM_printf("\r\n");
		
		tmp_addr +=16;
	}
	return 0;	//success
}

/*
********************************************************************************
*                        THE END
********************************************************************************
*/
