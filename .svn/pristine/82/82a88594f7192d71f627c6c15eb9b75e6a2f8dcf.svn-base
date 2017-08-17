/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       ：xp60.uvproj
  * @File Name          : lpc17xx_i2c.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-03-01 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx ssp module,reference for nxp and 3.1 i2c drivers        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* 定义用于和I2C中断传递信息的全局变量                                         */
volatile CPU_INT08U 	I2C_sla;	        /* I2C器件从地址 			       */
volatile CPU_INT32U	    I2C_suba;		    /* I2C器件内部子地址 			   */
volatile CPU_INT08U 	I2C_suba_num;		/* I2C子地址字节数			       */
volatile CPU_INT08U     *I2C_buf;        	/* 数据缓冲区指针  			       */
volatile CPU_INT32U     I2C_num;		    /* 要读取/写入的数据个数 		   */
volatile CPU_INT08U 	I2C_end;		    /* I2C总线结束标志：结束总线是置1  */
/* 一定要考虑没有子地址的情况 */
volatile CPU_INT08U     I2C_suba_en;        /* 0：子地址已发送
                                               1：读操作，子地址未发送
                                               2：写操作，子地址未发送         */

void I2C0_MasterISR_Handler (void)
{
    switch (LPC_I2C0->I2STAT & 0xF8)                /* 根据状态码进行相应的处理                   */
    {	
        case 0x08:	                                /* 已发送起始条件，主发送和主接收都有装入SLA+W或者SLA+R */
        {
		    if(I2C_suba_en == 1)                    /* 读操作，子地址未发送 			          */	
      		{
			    LPC_I2C0->I2DAT = I2C_sla & 0xFE; 		/* 先写入SLA+W 				              */
		    }
			else	
            {
	    	    LPC_I2C0->I2DAT    = I2C_sla;        		/* 否则直接发送从机地址               */       
    	    }
			LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC); /* 清零SI位和STA位    	      */
		}	
		break;

        case 0x10:	                                /* 已发送重复起始条件，主发送和主接收都有 	  */
        {
			LPC_I2C0->I2DAT    = I2C_sla;           /* 重起总线后，重发从地址                     */
    		LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位                */
    	}
		break;
    
  		case 0x18:									/* SLA+W 已经发送，已接收ACK                  */
  		case 0x28:	                                /* 已发送I2DAT中的数据，已接收ACK             */
	    {
			if (I2C_suba_en == 0)
	    	{
		      	if (I2C_num > 0)                    /* 还有数据未发送                             */  
		      	{	
			        LPC_I2C0->I2DAT = *I2C_buf++;   /* 发送数据                                   */
			        LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC); /* 清零SI位和STA位       */
			        I2C_num--;
		      	}
		      	else	                            /* 没有数据发送了                             */
		      	{
		        	LPC_I2C0->I2CONSET = I2CONSET_STO;	/* 置位STO，停止总线			          */
		        	LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);	/* 清零SI位和STA位    */
		        	I2C_end = 1;				    /* 总线已经停止 			                  */
		      	}
	    	}
	    
			if(I2C_suba_en == 1)	                /* 若是指定地址读，发送子地址后重启总线	*/
			{ 
			    if (I2C_suba_num == 2)              /* 子地址未发送完，发送子地址高位             */
			    {	
			        LPC_I2C0->I2DAT = ((I2C_suba >> 8) & 0xFF);
			        LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位    	  */
			    	I2C_suba_num--;
			    	break;	
			  	} 
			  	if (I2C_suba_num == 1)              /* 子地址未发送完，发送子地址低位             */
			  	{	
			    	LPC_I2C0->I2DAT = (I2C_suba & 0xFF);
			    	LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位        */
			    	I2C_suba_num--;
			    	break;	
			  	} 
			  	if (I2C_suba_num == 0)              /* 子地址发送完毕，重新启动总线               */                      
			  	{	
					LPC_I2C0->I2CONSET = I2CONSET_STA;	/* 置位STA    			                  */
			    	LPC_I2C0->I2CONCLR = I2CONCLR_SIC;	/* 清零SI位    			                  */
			    	I2C_suba_en = 0;     			/* 子地址己发送        			              */
			    	break;
			  	}            
			}
			
			if (I2C_suba_en == 2)	                /* 若是指定地址写，先发送子地址        	      */
			{	
			  	if (I2C_suba_num == 2)              /* 子地址未发送完，发送子地址高位             */
			  	{	
			    	LPC_I2C0->I2DAT = ((I2C_suba >> 8) & 0xFF);        
			    	LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位   	  */
			    	I2C_suba_num--;
			    	break;
			  	}
			  	if (I2C_suba_num == 1)              /* 子地址未发送完，发送子地址低位             */
			  	{	
			    	LPC_I2C0->I2DAT    = (I2C_suba & 0xFF);
			    	LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位    	  */
			    	I2C_suba_num--;
			    	I2C_suba_en = 0;     			/* 子地址己发送        			*/
			    	break;
			  	}
			}
		}
		break;
    
		case 0x40:	                                /* 已发送SLA+R,已接收ACK                      */
		{
			if (I2C_num <= 1)	                    /* 如果是最后一个字节                         */				
			{  
			    LPC_I2C0->I2CONCLR = I2CONCLR_AAC;  /* 下次发送非应答信号 */
			}
			else	                                /* 否则 */	
			{
			    LPC_I2C0->I2CONSET = I2CONSET_AA;	/* 下次发送应答信号 		                  */
			    
			} 
			LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位 */
		}
		break;
    
		case 0x20:	                                /* 已发送SLA+W,已接收非应答                   */
		case 0x30:	                                /* 已发送I2DAT中的数据，已接收非应答          */
		case 0x38:	                                /* 在SLA+R/W或数据字节中丢失仲裁              */
		case 0x48:	                                /* 已发送SLA+R,已接收非应答                   */
		{
		    LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC); /* 清零SI位和STA位 */
		    I2C_end = 0xFF;                         /* 总线异常标志                               */
		}
		break;   				
    
		case 0x50:	                                /* 已接收数据字节，已返回ACK                  */
		{
			*I2C_buf++ = LPC_I2C0->I2DAT;           /* 把接收的字节放入缓冲区                     */    
			I2C_num--;
			if (I2C_num == 1)                       /* 接收最后一个字节                           */ 	
			{
			    LPC_I2C0->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC); /* 清除STA,SI,AA标志位*/ 
			}
			else
			{  	
			    LPC_I2C0->I2CONSET = I2CONSET_AA;	/* 置位AA*/
			    LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);/* 清零SI位和STA位*/
			}
		}
		break;
    
		case 0x58:	                                      /* 已接收数据字节，已返回非应答*/
		{
			*I2C_buf++ = LPC_I2C0->I2DAT;     			  /* 读取最后一字节数据*/
			LPC_I2C0->I2CONSET = I2CONSET_STO;        			  /* 结束总线*/
			LPC_I2C0->I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);  /* 清零SI位和STA位*/
			I2C_end = 1;                                          /* 总线已经停止*/
		}
		break;

	    default:
	    {
		    LPC_I2C0->I2CONCLR = I2CONCLR_SIC;
	    }
		break;
	}
}

/*
 * param[in]   I2Cx: I2C peripheral selected, should be:
 * 			   - LPC_I2C0
 * 			   - LPC_I2C1
 * 			   - LPC_I2C2
 * param[in]   target_clock : clock of I2C (Hz) 
 * Return      None
 * Notes       Setup clock rate for I2C peripheral
 */
static void I2C_SetClock (LPC_I2C_TypeDef *I2Cx, CPU_INT32U target_clock)
{
	CPU_INT32U temp = 0;

	CHECK_PARAM(PARAM_I2Cx(I2Cx));

	// Get PCLK of I2C controller
	if (I2Cx == LPC_I2C0)
	{
		temp = PerClkFreqGet (PER_NBR_I2C0) / target_clock;
	}
	else if (I2Cx == LPC_I2C1)
	{
		temp = PerClkFreqGet (PER_NBR_I2C1) / target_clock;
	}
	else if (I2Cx == LPC_I2C2)
	{
		temp = PerClkFreqGet (PER_NBR_I2C2) / target_clock;
	}

	/* Set the I2C clock value to register */
	I2Cx->I2SCLH = (CPU_INT32U)(temp / 2);
	I2Cx->I2SCLL = (CPU_INT32U)(temp - I2Cx->I2SCLH);
}

void I2C_Init(LPC_I2C_TypeDef *I2Cx, CPU_INT32U clockrate)
{
	CHECK_PARAM(PARAM_I2Cx(I2Cx));

	if (I2Cx==LPC_I2C0)
	{
		/* power for I2C0 module */
		GPIO_Cfg(I2C0_PORT,I2C0_PINS,GPIO_OPT_FNCT_2);  /* I2C0 pin select */
		PerClkEn(PER_NBR_I2C0);
	}
	else if (I2Cx==LPC_I2C1)
	{
		/* power for I2C1 module */
		PerClkEn(PER_NBR_I2C1);
	}
	else if (I2Cx==LPC_I2C2)
	{
		/* power for I2C2 module */
		PerClkEn(PER_NBR_I2C2);
	}
	else {
		// Up-Support this device
		return;
	}

    /* Set clock rate */
    I2C_SetClock(I2Cx, clockrate);
    
	/* Set I2C operation to default */
    I2Cx->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_STAC | I2CONCLR_I2ENC);
	I2Cx->I2CONSET = I2CONSET_I2EN;
}

void I2C0_Init(void)
{	
	I2C_Init(LPC_I2C0,I2C0_CLOCK);

	BSP_IntVectSet((CPU_INT08U)BSP_INT_SRC_NBR_I2C0,(CPU_FNCT_VOID)I2C0_MasterISR_Handler); 
    //BSP_IntPrioSet((CPU_INT08U)BSP_INT_SRC_NBR_I2C0,0);
	BSP_IntEn(BSP_INT_SRC_NBR_I2C0);
}

/*
 * param[in]  I2Cx: I2C peripheral selected, should be:
 * 			  - LPC_I2C0
 * 			  - LPC_I2C1
 * 			  - LPC_I2C2
 * param[in]  byte to send
 * Return     value of I2C status register after sending
 * Notes      Send a byte
 */
CPU_INT32U I2C0_SendByte (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U dat)
{
    CPU_INT32U timeout = I2C0_MAX_TIMEOUT;
  
    I2C_sla       = sla;		  /* 写操作的器件地址       */
    I2C_suba      = suba;         /* 器件内部子地址         */
    I2C_suba_num  = 1;            /* 子地址为1字节          */
    I2C_buf       = &dat;		  /* 待发送的数据           */
    I2C_num       = 1;		      /* 发送1字节数据          */
    I2C_suba_en   = 2;		      /* 写操作，子地址未发送   */
    I2C_end       = 0;            /* 总线忙标志             */
  
    LPC_I2C0->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC);	        
    LPC_I2C0->I2CONSET = (I2CONSET_I2EN | I2CONSET_STA);    /* 设置为主机，并启动总线 */
	
	while(0 == I2C_end)           /* 等待发送结束           */
    {
        if(5 > timeout--)
        break;
    }  
    
	if(1 == I2C_end)              /* 发送成功               */
    {
	    return(DEF_TRUE);
    }else                          /* 发送失败               */
    {
	    return(DEF_FALSE);
	}
}

/*
 * param[in]   I2Cx: I2C peripheral selected, should be:
 * 			   - LPC_I2C0
 * 			   - LPC_I2C1
 * 			   - LPC_I2C2
 * param[out]  retdat: pointer to return data
 * param[in]   ack: assert acknowledge or not, should be: 
 *             - DEF_TRUE
 *             - DEF_FALSE
 * Return      value of I2C status register after sending
 * Notes       Get a byte
 */
CPU_INT32U I2C0_RcvByte (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *pdat)
{
    CPU_INT32U timeout = I2C0_MAX_TIMEOUT;
  
    I2C_sla       = sla+1;	      /* 读操作的器件地址       */
    I2C_suba      = suba;         /* 器件内部子地址         */
    I2C_suba_num  = 1;            /* 子地址为1字节          */
    I2C_buf       = pdat;		  /* 存放接收的数据buf地址  */
    I2C_num       = 1;		      /* 接收1字节数据          */
    I2C_suba_en   = 1;		      /* 读操作，子地址未发送   */
    I2C_end       = 0;            /* 总线忙标志             */
  
    LPC_I2C0->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC);	        
    LPC_I2C0->I2CONSET = (I2CONSET_I2EN | I2CONSET_STA);    /* 设置为主机，并启动总线 */
  
    while(0 == I2C_end)           /* 等待接收结束           */
    {
        if(5 > timeout--)
        break;
    }  
    if(1 == I2C_end)              /* 发送成功               */
    {
	    return(DEF_TRUE);
    }
	else                          /* 发送失败               */
    {
	    return(DEF_FALSE);
	}
}

CPU_INT32U I2C0_SendBytes (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *s, CPU_INT32U num)
{
    CPU_INT32U timeout = I2C0_MAX_TIMEOUT;

    if (num > 0)	              /* 判断num个数的合法性    */
    {    
        I2C_sla       = sla;	  /* 写读操作的器件地址,W=0 */
        I2C_suba      = suba;     /* 器件子地址 			*/
    	I2C_suba_num  = 1;		  /* 器件子地址为2字节 	    */   
    	I2C_buf       = s;		  /* 数据接收缓冲区指针 	*/
    	I2C_num       = num;	  /* 要读取的个数 		    */
    	I2C_suba_en   = 2;		  /* 写操作，子地址未发送	*/
    	I2C_end       = 0;        /* 总线忙标志             */
    
        LPC_I2C0->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC);	        
        LPC_I2C0->I2CONSET = (I2CONSET_I2EN | I2CONSET_STA);    /* 设置为主机，并启动总线 */
       
    	while(0 == I2C_end)                           /* 等待接收结束                       */
    	{
      		if(5 > timeout--)
        	break;
    	}  
    	if(1 == I2C_end)                              /* 发送成功                           */
      	{
			return(DEF_TRUE);
    	}
		else                                          /* 发送失败                           */
      	{
		    return(DEF_FALSE);
  		}
	}
  	return (DEF_FALSE);
}

CPU_INT32U I2C0_RcvBytes (CPU_INT08U sla, CPU_INT08U suba, CPU_INT08U *s, CPU_INT32U num)
{
    CPU_INT32U timeout = I2C0_MAX_TIMEOUT;

    if (num > 0)	              /* 判断num个数的合法性    */
    {    
        I2C_sla       = sla + 1;  /* 读操作的器件地址,R=1 	*/
        I2C_suba      = suba;	  /* 器件子地址 			*/
        I2C_suba_num  = 1;		  /* 器件子地址为1字节 	    */   
        I2C_buf       = s;		  /* 数据接收缓冲区指针 	*/
        I2C_num       = num;	  /* 要读取的个数 		    */
        I2C_suba_en   = 1;		  /* 读操作，子地址未发送	*/
        I2C_end       = 0;        /* 总线忙标志             */
      
        LPC_I2C0->I2CONCLR = (I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC);	        
        LPC_I2C0->I2CONSET = (I2CONSET_I2EN | I2CONSET_STA);    /* 设置为主机，并启动总线 */
       
		while(0 == I2C_end)                         /* 等待接收结束           */
		{
		  if(5 > timeout--)
		  break;
		}  
		if(1 == I2C_end)                            /* 接收成功               */
		{
		  return(DEF_TRUE);
		}
		else                                        /* 发送失败               */
		{
		  return(DEF_FALSE);
		}
    }
    return (DEF_FALSE);
}

/*******************  (C) COPYRIGHT 2010 DJI ************END OF FILE***********/
