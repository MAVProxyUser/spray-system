

#include "LPC17xx.h"
#include "LPC17xx_uart.h"
#include "user_pwm.h"
#include "cfg_handler.h"
extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

/*
 * PWM 2 -- P2.2  PWM1[3]
 * PWM 4 -- P2.4  PWM1[5]
 * PWM 1 -- P1.20 PWM1[2]
 * PWM 3 -- P1.23 PWM1[4]
*/

#define PWM_ENABLE 		LPC_PWM1->TCR |= (uint32_t)0x01<<3
#define PWM_DISABLE		LPC_PWM1->TCR &= ~((uint32_t)0x01<<3)
//pwm 频率
#define PWM_FREQ  50  

//控制电调：PPM 50HZ 1120us~1920us
void User_PWM_Init(void)
{
	LPC_SC->PCONP|=(1<<6);  //PWM1 power/clock control bit
	//LPC_PINCON->PINSEL4=(LPC_PINCON->PINSEL4&0xfffffccf)|0x0110;//P2.2 P2.4
	LPC_PINCON->PINSEL3=(LPC_PINCON->PINSEL3&0xfffffcff)|0x0200;//P1.20 
	
	LPC_SC->PCLKSEL0=(LPC_SC->PCLKSEL0&~(3<<12))|0x01<<12; //pwm clk = PCLK_peripheral
	PWM_DISABLE;
	LPC_PWM1->PR = 0 ; //不进行分频
	LPC_PWM1->PC = 0 ;
	LPC_PWM1->CTCR =0; //选择非捕获模式	
	LPC_PWM1->MCR=1<<1; //没有中断，也不停止
	//pwm 2 3 4 5  输出使能
	LPC_PWM1->PCR|=((uint32_t)1<<10);
	LPC_PWM1->MR0=SystemCoreClock/PWM_FREQ;
	LPC_PWM1->MR2=100;  //MIN_CNT
	//LPC_PWM1->MR3=100;
	//LPC_PWM1->MR4=130000;
	//LPC_PWM1->MR5=100;
	LPC_PWM1->TCR|=1;
	PWM_ENABLE;
}


//********************************************//
//200000-1001000=100000
//函数参数：num 		电机的号码
//          persent 百分比*100  如：50%  则输入为50 
//********************************************//
void Set_PWM(uint32_t persent)
{
	uint32_t freq = 0;
	static uint32_t freq_last = 0;
	float freq_temp = persent*((MAX_PWM - MIN_PWM)/100) + MIN_PWM;
	freq = (uint32_t)freq_temp;
	
	if(freq > MAX_PWM)	
	  freq = MAX_PWM;
	if(freq < MIN_PWM)	
		freq = MIN_PWM;
	if(freq_last != freq)
	{
	  LPC_PWM1->MR2 = freq;
	  LPC_PWM1->LER |= 1<<2;
	}
	freq_last = freq;
}







