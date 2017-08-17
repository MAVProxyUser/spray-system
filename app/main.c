#include "LPC17xx.h"
#include "../cstartup/type.h"
#include "../drivers/drivers.h"
#include "../can/can_inc.h"
#include "../app/cfg/cfg_unpack.h"       
#include "../app/cfg/cfg_handler.h"
#include "../app/cfg/cfg_handler_a2.h"
//#include "cm_at88/cm_app.h"
#include "../bsp/bsp.h"
#include "../cstartup/type.h"
#include "../iap/sbl_iap.h"
#include "../iap/sbl_config.h"
#include "../usb/usb_composite_app.h"
#include "../drivers.h"
//__DEBUG_UART2_PRINTF__,
volatile uint32_t g_device_status = 0x00000001;
volatile uint32_t g_current_tick=0;
uint32_t g_PWM_SET[2] = {0}; 
speed_xy_t g_stLoaclBodySpeed;
void deviceInfoInit (void);
void MotorProcess( void );
void Motor_ST_Init(void);
void pump_aging_test(void);
uint8_t MotorCheck(void );
void Esc_Ver_Check(void); 
void cfg_unpack_tick_a2(void);
uint8_t remaining_capacity_detection( void );
uint32_t clock = 0;
uint32_t foc_switch_count = 0;
uint16_t liquid_capacity;
/** 温度变量  **/
extern uint16_t flow_sensor_value;
extern uint16_t press0_value_min,press1_value_min;
extern uint16_t current_capacity_total;
uint8_t debug_PWM_present=0;
extern uint16_t Throttle_Value_1,Throttle_Value_2;
extern spray_system_flow_cali_para_t spray_system_flow_cali_para;// for test
uint8_t start_flag = 0;
extern uint16_t debug_throttle_value_1,debug_throttle_value_2;//for debug
extern cmd_foc_ack_t debug_stFOC_Value_1,debug_stFOC_Value_3;//for debug
int main( void ) 
{
	SystemInit();
	SystemCoreClockUpdate();  
	UARTS_Init();	
	WDT_Init();
  Interrupt_Init( );
	uart_printf( 0, "UARTS_Init\r\n" );
#if ( __CAN1_ENABLE__ )
	CAN_Init();
#endif
#if __USB_ENABLE__
	USB_Init_CDC();
#endif
	TIMER0_Init(); 
	TIMER1_Init(); 
	TIMER2_Init(); 
	LedInit();
	level_sensor_init();
#if __LPC15xx_Fitting__	
  User_PWM_Init();
	uart_swtich_init();
	TX_RX1_ON();
#endif
	sensor_adc_init();
  WDT_Start( 1000 );  
	if( WDT_ResetCheck() ) {
		uart_printf( 0, "wdt reset\r\n" );
	}
  uart_printf( 0, "mcu reset\r\n" );
	TIMER0_Start();
  cfg_unpack_init();
	deviceInfoInit(); 
	set_foc_throttle_value(FOC_1,FOC_1_STATUS);
	set_foc_throttle_value(FOC_2,FOC_2_STATUS);
	spray_system_press_para_init_check();// 压力传感器初始化参数验证
	spray_sysem_flow_para_init_check();  // 流量传感器初始化参数验证
	spray_residual_volume_init_check();  // 无药报警时是否剩余少量药
	Esc_Ver_Check();
	while( 1 ) 
	{
		while( !TIMER0_1Ms_Timeout() );
		clock++;	
		if(clock % 2 == 0) // 500HZ
		{
			  temperature_sensor_handle();
			  press_sensor_handle();	
				cfg_unpack_tick(); 
				cfg_unpack_tick_a2();	
		}
		
    if(clock % 10 == 0) // 100HZ
		{  	
			  foc_switch_count++;
			  if(foc_switch_count%2)
				{
						cmd_hander_foc_request(CMD_NO_ORDER , FOC_2); //50HZ 
				}
				else 
				{
						cmd_hander_foc_request(CMD_NO_ORDER , FOC_1);
				}
		    WDT_Feed();
				remaining_capacity_detection();
		}	
	  if(clock % 10 == 0) // 20HZ 
		{
	     level_data_process();
		}
		
		if(clock % 100 == 0) //10HZ 
		{
			  if(get_capacity_flag_1L() == 0)
				{			
					  pressure1_calibration_process(); //如果校准失败（数据异常）  再次校准
						pressure2_calibration_process(); //如果校准失败（数据异常）  再次校准
				}	
			  flow1_calibration_process();
				flow2_calibration_process();
				spray_system_para_recoder();
		}

		if(clock % 200 == 0) //5HZ
		{
		  spary_sys_status_push();
		}
		
		if(clock % 1000 == 0) // 1HZ
		{
			 capacity_by_flow_sensor();
			 //if(start_flag == 0)
			 {
    	     uart_printf( 2, "#####press1_value_min = %d idx_7_freq = %d press0_value_min = %d  current_capacity_total = %d  \r\n", 
				   press1_value_min,flow_sensor_value,press0_value_min,current_capacity_total);
				   uart_printf( 2, "acc_signal  = %d   debug_throttle_value_2 = %d \r\n", debug_stFOC_Value_3.acc_signal, debug_throttle_value_2);	
			 }
		}
	}
}


void Esc_Ver_Check(void) 
{	
//cmd_handler_foc_ver_request( FOC_1);
	cmd_handler_foc_ver_request( FOC_2);
}
