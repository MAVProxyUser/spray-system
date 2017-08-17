/**
  ******************** (C) COPYRIGHT 2010 DJI **********************************
  *
  * @Project Name       £ºxp60.uvproj
  * @File Name          : lpc17xx_gpio.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-01-27 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    lpc17xx clock and power control        
  */

/* Includes ------------------------------------------------------------------*/
#include "..\drivers.h"

/* Exported typedef ----------------------------------------------------------*/
typedef  struct gpio_fast_reg {
    CPU_REG32  FIODIR;
    CPU_REG32  RESERVED0[3];
    CPU_REG32  FIOMASK;
    CPU_REG32  FIOPIN;
    CPU_REG32  FIOSET;
    CPU_REG32  FIOCLR;    
} GPIO_FAST_REG;

void GPIO_Init(void)
{
    PerClkEn (PER_NBR_GPIO);
}

/*
 * Name			  :	void   GPIO_Cfg (CPU_INT08U   gpio_port,
 *                                       CPU_INT32U   gpio_pins,     
 *                                       CPU_INT16U   gpio_opt);
 * Input          : 
 *                   gpio_port      The PIO controller port.
 *                                      GPIO_PORT0_FAST
 *                                      GPIO_PORT1_FAST 
 *                                      GPIO_PORT2_FAST
 *                                      GPIO_PORT3_FAST 
 *                                      GPIO_PORT4_FAST
 *
 *                  gpio_pin        Bit field with the pins to be configured
 *
 *                  gpio_cfg_opt    PIO configuration options
 *                                      GPIO_OPT_OUT_EN           Controlled pin is output
 *                                      GPIO_OPT_IN_EN            Controlled pin is input
 *                                      GPIO_OPT_RD_WR_EN         Controlled pin is affected by writes to
 *                                                                     FIOSET, FIOCLR, FIOPIN registers.
 *                                      GPIO_OPT_FNCT1            Controlled pin is configured with function 1
 *                                      GPIO_OPT_FNCT2            Controlled pin is configured with function 2
 *                                      GPIO_OPT_FNCT3            Controlled pin is configured with function 3
 *                                      GPIO_OPT_FNCT4            Controlled pin is configured with function 4
 *
 *                                      GPIO_OPT_MODE_PULLUP      Controlled pin has a pull-up resitor enabled.
 *                                      GPIO_OPT_MODE_NONE        Controlled pin has neither pull-up nor pull-down.
 *                                      GPIO_OPT_MODE_PULLDOWN    Controlled pin has a pull-down resistor enabled.
 *                                      GPIO_OPT_MODE_REPEATER    Controlled pin has repeater mode enabled.
 *                                      GPIO_OPT_MODE_OPEN_DRAIN  Controlled pin is in open drain mode.
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	gpio config.
 *
*/
void   GPIO_Cfg (CPU_INT08U   gpio_port,
                     CPU_INT32U   gpio_pins,     
                     CPU_INT16U   gpio_opt)
{
    GPIO_FAST_REG  *p_gpio_fast_reg;    
    CPU_REG32          *p_gpio_pinsel;
    CPU_REG32          *p_gpio_pinmode;  
    CPU_REG32          *p_gpio_pinmode_od;  
    
    CPU_INT32U          pinsel_opt;
    CPU_INT32U          pinmode_opt;
    CPU_INT32U          pin_nbr;
    
    
    switch (gpio_port) {
        case GPIO_PORT0_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT0_FAST_BASE_ADDR); 
             p_gpio_pinsel     = (CPU_REG32         *)(GPIO_REG_PINSEL_BASE_ADDR  + 0x00);
             p_gpio_pinmode    = (CPU_REG32         *)(GPIO_REG_PINMODE_BASE_ADDR + 0x00);
             p_gpio_pinmode_od = (CPU_REG32         *)(GPIO_REG_PINMODE_OD0_BASE_ADDR);
             break;

        case GPIO_PORT1_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT1_FAST_BASE_ADDR); 
             p_gpio_pinsel     = (CPU_REG32         *)(GPIO_REG_PINSEL_BASE_ADDR  + 0x08);
             p_gpio_pinmode    = (CPU_REG32         *)(GPIO_REG_PINMODE_BASE_ADDR + 0x08);
             p_gpio_pinmode_od = (CPU_REG32         *)(GPIO_REG_PINMODE_OD1_BASE_ADDR);
             break;

        case GPIO_PORT2_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT2_FAST_BASE_ADDR); 
             p_gpio_pinsel     = (CPU_REG32         *)(GPIO_REG_PINSEL_BASE_ADDR  + 0x10);
             p_gpio_pinmode    = (CPU_REG32         *)(GPIO_REG_PINMODE_BASE_ADDR + 0x10);
             p_gpio_pinmode_od = (CPU_REG32         *)(GPIO_REG_PINMODE_OD2_BASE_ADDR);
             
             break;

        case GPIO_PORT3_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT3_FAST_BASE_ADDR); 
             p_gpio_pinsel     = (CPU_REG32         *)(GPIO_REG_PINSEL_BASE_ADDR  + 0x18);
             p_gpio_pinmode    = (CPU_REG32         *)(GPIO_REG_PINMODE_BASE_ADDR + 0x18);
             p_gpio_pinmode_od = (CPU_REG32         *)(GPIO_REG_PINMODE_OD3_BASE_ADDR);
             
             break;

        case GPIO_PORT4_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT4_FAST_BASE_ADDR); 
             p_gpio_pinsel     = (CPU_REG32         *)(GPIO_REG_PINSEL_BASE_ADDR  + 0x20);
             p_gpio_pinmode    = (CPU_REG32         *)(GPIO_REG_PINMODE_BASE_ADDR + 0x20);
             p_gpio_pinmode_od = (CPU_REG32         *)(GPIO_REG_PINMODE_OD4_BASE_ADDR);
             break;

        case GPIO_PORT0:
        case GPIO_PORT1:             
        default:
             return;
    }

                                                                /* ------------ I/O DIRECTION CONFIGURATION ----------- */
    if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_OUT_EN)) {
        DEF_BIT_SET(p_gpio_fast_reg->FIODIR, gpio_pins);
    }
    
    if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_IN_EN)) {
        DEF_BIT_CLR(p_gpio_fast_reg->FIODIR, gpio_pins);
    }            
        
    if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_RD_WR_EN)) {
        DEF_BIT_CLR(p_gpio_fast_reg->FIOMASK, gpio_pins);
    }            
    
    if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_RD_WR_DIS)) {
        DEF_BIT_SET(p_gpio_fast_reg->FIOMASK, gpio_pins);
    }            

                
                                                                /* ---- I/O MODE/PERIPHERAL FUNCTION CONFIGURATION ---- */
    pinsel_opt  = GPIO_OPT_FNCT_INVALID;                   
    pinmode_opt = GPIO_OPT_MODE_INVALID;
                                                                /* Set PINSELxx based on GPIO_OPT_FNCT_xxx          */
    if (DEF_BIT_IS_SET_ANY(gpio_opt, GPIO_OPT_FNCT_ANY)) {
        if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_FNCT_1)) {
            pinsel_opt = 0x00;
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_FNCT_2)) {
            pinsel_opt = 0x01;
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_FNCT_3)) {
            pinsel_opt = 0x02;
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_FNCT_4)) {
            pinsel_opt = 0x03;
        } else {
            return;
        }
    }

                                                                /* Set PMODExx based on GPIO_OPT_MDOE_xxx         */
    if (DEF_BIT_IS_SET_ANY(gpio_opt, GPIO_OPT_MODE_ANY)) {
        if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_MODE_PULLUP)) {
            pinmode_opt = 0x00;
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_MODE_REPEATER)) {
            pinmode_opt = 0x01;            
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_MODE_NONE)) {
            pinmode_opt = 0x02;
        } else if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_MODE_PULLDOWN)) {
            pinmode_opt = 0x03;
        }
    }
    
    if ((pinsel_opt != GPIO_OPT_FNCT_INVALID) ||
        (pinmode_opt  != GPIO_OPT_MODE_INVALID)) {
        for (pin_nbr = 0; pin_nbr < 32; pin_nbr++) {
            if (DEF_BIT_IS_SET(gpio_pins, DEF_BIT(pin_nbr))) {
                
                if (pinsel_opt != GPIO_OPT_FNCT_INVALID) {
                    if (pin_nbr < 16) {
                        DEF_BIT_CLR(*p_gpio_pinsel, DEF_BIT_FIELD(2, pin_nbr * 2));
                        DEF_BIT_SET(*p_gpio_pinsel, DEF_BIT_MASK(pinsel_opt, pin_nbr * 2));
                    } else {
                        DEF_BIT_CLR(*((CPU_REG32 *)((CPU_INT32U)p_gpio_pinsel + 0x04)), DEF_BIT_FIELD(2, (pin_nbr - 16) * 2));
                        DEF_BIT_SET(*((CPU_REG32 *)((CPU_INT32U)p_gpio_pinsel + 0x04)), DEF_BIT_MASK(pinsel_opt, (pin_nbr - 16) * 2));
                    }
                }
        
                if (DEF_BIT_IS_SET(gpio_pins, DEF_BIT(pin_nbr))) {
                    if (pinmode_opt != GPIO_OPT_MODE_INVALID) {
                        if (pin_nbr < 16) {                        
                            DEF_BIT_CLR(*p_gpio_pinmode, DEF_BIT_FIELD(2, pin_nbr * 2));
                            DEF_BIT_SET(*p_gpio_pinmode, DEF_BIT_MASK(pinmode_opt, pin_nbr * 2));
                        } else {
                            DEF_BIT_CLR(*((CPU_REG32 *)((CPU_INT32U)p_gpio_pinmode + 0x04)), DEF_BIT_FIELD(2, (pin_nbr - 16) * 2));
                            DEF_BIT_SET(*((CPU_REG32 *)((CPU_INT32U)p_gpio_pinmode + 0x04)), DEF_BIT_MASK(pinmode_opt, (pin_nbr - 16) * 2));
                        }
                    }
                }
            }
        }
    }

    if (DEF_BIT_IS_SET(gpio_opt, GPIO_OPT_MODE_OPEN_DRAIN)) {
        *p_gpio_pinmode_od = gpio_pins;
    }
}

/*
 * Name			  :	void  GPIO_Set  (CPU_INT08U   gpio_port,
 *                                       CPU_INT32U   gpio_pin);
 * Input          : 
 *                  gpio_port       The PIO controller port.
 *                                      PIO_PORT0       
 *                                      PIO_PORT1    
 *                                      GPIO_PORT0_FAST
 *                                      GPIO_PORT1_FAST 
 *                                      GPIO_PORT2_FAST
 *                                      GPIO_PORT3_FAST
 *                                      GPIO_PORT4_FAST 
 *
 *                  gpio_pin        Port pin(s) to be set.
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	
 *
*/
void  GPIO_Set  (CPU_INT08U   gpio_port,
                     CPU_INT32U   gpio_pin)
{
    GPIO_FAST_REG  *p_gpio_fast_reg;    


    switch (gpio_port) {
        case GPIO_PORT0_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT0_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT1_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT1_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT2_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT2_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT3_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT3_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT4_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT4_FAST_BASE_ADDR); 
             break;             

        case GPIO_PORT0:
        case GPIO_PORT1:             
        default:
             return;
    
    }

    p_gpio_fast_reg->FIOSET = gpio_pin;
}


/*
 * Name			  :	void  GPIO_Clr  (CPU_INT08U   gpio_port,
 *                                       CPU_INT32U   gpio_pin);
 * Input          : 
 *                  gpio_port       The PIO controller port.
 *                                      PIO_PORT0       
 *                                      PIO_PORT1    
 *                                      GPIO_PORT0_FAST
 *                                      GPIO_PORT1_FAST 
 *                                      GPIO_PORT2_FAST
 *                                      GPIO_PORT3_FAST
 *                                      GPIO_PORT4_FAST 
 *
 *                  gpio_pin        Port pin(s) to be cleared.
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	
 *
*/
void  GPIO_Clr (CPU_INT08U  gpio_port,
                    CPU_INT32U  gpio_pin)
{    
    GPIO_FAST_REG  *p_gpio_fast_reg;    


    switch (gpio_port) {
        case GPIO_PORT0_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT0_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT1_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT1_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT2_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT2_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT3_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT3_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT4_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT4_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT0:
        case GPIO_PORT1:             
        default:
             return;
    }

    p_gpio_fast_reg->FIOCLR = gpio_pin;
}

/*
 * Name			  :	CPU_INT32U  GPIO_StatusGet  (CPU_INT08U   gpio_port);
 * Input          : 
 *                  gpio_port        The PIO controller port.
 *                                       GPIO_PORT0_FAST
 *                                       GPIO_PORT1_FAST 
 *                                       GPIO_PORT2_FAST
 *                                       GPIO_PORT3_FAST
 *                                       GPIO_PORT4_FAST 
 *
 *                  gpio_pin         Port pin(s) to be read.
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	
 *
*/
CPU_INT32U  GPIO_StatusGet  (CPU_INT08U   gpio_port)

{    
    GPIO_FAST_REG  *p_gpio_fast_reg;     
    CPU_INT32U          port_status;


    switch (gpio_port) {
        case GPIO_PORT0_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT0_FAST_BASE_ADDR);
             break;

        case GPIO_PORT1_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT1_FAST_BASE_ADDR);
             break;

        case GPIO_PORT2_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT2_FAST_BASE_ADDR);
             break;

        case GPIO_PORT3_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT3_FAST_BASE_ADDR);
             break;

        case GPIO_PORT4_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT4_FAST_BASE_ADDR);
             break;

        case GPIO_PORT0:
        case GPIO_PORT1:
        default:
             return (DEF_BIT_NONE);
    }
        
    port_status = p_gpio_fast_reg->FIOPIN;

    return (port_status);
}



/*
 * Name			  :	void  GPIO_Toggle (CPU_INT08U   gpio_port,
 *                                         CPU_INT32U   gpio_pin);
 * Input          : 
 *                  gpio_port        The PIO controller port.
 *                                       GPIO_PORT0_FAST
 *                                       GPIO_PORT1_FAST 
 *                                       GPIO_PORT2_FAST
 *                                       GPIO_PORT3_FAST
 *                                       GPIO_PORT4_FAST 
 *
 *                  gpio_pin         Port pin(s) to be read.
 * Output         : none
 * Caller(s)      : Application.
 * Return         : none
 * Notes          :	Toggle an I/O pin(s)
 *
*/
void  GPIO_Toggle (CPU_INT08U   gpio_port,
                       CPU_INT32U   gpio_pin)
{    
    GPIO_FAST_REG  *p_gpio_fast_reg;         
    CPU_INT32U          pin_set;
    CPU_INT32U          pin_clr;
    

    switch (gpio_port) {             
        case GPIO_PORT0_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT0_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT1_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT1_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT2_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT2_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT3_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT3_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT4_FAST:
             p_gpio_fast_reg   = (GPIO_FAST_REG *)(GPIO_REG_PORT4_FAST_BASE_ADDR); 
             break;

        case GPIO_PORT0:
        case GPIO_PORT1:             
        default:
             return;
    }
    
    pin_clr                 =  (p_gpio_fast_reg->FIOPIN) & gpio_pin;    
    pin_set                 = ~(p_gpio_fast_reg->FIOPIN) & gpio_pin;
    p_gpio_fast_reg->FIOCLR = pin_clr;
    p_gpio_fast_reg->FIOSET = pin_set;
}


/* GPIO interrupt set --------------------------------------------------------*/
void GPIO_IntCmd(CPU_INT08U gpio_port, CPU_INT32U gpio_pin, CPU_INT08U edgeState)
{
	if((gpio_port == 0)&&(edgeState == 0))
		LPC_GPIOINT->IO0IntEnR = gpio_pin;
	else if ((gpio_port == 2)&&(edgeState == 0))
		LPC_GPIOINT->IO2IntEnR = gpio_pin;
	else if ((gpio_port == 0)&&(edgeState == 1))
		LPC_GPIOINT->IO0IntEnF = gpio_pin;
	else if ((gpio_port == 2)&&(edgeState == 1))
		LPC_GPIOINT->IO2IntEnF = gpio_pin;
	else
		//Error
		while(1);
}


CPU_INT32U GPIO_GetIntStatus(CPU_INT08U gpio_port, CPU_INT32U gpio_pin, CPU_INT08U edgeState)
{
	if((gpio_port == 0) && (edgeState == 0))//Rising Edge
		return (((LPC_GPIOINT->IO0IntStatR)>>gpio_pin)& 0x1);
	else if ((gpio_port == 2) && (edgeState == 0))
		return (((LPC_GPIOINT->IO2IntStatR)>>gpio_pin)& 0x1);
	else if ((gpio_port == 0) && (edgeState == 1))//Falling Edge
		return (((LPC_GPIOINT->IO0IntStatF)>>gpio_pin)& 0x1);
	else if ((gpio_port == 2) && (edgeState == 1))
		return (((LPC_GPIOINT->IO2IntStatF)>>gpio_pin)& 0x1);
	else
		//Error
		while(1);
}

void GPIO_ClearInt(CPU_INT08U gpio_port, CPU_INT32U gpio_pin)
{
	if(gpio_port == 0)
		LPC_GPIOINT->IO0IntClr = gpio_pin;
	else if (gpio_port == 2)
		LPC_GPIOINT->IO2IntClr = gpio_pin;
	else
		//Invalid portNum
		while(1);
}

