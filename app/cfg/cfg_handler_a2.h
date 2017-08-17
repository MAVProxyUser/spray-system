/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : OSD_Max7456.uvproj
  * @File Name          : cfg_handler.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-11-26
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    cfg interface
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_HANDLER_H__
#define __CFG_HANDLER_H__

/* public define -------------------------------------------------------------*/
#define CFG_CMDID_SCAN          		      0x0101
#define CFG_CMDID_SCAN_ACK                    0x0102

//add 2010-08-27 for firmware update
#define CFG_CMDID_ENTRY_UPDATE                0x0242
#define CFG_CMDID_ENTRY_UPDATE_ACK            0x0243


#define CFG_CMDID_UPGRADE_START                0x0fa2
#define CFG_CMDID_UPGRADE_DATA                 0x0fa4
#define CFG_CMDID_UPGRADE_END                  0x0fa6
#define CFG_CMDID_UPGRADE_CHECK                0x0fa8

//add 2012-09-06
#define CFG_CMDID_REQUEST	                  0x09E0
#define CFG_CMDID_REQUEST_ACK                 0x09E1

#define CFG_CMDID_IOSD_POSITION_REQUEST       0x09E2
#define CFG_CMDID_IOSD_POSITION_ACK           0x09E3
#define CFG_CMDID_IOSD_POSITION_SET           0x09E4

#define CFG_CMDID_IOSD_WARNINGS_REQUEST       0x09E5
#define CFG_CMDID_IOSD_WARNINGS_ACK		      0x09E6
#define CFG_CMDID_IOSD_WARNINGS_SET		      0x09E7

#define CFG_CMDID_READ_SDCARD_REQUEST         0x022D
#define CFG_CMDID_READ_SDCARD_ACK             0x022E

/* add for new upgrade  */
#define CFG_CMDID_ENTRY_UPDATE_NEW            0x0fa0
#define CFG_CMDID_ENTRY_UPDATE_NEW_ACK        0x0fa1

#define CFG_CMDID_ENTRY_UPDATE_OLD            0x0242
#define CFG_CMDID_ENTRY_UPDATE_OLD_ACK        0x0243

/* for DEVICEINFO */
#define CFG_CMDID_DEVICEINFO_SET              0x0fb0
#define CFG_CMDID_DEVICEINFO_REQUEST          0x0fb1
#define CFG_CMDID_DEVICEINFO_REQUEST_ACK      0x0fb2

#define CFG_CMD_ID_SCAN          		  0x0101
#define CFG_CMD_ID_SCAN_ACK               0x0102

/* define device id */
#define CAN_DEVICE_ID_CTL                  	  (0x01)
#define CAN_DEVICE_ID_IMU                  	  (0x02)
#define CAN_DEVICE_ID_GPS                  	  (0x03)
#define CAN_DEVICE_ID_LED                  	  (0x04)
#define CAN_DEVICE_ID_RCV                  	  (0x09)
#define CAN_DEVICE_ID_NV08C                   (0x0a)
#define CAN_DEVICE_ID_GLONASS                 (0x0b)
#define CAN_DEVICE_ID_DBUS_EXT                (0x0f)//DBus extern module
#define CAN_DEVICE_ID_GPSRPO_PLUS             (0x22)
#define CAN_DEVICE_ID_GPS_M8                  (0x23)//DBus extern module
#define CAN_DEVICE_ID_FARMING_BOARD           (0x24)//farming board

#define MY_CAN_DEVICE_ID  CAN_DEVICE_ID_FARMING_BOARD
/* extern function declaration -----------------------------------------------*/
#pragma pack(1)
typedef struct
{
	CPU_INT16U body_length;
	CPU_INT08U protocol_version;
	CPU_INT16U sequence_number;
	CPU_INT16U cmd_id;
	CPU_INT16U flag;
	CPU_INT08U encryp_type;
	CPU_INT08U device_id[16];
}CMD_HEADER_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   result;
} RESULT_ACK_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   result;
}	CMD_ENTRY_UPDATE_ACK_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   result;
}CMD_SCAN_ACK_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
} CMD_SCAN_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   canDeviceId;
    CPU_INT08U   canSubDeviceId;
} CMD_SCAN_NEW_t;

typedef struct tagDEVICEInfo
{
	CPU_INT08U canDeviceId;
	CPU_INT08U canSubDeviceId;
	CPU_INT08U hardwareId[10]; //DEVICE 硬件ID
	CPU_INT32U fwVersion;      //固件版本
	CPU_INT32U loaderVersion;  //loader version
	CPU_INT08U reserved[12];   //预留字段，供以后扩展
} DEVICEINFO_BODY_t;

typedef struct
{
   CMD_HEADER_t      cmd_header;
   DEVICEINFO_BODY_t info;
}CMD_DEVICEINFO_SET_t;

typedef struct
{
    CMD_HEADER_t cmd_header;
	CPU_INT08U          canDeviceId;
    CPU_INT08U          canSubDeviceId;
}CMD_DEVICEINFO_REQUEST_t;

typedef struct
{
    CMD_HEADER_t      cmd_header;
	CPU_INT08U        result;
    DEVICEINFO_BODY_t info;
}CMD_DEVICEINFO_REQUEST_ACK_t, CMD_DEVICEINFO_SET_ACK_t;

typedef struct
{
    CMD_HEADER_t cmd_header;
	CPU_INT08U   canDeviceId;
    CPU_INT08U   canSubDeviceId;
}CMD_DEVICEINFO_CONFIG_CHECK_t;

typedef struct
{
    CMD_HEADER_t cmd_header;
	CPU_INT08U   result;
	CPU_INT08U   HardWareId[12];
	CPU_INT08U   config_state;
}CMD_DEVICEINFO_CONFIG_CHECK_ACK_t;


typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   canDeviceId;
	CPU_INT08U   canSubDeviceId;
}	CMD_ENTRY_UPGRADE_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U   result;
	CPU_INT08U   canDeviceId;
	CPU_INT08U   canSubDeviceId;
}	CMD_ENTRY_UPGRADE_ACK_t;

/* FIRMWARE_LODD_START */
typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
} CMD_UPGRADE_START_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          result;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
}CMD_UPGRADE_START_ACK_t;

/* FIRMWARE_LODD_START */
typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          m_Encrypt;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
	CPU_INT08U          firmware_data[256];
} CMD_UPGRADE_DATA_t;

typedef struct
{
  	CMD_HEADER_t cmd_header;
	CPU_INT08U          result;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
}CMD_UPGRADE_DATA_ACK_t;

/* FIRMWARE_LODD_END */
typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
	/* add 2011-05-08 */
	CPU_INT32U          hexLen;
} CMD_UPGRADE_END_t;

typedef struct
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          result;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
} CMD_UPGRADE_END_ACK_t;

typedef struct	 /* check total flash erea */
{
	CMD_HEADER_t cmd_header;
	CPU_INT08U          canDeviceId;
	CPU_INT08U          canSubDeviceId;
	CPU_INT08U          dataBuf[256];
} CMD_UPGRADE_CHECK_t;

typedef void (*ptr_func_handler)(CPU_INT08U*,CPU_INT16U);
typedef struct tag_handler_pair
{
	ptr_func_handler handler;
	CPU_INT16U       cmd_id;
	CPU_INT16U       rsv0;
} handler_pair;

#pragma pack()

extern CPU_INT32U loaderVersion;
extern CPU_INT08U canDeviceId;
extern CPU_INT32U fwVersion;
extern CPU_INT08U canSubDeviceId;

void send_cmd(CPU_INT08U encryp_type, CPU_INT08U *buf, CPU_INT16U buf_len);
int valuate_checksum(CPU_INT08U *buf, CPU_INT16U buf_len);              // check received data's check sum
void cfg_unpack_tick_a2 (void);
void gps_can_handler(void);
void imu_can_handler(void);
void btu_cfg_cache_handler(void);
void deviceInfoInit (void);
/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif
