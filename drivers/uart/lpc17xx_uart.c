/**
  ******************** (C) COPYRIGHT 2012 DJI **********************************
  *
  * @Project Name       : BL_WKM2_MAIN.uvproj
  * @File Name          : lpc17xx_uart.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx uart module functions        
  */

/* Includes ------------------------------------------------------------------*/
#define  UART_MODULE
#include "../drivers.h"

/* Private function prototypes -----------------------------------------------*/
#if UART0_EN > 0
static void  UART0_Init( uint32_t baud );
void  UART0_ISRHandler(void);
#endif

#if UART1_EN > 0
static void UART1_Init( uint32_t baud );
void UART1_ISRHandler(void);
#endif

#if UART2_EN > 0
static void UART2_Init( uint32_t baud );
void UART2_ISRHandler(void);
#endif

#if UART3_EN > 0
static void UART3_Init( uint32_t baud );
void UART3_ISRHandler(void);
#endif

//#ifdef __DEBUG_UART2_PRINTF__
//static uint8_t uart_buf_printf[128] __attribute__ ((section("ETHERNET_RAM"),aligned));
//static uint32_t printf_lock = 0;
//#endif
static uint8_t uart_buf_printf[128] __attribute__ ((section("ETHERNET_RAM"),aligned));
static uint32_t printf_lock = 0;

void uart_swtich_init(void)
{
	LPC_GPIO2->FIODIR |= 1<< 2;
	LPC_GPIO2->FIOSET |= 1<< 2;
	
	LPC_GPIO2->FIODIR |= 1<< 4;
	LPC_GPIO2->FIOSET |= 1<< 4;
}

void TX_RX1_ON(void)
{
	LPC_GPIO2->FIOCLR |= 1<< 2;
	LPC_GPIO2->FIOCLR |= 1<< 4;
}

void TX_RX2_ON(void)
{
	LPC_GPIO2->FIOSET |= 1<< 2;
	LPC_GPIO2->FIOCLR |= 1<< 4;
}
// __DEBUG_UART2_PRINTF__  __DEBUG_UART2_PRINTF__,
void uart_printf(uint8_t uart_n,char *pstr, ...)    
{
  #ifdef __DEBUG_UART2_PRINTF__
    va_list arglist;
    uint8_t *fp;

	if(printf_lock)return;
	  printf_lock = 1;  //互斥
    va_start(arglist, pstr);
    vsnprintf((void*)uart_buf_printf,128,(void*)pstr, arglist);
    va_end(arglist);
    fp = uart_buf_printf; 
   	
	  switch (uart_n)
    {
				case 0:	
				default:
				{ 
					while(*fp) UART0_SendByte(*fp++);
				}
				break;   
				case 2:
				{ 
					while(*fp) UART2_SendByte(*fp++);
				}
				break;	
	      case 3:
				{ 
					while(*fp) UART3_SendByte(*fp++);
				}
				break;				
    }
	printf_lock = 0; 
#endif
}



/*
 * Name			  :	void UARTS_Init(void);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Initialize uart.
 *
 */
void UARTS_Init(void)
{
#if UART0_EN > 0
    UART0_Init( 115200 );
#endif

#if UART1_EN > 0
    UART1_Init( 115200 );
#endif

#if UART2_EN > 0
	 UART2_Init( 115200);
#endif

#if UART3_EN > 0
   UART3_Init( 115200 );
#endif
}

/* UART0 FUNCTION ------------------------------------------------------------*/
#if   UART0_EN > 0
/*
 * Name			  :	void UART0_Init(UART_InitTypeDef UART0_InitStructure);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Initialize uart.
 *
 */
static void UART0_Init( uint32_t baud )
{	
	uint32_t pclk_freq;
	uint32_t i,err_abs,tmp_i,tmp_err_abs;
	uint16_t div;
	uint8_t j,k,tmp_j,tmp_k,fra_div,fra_mul;
	
	tmp_j = tmp_k = 0;
	tmp_i = 0;
	/* find Divisor and Fractional Divider */
	/* 只四舍五入(误差大),要考虑小数位(误差小)*/
	/* 1<=MULVAL>=15; 0<=DIVADDVAL>=14; DIVADDVAL<MULVAL */ 
	pclk_freq = (25000000); 
    div = (uint16_t)(2*pclk_freq/16/baud + 1)/2; /* 四舍五入 */
	
	fra_div = 0;
    fra_mul = 1;
	if((pclk_freq/16/div) != baud)	 /* integer ?*/
	{
		err_abs = 0xffffffff;
		/* find the best div,fra_div,fra_abs */
		for(i=div; i>div/2; i--)
		{
		    for(j=0; j<15; j++)				/* DIVADDVAL */
			{
				for(k=j+1; k<16; k++)		/* MULVAL */
				{
					tmp_err_abs = DEF_ABS((int32_t)(((pclk_freq*k)/16/i/(k+j)) - baud)); 
					if(tmp_err_abs < err_abs)
					{
					    err_abs = tmp_err_abs;
						tmp_i = i;
						tmp_j = j;
						tmp_k = k;
					}
					if(j == 0)break; 
				}
			}
		}
		div	= tmp_i;
		fra_div	= tmp_j;
		fra_mul	= tmp_k;
	}

	LPC_SC->PCONP       |=  1<<3;	          /* enable UART0 clock source */

	PINSEL0 &= ~(3<<4);            
	PINSEL0 |=   1<<4 ;			  /* uart0 TXD pin select */
	PINSEL0 &= ~(3<<6);            
	PINSEL0 |=  (1<<6);			  /* uart0 RXD pin select */

	LPC_UART0->LCR = SET_DLAB;
	LPC_UART0->DLL = (uint8_t)(div&0x00ff);
	LPC_UART0->DLM = (uint8_t)((div>>8)&0x00ff);
	LPC_UART0->FDR = (fra_div<<FDR_DIVADDVAL)|(fra_mul<<FDR_MULVAL);
	
	/* LPC_UART0->TER 中发生使能位默认为1,无需再次设置发送使能位 */
	LPC_UART0->LCR = 0x03;  /* DLAB = 0，diable access to 波特率,8N1 */
	LPC_UART0->IER = DEF_BIT_NONE;
	LPC_UART0->FCR = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET | RX_TRI_LS3; /* 14 bytes trigger */

  LPC_UART0->IER |= IER_RBR_EN;  //使能接收中断
	LPC_UART0->IER |= IER_THRE_EN; //使能发送中断
	NVIC_EnableIRQ(UART0_IRQn);

	UART0_BufClr();
}

void UART0_BufClr(void)
{
  UART0_tx_buf.wrIdx = 0;
	UART0_tx_buf.rdIdx = 0;
	UART0_tx_buf.mask  = (uint32_t)(TX_BUFFER_SIZE0-1);
	UART0_tx_buf.pBuf  = &tx_buffer0[0];

	UART0_rx_buf.wrIdx = 0;
	UART0_rx_buf.rdIdx = 0;
	UART0_rx_buf.mask  = (uint32_t)(RX_BUFFER_SIZE0-1);
	UART0_rx_buf.pBuf  = &rx_buffer0[0];	
}

/* polling r&w */
void UART0_putchar(uint8_t c)
{
	LPC_UART0->THR = c;	       
    while( (LPC_UART0->LSR & LSR_TEMT) == 0 );  
}

uint8_t UART0_getchar(void)
{
    while( (LPC_UART0->LSR&LSR_RDR) == 0 );
	return LPC_UART0->RBR;
}

/* interrupt r&w */
void UART0_SendByte(uint8_t data)
{
	CPU_SR_ALLOC();
    
	while(RING_BUF_COUNT(UART0_tx_buf) >= (uint32_t)TX_BUFFER_SIZE0-16);  /* 不会锁死 */

	CPU_CRITICAL_ENTER(); 
	RING_BUF_WR(UART0_tx_buf,data);
    if ((LPC_UART0->LSR&LSR_THRE) != 0) LPC_UART0->THR = RING_BUF_RD(UART0_tx_buf);
    CPU_CRITICAL_EXIT();
}

void UART0_send_pack(uint8_t *buf,uint16_t len)
{
    uint16_t wrNum;
	CPU_SR_ALLOC();
    
	if(len>TX_BUFFER_SIZE0)return;
	
	for(;len>0;)
	{	
	    while(RING_BUF_COUNT(UART0_tx_buf) >= (uint32_t)TX_BUFFER_SIZE0-16);  /* 不会锁死 */
	    wrNum = TX_BUFFER_SIZE0 - RING_BUF_COUNT(UART0_tx_buf); /* buf could be writed */
		if(wrNum>len)
		{
		    wrNum = len;
		} 
		 
		len -= wrNum;
		CPU_CRITICAL_ENTER(); 
		while(wrNum--)
		{    
	        RING_BUF_WR(UART0_tx_buf,*buf++);	
		}
		if ( ((LPC_UART0->LSR&LSR_THRE) != 0) )LPC_UART0->THR = RING_BUF_RD(UART0_tx_buf); 
		CPU_CRITICAL_EXIT();
	}
}

void UART0_IRQHandler(void) 
{
    uint8_t reg_iir,i;
	 
	reg_iir = (uint8_t)(LPC_UART0->IIR & 0x0F);
	
	if(reg_iir != 1)
	{
		reg_iir = reg_iir >> 1;
		switch(reg_iir)
		{
		    case 0x3: /* Highest */
			{
			
			}
			break;
			case 0x2: /* Second */
			case 0x6:
			{
				while(LPC_UART0->LSR & LSR_RDR)
				{
					RING_BUF_WR(UART0_rx_buf,LPC_UART0->RBR);	
				}
			}
			break;
			case 0x1:  /* Third */
			{
			    i = 16;
				while (i && RING_BUF_COUNT(UART0_tx_buf))
                {
                    i--;
                    LPC_UART0->THR = RING_BUF_RD(UART0_tx_buf);
                }  
			}
			break;
			default :
			break;
		}
	}
}
#endif

#if UART1_EN > 0

void UART1_BufClr(void)
{
  UART1_tx_buf.wrIdx = 0;
	UART1_tx_buf.rdIdx = 0;
	UART1_tx_buf.mask  = (uint32_t)(TX_BUFFER_SIZE1-1);
	UART1_tx_buf.pBuf  = &tx_buffer1[0];

	UART1_rx_buf.wrIdx = 0;
	UART1_rx_buf.rdIdx = 0;
	UART1_rx_buf.mask  = (uint32_t)(RX_BUFFER_SIZE1-1);
	UART1_rx_buf.pBuf  = &rx_buffer1[0];	
}

//初始化使能管脚
void RS485_RD_Init( void )
{
	LPC_GPIO1->FIODIR   |= 1 << 14;
}

static void UART1_Init( uint32_t baud )
{	
	uint32_t pclk_freq;
	uint32_t i,err_abs,tmp_i,tmp_err_abs;
	uint16_t div;
	uint8_t j,k,tmp_j,tmp_k,fra_div,fra_mul;
	
	tmp_j = tmp_k = 0;
	tmp_i = 0;
	/* find Divisor and Fractional Divider */
	/* 只四舍五入(误差大),要考虑小数位(误差小)*/
	/* 1<=MULVAL>=15; 0<=DIVADDVAL>=14; DIVADDVAL<MULVAL */ 
	pclk_freq = (25000000); 
    div = (uint16_t)(2*pclk_freq/16/baud + 1)/2; /* 四舍五入 */
	
	fra_div = 0;
    fra_mul = 1;
	if((pclk_freq/16/div) != baud)	 /* integer ?*/
	{
		err_abs = 0xffffffff;
		/* find the best div,fra_div,fra_abs */
		for(i=div; i>div/2; i--)
		{
		    for(j=0; j<15; j++)				/* DIVADDVAL */
			{
				for(k=j+1; k<16; k++)		/* MULVAL */
				{
					tmp_err_abs = DEF_ABS((int32_t)(((pclk_freq*k)/16/i/(k+j)) - baud)); 
					if(tmp_err_abs < err_abs)
					{
					    err_abs = tmp_err_abs;
						tmp_i = i;
						tmp_j = j;
						tmp_k = k;
					}
					if(j == 0)break; 
				}
			}
		}
		div	= tmp_i;
		fra_div	= tmp_j;
		fra_mul	= tmp_k;
	}

	LPC_SC->PCONP       |=  1<<4;	          /* enable UART1 clock source */

	PINSEL4 &= ~(0x03<<0);            
	PINSEL4 |=   0x02<<0 ;			  /* uart1 TXD pin select */
	PINSEL4 &= ~(0x03<<2);            
	PINSEL4 |=  (0x02<<2);			  /* uart1 RXD pin select */

	LPC_UART1->LCR = SET_DLAB;
	LPC_UART1->DLL = (uint8_t)(div&0x00ff);
	LPC_UART1->DLM = (uint8_t)((div>>8)&0x00ff);
	LPC_UART1->FDR = (fra_div<<FDR_DIVADDVAL)|(fra_mul<<FDR_MULVAL);
	
	/* LPC_UART0->TER 中发生使能位默认为1,无需再次设置发送使能位 */
	LPC_UART1->LCR = 0x03;  /* DLAB = 0，diable access to 波特率,8N1 */
	LPC_UART1->IER = DEF_BIT_NONE;
	LPC_UART1->FCR = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET | RX_TRI_LS3; /* 14 bytes trigger */

  LPC_UART1->IER |= IER_RBR_EN;  //使能接收中断
	LPC_UART1->IER |= IER_THRE_EN; //使能发送中断
	NVIC_EnableIRQ(UART1_IRQn);

	UART1_BufClr();
	//RS485_RD_Init();
}


/* polling r&w */
void UART1_putchar(uint8_t c)
{
	LPC_UART1->THR = c;	       
    while( (LPC_UART1->LSR & LSR_TEMT) == 0 );  
}

uint8_t UART1_getchar(void)
{
    while( (LPC_UART1->LSR&LSR_RDR) == 0 );
	return LPC_UART1->RBR;
}

/* interrupt r&w */
void UART1_SendByte(uint8_t data)
{
	CPU_SR_ALLOC();
    
	while(RING_BUF_COUNT(UART1_tx_buf) >= (uint32_t)TX_BUFFER_SIZE1-16);  /* 不会锁死 */

	CPU_CRITICAL_ENTER(); 
	RING_BUF_WR(UART1_tx_buf,data);
    if ((LPC_UART1->LSR&LSR_THRE) != 0) LPC_UART1->THR = RING_BUF_RD(UART1_tx_buf);
    CPU_CRITICAL_EXIT();
}

void UART1_send_pack(uint8_t *buf,uint16_t len)
{
  uint16_t wrNum;
	uint32_t delay ;
	CPU_SR_ALLOC();
	
//  RS485_WR();  
	
	for(delay = 0x1fff ; delay>1 ; delay-- );
	
	if(len>TX_BUFFER_SIZE1)return;

	for(;len>0;)
	{	
	    while(RING_BUF_COUNT(UART1_tx_buf) >= (uint32_t)TX_BUFFER_SIZE1-16);  /* 不会锁死 */
	    wrNum = TX_BUFFER_SIZE1 - RING_BUF_COUNT(UART1_tx_buf); /* buf could be writed */
		if(wrNum>len)
		{
		    wrNum = len;
		} 

		len -= wrNum;
		CPU_CRITICAL_ENTER(); 
		while(wrNum--)
		{    
	        RING_BUF_WR(UART1_tx_buf,*buf++);	
		}
		if ( ((LPC_UART1->LSR&LSR_THRE) != 0) )LPC_UART1->THR = RING_BUF_RD(UART1_tx_buf); 
		CPU_CRITICAL_EXIT();
	}
}


uint8_t UART1_ReicieveByte(void)
{	
    uint8_t data;
    CPU_SR_ALLOC();
    
    while(RING_BUF_IS_EMPTY(UART1_rx_buf));

	CPU_CRITICAL_ENTER();
	data = RING_BUF_RD(UART1_rx_buf);
    CPU_CRITICAL_EXIT();

	return data;
}

void UART1_IRQHandler(void) 
{
  uint8_t reg_iir,i;

	reg_iir = (uint8_t)(LPC_UART1->IIR & 0x0F);

	if(reg_iir != 1)
	{
		reg_iir = reg_iir >> 1;
		switch(reg_iir)
		{
		  case 0x3: /* Highest */
			{

			}
			break;
			case 0x2: /* Second */
			case 0x6:
			{
				while(LPC_UART1->LSR & LSR_RDR)
				{
					RING_BUF_WR(UART1_rx_buf,LPC_UART1->RBR);	
				}
			}
			break;
			case 0x1:  /* Third */
			{
			    i = 16;

				//如果已经发送完，下一次进中断再置为接收     
				if(  RING_BUF_COUNT(UART1_tx_buf) == 0 ){
//					TIMER1_Start();
//					RS485_RD();
				}
				while (i && RING_BUF_COUNT(UART1_tx_buf))
				{
						i--;
//						RS485_WR(); 
						LPC_UART1->THR = RING_BUF_RD(UART1_tx_buf);
				}
//				//如果已经发送完，就设置成接受模式
//				if(  RING_BUF_COUNT(UART1_tx_buf) == 0 ){
//					TIMER1_Start();
//				}
			}
			break;
			default :
			break;
		}
	}
}
#endif /* uart1 endif */


//初始化使能管脚
void Radar_RS485_RD_Init( void )
{
	LPC_GPIO1->FIODIR   |= 1 << 0;
	//Radar_RS485_RD()
}


/* UART2 FUNCTION ------------------------------------------------------------*/
#if UART2_EN > 0
/*
 * Name			  :	void UART0_Init(UART_InitTypeDef UART0_InitStructure);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Initialize uart.
 *
 */
static void UART2_Init( uint32_t baud )
{
	uint32_t pclk_freq;
	uint32_t i,err_abs,tmp_i,tmp_err_abs;
	uint16_t div;
	uint8_t j,k,tmp_j,tmp_k,fra_div,fra_mul;
	
	tmp_j = tmp_k = 0;
	tmp_i = 0;
	/* find Divisor and Fractional Divider */
	/* 只四舍五入(误差大),要考虑小数位(误差小)*/
	/* 1<=MULVAL>=15; 0<=DIVADDVAL>=14; DIVADDVAL<MULVAL */ 
	pclk_freq = (25000000); 
    div       = (uint16_t)(2*pclk_freq/16/baud + 1)/2; /* 四舍五入 */
	
	fra_div = 0;
    fra_mul = 1;
	if((pclk_freq/16/div) != baud)	 /* integer ?*/
	{
		err_abs = 0xffffffff;
		/* find the best div,fra_div,fra_abs */
		for(i=div; i>div/2; i--)
		{
		    for(j=0; j<15; j++)				/* DIVADDVAL */
			{
				for(k=j+1; k<16; k++)		/* MULVAL */
				{
					tmp_err_abs = DEF_ABS((int32_t)(((pclk_freq*k)/16/i/(k+j)) - baud)); 
					if(tmp_err_abs < err_abs)
					{
					    err_abs = tmp_err_abs;
						tmp_i = i;
						tmp_j = j;
						tmp_k = k;
					}
					if(j == 0)break; 
				}
			}
		}
		div	= tmp_i;
		fra_div	= tmp_j;
		fra_mul	= tmp_k;
	}

	LPC_SC->PCONP |= 1<<24;	          /* enable UART2 clock source */

//	PINSEL0       &= ~(3<<20);            
//    PINSEL0       |=   1<<20 ;	      /* uart2 TXD pin select */
		//P2.8
  PINSEL4       &= ~(3<<16);            
  PINSEL4       |=   2<<16 ;	      /* uart2 TXD pin select */
	
	PINSEL0       &= ~(3<<22);            
	PINSEL0       |=  (1<<22);		  /* uart2 RXD pin select */
	
	

	

	LPC_UART2->LCR = SET_DLAB;
	LPC_UART2->DLL = (uint8_t)(div&0x00ff);
	LPC_UART2->DLM = (uint8_t)((div>>8)&0x00ff);
	LPC_UART2->FDR = (fra_div<<FDR_DIVADDVAL)|(fra_mul<<FDR_MULVAL);
	
	/* LPC_UART2->TER 中发生使能位默认为1,无需再次设置发送使能位 */
	LPC_UART2->LCR = 0x03;  /* DLAB = 0，diable access to 波特率,8N1 */
	LPC_UART2->IER = DEF_BIT_NONE;
	LPC_UART2->FCR = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET | RX_TRI_LS3; /* 14 bytes trigger */
	
	LPC_UART2->IER |= IER_THRE_EN; //使能发送中断
  LPC_UART2->IER |= IER_RBR_EN;  //使能接收中断

	NVIC_EnableIRQ(UART2_IRQn);;

	UART2_BufClr();
	
	Radar_RS485_RD_Init();
	Radar_RS485_RD();
}

void UART2_BufClr(void)
{
    UART2_tx_buf.wrIdx = 0;
	UART2_tx_buf.rdIdx = 0;
	UART2_tx_buf.mask  = (uint32_t)(TX_BUFFER_SIZE2-1);
	UART2_tx_buf.pBuf  = &tx_buffer2[0];

	UART2_rx_buf.wrIdx = 0;
	UART2_rx_buf.rdIdx = 0;
	UART2_rx_buf.mask  = (uint32_t)(RX_BUFFER_SIZE2-1);
	UART2_rx_buf.pBuf  = &rx_buffer2[0];	
	
	UART2_rx_buf_copy.wrIdx = 0;
	UART2_rx_buf_copy.rdIdx = 0;
	UART2_rx_buf_copy.mask  = (uint32_t)(RX_BUFFER_SIZE2-1);
	UART2_rx_buf_copy.pBuf  = &rx_buffer2_copy[0];
}

/* polling r&w */
void UART2_putchar(uint8_t c)
{
	LPC_UART2->THR = c;	       
    while( (LPC_UART2->LSR & LSR_TEMT) == 0 );  
}

uint8_t UART2_getchar(void)
{
    while( (LPC_UART2->LSR&LSR_RDR) == 0 );
	return LPC_UART2->RBR;
}

/* interrupt r&w */
void UART2_SendByte(uint8_t data)
{
	CPU_SR_ALLOC();
  Radar_RS485_WR();
	while(RING_BUF_COUNT(UART2_tx_buf) >= (uint32_t)TX_BUFFER_SIZE2-16);  /* 不会锁死 */

	CPU_CRITICAL_ENTER(); 
	RING_BUF_WR(UART2_tx_buf,data);
    if ( ((LPC_UART2->LSR&LSR_THRE) != 0) )LPC_UART2->THR = RING_BUF_RD(UART2_tx_buf);
    CPU_CRITICAL_EXIT();
}

void UART2_send_pack(uint8_t *buf,uint16_t len)
{
  #ifdef __DEBUG_UART2_PRINTF__
	
	
	#else
	uint16_t wrNum;
	CPU_SR_ALLOC();
	if(len>TX_BUFFER_SIZE2)return;
	Radar_RS485_WR(); 
	for(;len>0;)
	{	
	    while(RING_BUF_COUNT(UART2_tx_buf) >= (uint32_t)TX_BUFFER_SIZE2-16);  /* 不会锁死 */
	    wrNum = TX_BUFFER_SIZE2 - RING_BUF_COUNT(UART2_tx_buf); /* buf could be writed */
		if(wrNum>len)
		{
		    wrNum = len;
		} 
		 
		len -= wrNum;
		CPU_CRITICAL_ENTER(); 
		while(wrNum--)
		{    
	        RING_BUF_WR(UART2_tx_buf,*buf++);	
		}
		if ( ((LPC_UART2->LSR&LSR_THRE) != 0) )LPC_UART2->THR = RING_BUF_RD(UART2_tx_buf); 
		CPU_CRITICAL_EXIT();
	}
	#endif
}

uint8_t UART2_ReicieveByte(void)
{	
    uint8_t data;
    CPU_SR_ALLOC();
    
    while(RING_BUF_IS_EMPTY(UART2_rx_buf));

	CPU_CRITICAL_ENTER();
	data = RING_BUF_RD(UART2_rx_buf);
    CPU_CRITICAL_EXIT();

	return data;
}

uint32_t smm;
extern void TIMER2_Start(void);
void UART2_IRQHandler(void)
{
    uint8_t reg_iir,i;
	 uint8_t rx_data ;
	reg_iir = (uint8_t)(LPC_UART2->IIR & 0x0F);
	
	if(reg_iir != 1)
	{
		reg_iir = reg_iir >> 1;
		switch(reg_iir)
		{
		    case 0x3: /* Highest */
			{
			
			}
			break;
			case 0x2: /* Second */
			case 0x6:
			{
				while(LPC_UART2->LSR & LSR_RDR)
				{
					rx_data = LPC_UART2->RBR ;
					RING_BUF_WR(UART2_rx_buf,rx_data);	
					RING_BUF_WR(UART2_rx_buf_copy,rx_data);						
				}
			}
			break;
			case 0x1:  /* Third */
			{
			    i = 16;
				if(  RING_BUF_COUNT(UART2_tx_buf) == 0 ){
					smm++;
					TIMER2_Start();
				}
				while (i && RING_BUF_COUNT(UART2_tx_buf))
				{
						i--;
						Radar_RS485_WR();
						LPC_UART2->THR = RING_BUF_RD(UART2_tx_buf);
				}  
			}
			break;
			default :
			break;
		}
	}
}
#endif


/* UART3 FUNCTION ------------------------------------------------------------*/
#if UART3_EN > 0
/*
 * Name			  :	void UART0_Init(UART_InitTypeDef UART0_InitStructure);
 * Input          : none
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Initialize uart.
 *
 */
static void UART3_Init( uint32_t baud )
{
	uint32_t pclk_freq;
	uint32_t i,err_abs,tmp_i,tmp_err_abs;
	uint16_t div;
	uint8_t j,k,tmp_j,tmp_k,fra_div,fra_mul;
	
	tmp_j = tmp_k = 0;
	tmp_i = 0;
	/* find Divisor and Fractional Divider */
	/* 只四舍五入(误差大),要考虑小数位(误差小)*/
	/* 1<=MULVAL>=15; 0<=DIVADDVAL>=14; DIVADDVAL<MULVAL */ 
	pclk_freq = (25000000); 
    div       = (uint16_t)(2*pclk_freq/16/baud + 1)/2; /* 四舍五入 */

	fra_div = 0;
    fra_mul = 1;
	if((pclk_freq/16/div) != baud)	 /* integer ?*/
	{
		err_abs = 0xffffffff;
		/* find the best div,fra_div,fra_abs */
		for(i=div; i>div/2; i--)
		{
		    for(j=0; j<15; j++)				/* DIVADDVAL */
			{
				for(k=j+1; k<16; k++)		/* MULVAL */
				{
					tmp_err_abs = DEF_ABS((int32_t)(((pclk_freq*k)/16/i/(k+j)) - baud)); 
					if(tmp_err_abs < err_abs)
					{
					    err_abs = tmp_err_abs;
						tmp_i = i;
						tmp_j = j;
						tmp_k = k;
					}
					if(j == 0)break; 
				}
			}
		}
		div	= tmp_i;
		fra_div	= tmp_j;
		fra_mul	= tmp_k;
	}

	LPC_SC->PCONP |= 1<<25;	          /* enable UART3 clock source */

//	PINSEL1       &= ~(3<<18);            
//  PINSEL1       |=   3<<18 ;	      /* uart3 TXD pin select */
//	PINSEL1       &= ~(3<<20);            
//	PINSEL1       |=  (3<<20);		  /* uart3 RXD pin select */
	
	PINSEL0       &= ~(2<<0);            
  PINSEL0       |=   2<<0;	      /* uart3 TXD pin select */
	PINSEL0       &= ~(2<<2);            
	PINSEL0       |=  (2<<2);		  /* uart3 RXD pin select */
	

	LPC_UART3->LCR = SET_DLAB;
	LPC_UART3->DLL = (uint8_t)(div&0x00ff);
	LPC_UART3->DLM = (uint8_t)((div>>8)&0x00ff);
	LPC_UART3->FDR = (fra_div<<FDR_DIVADDVAL)|(fra_mul<<FDR_MULVAL);
	
	/* LPC_UART3->TER 中发生使能位默认为1,无需再次设置发送使能位 */
	LPC_UART3->LCR = 0x03;  /* DLAB = 0，diable access to 波特率,8N1 */
	LPC_UART3->IER = DEF_BIT_NONE;
	LPC_UART3->FCR = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET | RX_TRI_LS3; /* 14 bytes trigger */
	
	LPC_UART3->IER |= IER_THRE_EN; //使能发送中断
  LPC_UART3->IER |= IER_RBR_EN;  //使能接收中断
 // NVIC_SetPriority(UART3_IRQn, ((0x01<<3)|0x01));
	NVIC_EnableIRQ(UART3_IRQn);

	UART3_BufClr();
}

void UART3_BufClr(void)
{
  UART3_tx_buf.wrIdx = 0;
	UART3_tx_buf.rdIdx = 0;
	UART3_tx_buf.mask  = (uint32_t)(TX_BUFFER_SIZE3-1);
	UART3_tx_buf.pBuf  = &tx_buffer3[0];

	UART3_rx_buf.wrIdx = 0;
	UART3_rx_buf.rdIdx = 0;
	UART3_rx_buf.mask  = (uint32_t)(RX_BUFFER_SIZE3-1);
	UART3_rx_buf.pBuf  = &rx_buffer3[0];	
}

/* polling r&w */
void UART3_putchar(uint8_t c)
{
	LPC_UART3->THR = c;	       
    while( (LPC_UART3->LSR & LSR_TEMT) == 0 );  
}

uint8_t UART3_getchar(void)
{
    while( (LPC_UART3->LSR&LSR_RDR) == 0 );
	return LPC_UART3->RBR;
}

/* interrupt r&w */
void UART3_SendByte(uint8_t data)
{
	CPU_SR_ALLOC();
    
	while(RING_BUF_COUNT(UART3_tx_buf) >= (uint32_t)TX_BUFFER_SIZE3-16);  /* 不会锁死 */

	CPU_CRITICAL_ENTER(); 
	RING_BUF_WR(UART3_tx_buf,data);
    if ( ((LPC_UART3->LSR&LSR_THRE) != 0) )LPC_UART3->THR = RING_BUF_RD(UART3_tx_buf);
    CPU_CRITICAL_EXIT();
}

void UART3_send_pack(uint8_t *buf,uint16_t len)
{
    uint16_t wrNum;
	CPU_SR_ALLOC();
    
	if(len>TX_BUFFER_SIZE3)return;
	
	for(;len>0;)
	{	
	    while(RING_BUF_COUNT(UART3_tx_buf) >= (uint32_t)TX_BUFFER_SIZE3-16);  /* 不会锁死 */
	    wrNum = TX_BUFFER_SIZE3 - RING_BUF_COUNT(UART3_tx_buf); /* buf could be writed */
		if(wrNum>len)
		{
		    wrNum = len;
		} 
		 
		len -= wrNum;
		CPU_CRITICAL_ENTER(); 
		while(wrNum--)
		{    
	        RING_BUF_WR(UART3_tx_buf,*buf++);	
		}
		if ( ((LPC_UART3->LSR&LSR_THRE) != 0) )LPC_UART3->THR = RING_BUF_RD(UART3_tx_buf); 
		CPU_CRITICAL_EXIT();
	}
}

uint8_t UART3_ReicieveByte(void)
{	
    uint8_t data;
    CPU_SR_ALLOC();
    
    while(RING_BUF_IS_EMPTY(UART3_rx_buf));

	CPU_CRITICAL_ENTER();
	data = RING_BUF_RD(UART3_rx_buf);
    CPU_CRITICAL_EXIT();

	return data;
}

void UART3_IRQHandler(void) //UART3_ISRHandler !!!!
{
    uint8_t reg_iir,i;
	 
	reg_iir = (uint8_t)(LPC_UART3->IIR & 0x0F);
	
	if(reg_iir != 1)
	{
		reg_iir = reg_iir >> 1;
		switch(reg_iir)
		{
		  case 0x3: /* Highest */
			{
			
			}
			break;
			case 0x2: /* Second */
			case 0x6:
			{
				while(LPC_UART3->LSR & LSR_RDR)
				{
					RING_BUF_WR(UART3_rx_buf,LPC_UART3->RBR);	
				}

			}
			break;
			case 0x1:  /* Third */
			{
			    i = 16;
				while (i && RING_BUF_COUNT(UART3_tx_buf))
        {
             i--;
             LPC_UART3->THR = RING_BUF_RD(UART3_tx_buf);
        }  
			}
			break;
			default :
			break;
		}
	}
}
#endif


/*******************  (C) COPYRIGHT 2012 DJI ************END OF FILE***********/
