/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : BL_WKM2_MAIN.uvproj
  * @File Name          : cfg_unpack.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2012-05-28 
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    Begginning of application   
  */
#include <string.h>
#include "../../cstartup/type.h"
#include "../../drivers/drivers.h"
#include "../../can/can_inc.h"
#include "../../usb/vcom_app.h"
#include "cfg_inc.h"
#include "../crc/MF_CRC8.h"
#include "../crc/MF_CRC16.h"
#include "cfg_decoder.h"
#include "cfg_handler.h"
#include "lpc17xx_uart.h"

#if __USB_ENABLE__
#define USB_UNPACK_MAX_SIZE     (512)
    static uint8_t usb_data_buff[USB_UNPACK_MAX_SIZE];
    cfg_decoder_object_t usb_decoder_obj;
#endif
#if __CAN1_ENABLE__
#define CAN1_UNPACK_MAX_SIZE     (512)
    static uint8_t can1_data_buff[CAN1_UNPACK_MAX_SIZE];
    cfg_decoder_object_t can1_decoder_obj;

		static uint8_t can1_data_buff2[CAN1_UNPACK_MAX_SIZE];
    cfg_decoder_object_t can1_decoder_obj2;

		static uint8_t can1_data_buff_LED[CAN1_UNPACK_MAX_SIZE];
    cfg_decoder_object_t can1_decoder_obj_LED;

		static uint8_t can1_data_buff_upgrade[CAN1_UNPACK_MAX_SIZE];
    cfg_decoder_object_t can1_decoder_obj_upgrade;
#endif
#if __CAN2_ENABLE__
#define CAN2_UNPACK_MAX_SIZE     (512)
    static uint8_t can2_data_buff[CAN2_UNPACK_MAX_SIZE];
    cfg_decoder_object_t can2_decoder_obj;
#endif
#ifdef __UART0__
#define UART0_UNPACK_MAX_SIZE     (512)
    static uint8_t uart0_data_buff[UART0_UNPACK_MAX_SIZE];
    cfg_decoder_object_t uart0_decoder_obj;
#endif
#ifdef __UART1__
#define UART1_UNPACK_MAX_SIZE     (512)
    static uint8_t uart1_data_buff[UART1_UNPACK_MAX_SIZE]; 
    cfg_decoder_object_t uart1_decoder_obj;
#endif
#ifdef __UART2__
#define UART2_UNPACK_MAX_SIZE     (512)
    static uint8_t uart2_data_buff[UART2_UNPACK_MAX_SIZE];
    cfg_decoder_object_t uart2_decoder_obj;
    static uint8_t uart2_data_buff_copy[UART2_UNPACK_MAX_SIZE];
    cfg_decoder_object_t uart2_decoder_obj_copy;		
#endif
#ifdef __UART3__
#define UART3_UNPACK_MAX_SIZE     (512)
    static uint8_t uart3_data_buff[UART3_UNPACK_MAX_SIZE];
    cfg_decoder_object_t uart3_decoder_obj;
#endif

void cfg_unpack_init( void )
{
#if __USB_ENABLE__
    cfg_decoder_init( &usb_decoder_obj, usb_data_buff, USB_UNPACK_MAX_SIZE );
#endif
#if __CAN1_ENABLE__
    cfg_decoder_init( &can1_decoder_obj, can1_data_buff, CAN1_UNPACK_MAX_SIZE );
		cfg_decoder_init( &can1_decoder_obj2, can1_data_buff2, CAN1_UNPACK_MAX_SIZE );
    cfg_decoder_init( &can1_decoder_obj_LED, can1_data_buff_LED, CAN1_UNPACK_MAX_SIZE );
		cfg_decoder_init( &can1_decoder_obj_upgrade, can1_data_buff_upgrade, CAN1_UNPACK_MAX_SIZE );
#endif
#if __CAN2_ENABLE__
    cfg_decoder_init( &can2_decoder_obj, can2_data_buff, CAN2_UNPACK_MAX_SIZE );
#endif
#ifdef __UART0__
    cfg_decoder_init( &uart0_decoder_obj, uart0_data_buff, UART0_UNPACK_MAX_SIZE );
#endif
#ifdef __UART1__
    cfg_decoder_init( &uart1_decoder_obj, uart1_data_buff, UART1_UNPACK_MAX_SIZE );
#endif
#ifdef __UART2__
    cfg_decoder_init( &uart2_decoder_obj, uart2_data_buff, UART2_UNPACK_MAX_SIZE );
		cfg_decoder_init( &uart2_decoder_obj_copy, uart2_data_buff_copy, UART2_UNPACK_MAX_SIZE );
#endif
#ifdef __UART3__
    cfg_decoder_init( &uart3_decoder_obj, uart3_data_buff, UART3_UNPACK_MAX_SIZE );
#endif
}

void cfg_unpack_tick( void )        
{
	uint8_t data = 0;
    uint16_t length = 0;

#if __USB_ENABLE__
	length = OutBufAvailBytes();
    while( length-- ) {
        data = VCOM_GetByte();
        if( cfg_decoder_upack( &usb_decoder_obj, data ) ) {
            g_cfg_end_point = EP_USB;
            command_process_v1( usb_decoder_obj.p_data, usb_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif
#if __CAN1_ENABLE__
	length = CAN_RING_BUF_COUNT( &can1RxRingBuf );
    while( length-- ) {
        data = CAN_RING_BUF_RD( &can1RxRingBuf );
        if( cfg_decoder_upack( &can1_decoder_obj, data ) ) {
            g_cfg_end_point = EP_CAN1;
            command_process_v1( can1_decoder_obj.p_data, can1_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
//CAN1µÄBUF2
	length = CAN_RING_BUF_COUNT( &can1Rx2RingBuf );
    while( length-- ) {
        data = CAN_RING_BUF_RD( &can1Rx2RingBuf );
        if( cfg_decoder_upack( &can1_decoder_obj2, data ) ) {
            g_cfg_end_point = EP_CAN1;
						command_process_v1( can1_decoder_obj2.p_data, can1_decoder_obj2.len );
            g_cfg_end_point = EP_NULL;
        }
    }
//CAN1µÄBUF3
	length = CAN_RING_BUF_COUNT( &can1RxLEDRingBuf );
    while( length-- ) {
        data = CAN_RING_BUF_RD( &can1RxLEDRingBuf );
        if( cfg_decoder_upack( &can1_decoder_obj_LED, data ) ) {
            g_cfg_end_point = EP_CAN1_LED;
            command_process_v1( can1_decoder_obj_LED.p_data, can1_decoder_obj_LED.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif
#if __CAN2_ENABLE__
	length = CAN_RING_BUF_COUNT( &can2RxRingBuf );
    while( length-- ) {
        data = CAN_RING_BUF_RD( &can2RxRingBuf );
        if( cfg_decoder_upack( &can2_decoder_obj, data ) ) {
            g_cfg_end_point = EP_CAN2;
            command_process_v1( can2_decoder_obj.p_data, can2_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif
#if __UART0__
	length = RING_BUF_COUNT( UART0_rx_buf );
    while( length-- ) {
        data = RING_BUF_RD( UART0_rx_buf );
        if( cfg_decoder_upack( &uart0_decoder_obj, data ) ) {
            g_cfg_end_point = EP_UART0;
            command_process_v1( uart0_decoder_obj.p_data, uart0_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif
		

#if __UART1__
	length = RING_BUF_COUNT( UART1_rx_buf );
		if( length != 0 )
    while( length-- ) {
        data = RING_BUF_RD( UART1_rx_buf );
        if( cfg_decoder_upack( &uart1_decoder_obj, data ) ) {
            g_cfg_end_point = EP_UART1;
            command_process_v1( uart1_decoder_obj.p_data, uart1_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif	

#if __UART2__
{
	
	length = RING_BUF_COUNT( UART2_rx_buf_copy );	
	if( length != 0 ){	
    while( length-- ) {
        data = RING_BUF_RD( UART2_rx_buf_copy );

        if( cfg_decoder_upack( &uart2_decoder_obj_copy, data ) ) {
            g_cfg_end_point = EP_UART2;
            command_process_v1( uart2_decoder_obj_copy.p_data, uart2_decoder_obj_copy.len );
            g_cfg_end_point = EP_NULL;
        }
    }
	}
//	length = RING_BUF_COUNT( UART2_rx_buf_copy );	
//	if( length != 0 ){	
//    while( length-- ) {
//        data = RING_BUF_RD( UART2_rx_buf_copy );

////        if( cfg_decoder_upack( &uart2_decoder_obj_copy, data ) ) {
////            g_cfg_end_point = EP_UART2;
////            command_process_v1( uart2_decoder_obj_copy.p_data, uart2_decoder_obj_copy.len );
////            g_cfg_end_point = EP_NULL;
////        }
//			 g_cfg_end_point = EP_UART2;
//			 Flow_Sensor_Unpack(&uart2_decoder_obj_copy, data);
//			 g_cfg_end_point = EP_NULL;
		
}
#endif	

#if __UART3__
	length = RING_BUF_COUNT( UART3_rx_buf );
		if( length != 0 )
    while( length-- ) {
        data = RING_BUF_RD( UART3_rx_buf );
        if( cfg_decoder_upack( &uart3_decoder_obj, data ) ) {
            g_cfg_end_point = EP_UART3;
            command_process_v1( uart3_decoder_obj.p_data, uart3_decoder_obj.len );
            g_cfg_end_point = EP_NULL;
        }
    }
#endif	
}

void PC_LOG( uint8_t num , const uint8_t *pstr, ... )
{
	static uint8_t pc_log_buf[128] ;
	va_list arglist;
	uint16_t len ;
	cmd_header_v1_t *p_cmd = ( cmd_header_v1_t * )pc_log_buf;

	va_start(arglist, pstr);
  len = vsnprintf((void*)&pc_log_buf[sizeof( cmd_header_v1_t )+1],114,(void*)pstr, arglist);
  va_end(arglist);
	
	p_cmd->sof = 0x55;
	p_cmd->vl.version = 1;
	p_cmd->sender.sender_id = DEVICE_ID;
	p_cmd->sender.sender_index = DEVICE_INDEX;
	p_cmd->receiver.receiver_id = 0x19 ; 
	p_cmd->receiver.receiver_index = 0 ;
	p_cmd->seqnum ++ ;
	p_cmd->seqnum = (p_cmd->seqnum > 7000)?0:p_cmd->seqnum ; 
	p_cmd->type.cmd_type = 0;
	p_cmd->type.cmd_ack = 0;
	p_cmd->type.encrypt_type = 0;
	p_cmd->set = 0x0;
	p_cmd->id  = 0x0e;

	pc_log_buf[sizeof( cmd_header_v1_t )]=num;
	pc_log_buf[sizeof( cmd_header_v1_t )+len + 1]='\0' ;
	p_cmd->vl.length = sizeof( cmd_header_v1_t )  + len + 2 + 2 ;

	Append_CRC8_Check_Sum( pc_log_buf , 4 );
	Append_CRC16_Check_Sum( pc_log_buf , p_cmd->vl.length );
	
	CAN_sendbytes_View( LPC_CAN1, &can1TxRingBuf, pc_log_buf, p_cmd->vl.length );
}

