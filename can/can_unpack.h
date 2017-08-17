/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : xp60.uvproj
  * @File Name          : can_unpack.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-10-18
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_UNPACK_H__
#define __CAN_UNPACK_H__

/* public function declaration -----------------------------------------------*/
#pragma pack(1)
typedef struct
{
	INT16U body_length;
	INT8U  protocol_version;
	INT16U sequence_number;
	INT16U cmd_id;
	INT16U flag;
	INT8U  encryp_type;
}GS_HEADER_t;

typedef struct
{
	GS_HEADER_t cmd_header;
	INT8U Red;
	INT8U White;
	INT32S Green;
	INT16U ServoVolage;
	INT32S pitch_ATV;
	INT32S throttle_ATV;
	INT16U wp_cnt;
    FP64 targetlati;
    FP64 targetlongti;
    FP32 targetalti;
	unsigned long fwclock;
}  CMD_GS_SENDGENERALSTATUS_t;

typedef struct
{
	GS_HEADER_t    cmd_header;
     INT32U        voltage_warning_type;
	 FP32          cur_voltage;
	 INT32U        GPS_Count;
	 unsigned long fwclock;
	 INT16U        gsTLstatus;
	 INT16U        osd_flag;              /* 状态位指示 */
	 INT16U        failsafe;
}  CMD_GS_SENDEXTGENERALSTATUS_t;

typedef struct
{
    GS_HEADER_t cmd_header;
	     INT16S roll;
         INT16S pitch;
         INT16S yaw;
         INT16S vx;
         INT16S vy;
         INT16S vz;
         FP64 lati;
         FP64 longti;
         FP32 alti;
         INT8S wp_idx;
		 unsigned long fwclock;		/* 43B+8B */
}  CMD_GS_SENDATTIPOS_t;

/*  __packed 和 #pragma pack(1)有着不同的含义
 *	#pragma pack(1)表示结构在内存中存储时,成员之间没有空洞,是紧密的
 *  __packed表示读该变量时,按照实际地址去寻址变量,而不是将奇数地址（或者是地址不是自然边界的整数倍时）
 * 舍弃余数位后再寻址
 */
__packed struct CFG_Offset_format
{
	FP32    IMU_Offset_x;
	FP32    IMU_Offset_y;
	FP32    IMU_Offset_z;
	FP32    GPS_offset_x;
	FP32    GPS_offset_y;
	FP32    GPS_offset_z;
	INT16U  IMU_Dir;
	INT8U   key;
	INT8U   cmd_type;
};

#pragma pack()

/* slave ack cmd or send pack typedef ------------------------------------------------------------*/

#if UNPACK_ON_ISR > 0
void CAN_rx_int_unpack_ctrl(void);
#else
void CAN_rx_task_unpack_led(void);
void CAN_rx_task_unpack_exchange(void);
//void CAN_rx_task_unpack_upgrade(void);
#endif

void CAN_rx_int_unpack_main_ctl(void);

void CAN_rx_int_unpack_led(void);

void usb_to_can( void );
void uart_to_can( void );

void request_led_color( void );



/* public define -------------------------------------------------------------*/




/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

#endif
