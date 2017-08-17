/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : Reciever.uvproj
  * @File Name          : can_unpack.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-10-14
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "math.h"
#include "../drivers/drivers.h"


#include "can_inc.h"
#include "../usb/type.h"


/* Private define ------------------------------------------------------------*/
#pragma pack(1)
typedef struct {
	uint16_t body_length;
	uint8_t protocol_version;
	uint16_t sequence_number;
	uint16_t cmd_id;
	uint16_t flag;
	uint8_t encryp_type;
	uint8_t device_id[16];
} CMD_HEADER_BLE_t;

typedef struct {
	CMD_HEADER_BLE_t hdr;
	uint8_t  result;
	uint8_t  deviceName[10];
	uint8_t  password[6];
	uint8_t  DeviceInfoChecksum[4];
	//1 aceone,2 acewp,3. wkh,4.wkm,5.nazam, 6.??  ,7.nazah
	uint8_t  mctype;
} DeviceInfo_t;
#pragma pack()

/* Private variables ---------------------------------------------------------*/

/* imu pack header IMU与主控风格的包*/
const CPU_INT08U CAN_IMU_PACK_HEADER1        = 0x55; /* 包头与包尾 */
const CPU_INT08U CAN_IMU_PACK_HEADER2        = 0xAA;
const CPU_INT08U CAN_IMU_PACK_HEADER3        = 0x55;
const CPU_INT08U CAN_IMU_PACK_HEADER4        = 0xAA;
const CPU_INT08U CAN_IMU_PACK_END1           = 0x66;
const CPU_INT08U CAN_IMU_PACK_END2           = 0xCC;
const CPU_INT08U CAN_IMU_PACK_END3           = 0x66;
const CPU_INT08U CAN_IMU_PACK_END4           = 0xCC;

/* imu package unpack state */
const CPU_INT08U CAN_IMU_PACK_STATE_HEADER1 = 0;      /* 解包状态 */
const CPU_INT08U CAN_IMU_PACK_STATE_HEADER2 = 1;
const CPU_INT08U CAN_IMU_PACK_STATE_HEADER3 = 2;
const CPU_INT08U CAN_IMU_PACK_STATE_HEADER4 = 3;
const CPU_INT08U CAN_IMU_PACK_STATE_CMD_L   = 4;
const CPU_INT08U CAN_IMU_PACK_STATE_CMD_H   = 5;
const CPU_INT08U CAN_IMU_PACK_STATE_LEN_L   = 6;
const CPU_INT08U CAN_IMU_PACK_STATE_LEN_H   = 7;
const CPU_INT08U CAN_IMU_PACK_STATE_DATA    = 8;
const CPU_INT08U CAN_IMU_PACK_STATE_END1    = 9;
const CPU_INT08U CAN_IMU_PACK_STATE_END2    = 10;
const CPU_INT08U CAN_IMU_PACK_STATE_END3    = 11;
const CPU_INT08U CAN_IMU_PACK_STATE_END4    = 12;

/********************************** CAN led Rx unpack *****************************/
const  CPU_INT16U LED_UNPACK_BUF_MAX_LEN = 128;
//static CPU_INT16U led_unpack_rdIdx       = 0;
//static CPU_INT16U led_unpack_cmd         = 0;
//static CPU_INT08U led_unpack_cmd_low     = 0;
//static CPU_INT16U led_unpack_len         = 0;
//static CPU_INT08U led_unpack_len_low     = 0;
//static CPU_INT08U led_unpack_state       = CAN_IMU_PACK_STATE_HEADER1;
//static CPU_INT08U led_unpack_complete    = 0;
CPU_INT08U led_unpack_buf[LED_UNPACK_BUF_MAX_LEN] __attribute__ ((aligned (4))) = {0};

/* extern variables ----------------------------------------------------------*/
extern uint8_t blooth_on_flag;
/* public Function -----------------------------------------------------------*/
int gs_cal_crc(unsigned char *ptr, unsigned char len);


extern int valuate_checksum(CPU_INT08U *buf, CPU_INT16U buf_len);
extern int check_sum_calculate(uint8_t* source,uint16_t source_len,uint16_t* p_des_len);
#define UNPACK_BLE_MAX_LEN 60

#if defined(__LED_BT21__)
extern uint8_t blooth_get_info_flag;

static uint32_t elf_hash( const uint8_t *name, uint8_t len )  
{  
  uint8_t idx = 0;
  uint32_t h = 0, g = 0;
  for( idx = 0; idx < len; idx++ ) {  
    h = ( h << 4 ) + *( name + idx );  
    if( ( g = h & 0xF0000000L ) != 0 )  
      h ^= ( g >> 24 );  
    h &= ~g;  
  }  
  return h;  
} 

static uint8_t deviceCheckHash( const uint8_t *pd )
{
  uint8_t nameLen = 0, pwdLen = 6, buf[16];
	uint32_t hash = 0, dhash = 0;
	
  nameLen = strlen( ( char * )pd );
  nameLen = ( nameLen >= 10 ) ? 10 : nameLen;
  
  memcpy( &buf[0], &pd[0], nameLen );
  memcpy( &buf[nameLen], &pd[10], 6 );
  
  hash = elf_hash( &buf[0], nameLen + pwdLen );
  if( hash == 0 || hash == 0xFFFFFFFF ) {
    hash = 0x55AA55AA;
  }
  
  dhash = *( ( uint32_t *)&pd[16]);
  if( hash == dhash ) {
    return 1; 
  } else {
    return 0; 
  }
}

static uint32_t hardnameCheckHash( const uint8_t *pd )
{
  uint32_t hash = elf_hash( &pd[0], 8 );
  if( hash == 0 || hash == 0xFFFFFFFF ) {
    hash = 0x55AA55AA;
  }
  return hash; 
}

#endif

#if defined(__LED_BT21__)
#pragma pack(1)
typedef struct {
	uint16_t length;
	uint8_t  version;
	uint16_t sequence_number;
	uint16_t cmdID;
	uint16_t Flags;
	uint8_t  encrypt;           
	uint8_t  major;
	uint8_t  minor;
	uint8_t  build_low;
	uint8_t  build_high;
	uint8_t  hardwareID[8];
	uint8_t  error;
	uint8_t  reserved[3];
} cmd_hdr;

typedef struct {
	uint8_t cmd[5];   		// 模块AT命令
	uint8_t type;  				// 产品区分 'W',悟空 'N',哪吒
	uint8_t flag1; 				// 分隔符
	uint8_t hw_id_crc[4]; // 由硬件ID计算出的CRC
	uint8_t flag2;				// 分隔符
	uint8_t dev_name[10];	// 设备名
} cmd_bt21;
#pragma pack()
#endif

//static uint32_t elf_hash( const uint8_t *name, uint8_t len )  
//{  
//  uint8_t idx = 0;
//  uint32_t h = 0, g = 0;
//  for( idx = 0; idx < len; idx++ ) {  
//    h = ( h << 4 ) + *( name + idx );  
//    if( ( g = h & 0xF0000000L ) != 0 )  
//      h ^= ( g >> 24 );  
//    h &= ~g;  
//  }  
//  return h;  
//} 

//static uint8_t deviceCheckHash( const uint8_t *pd )
//{
//  uint8_t nameLen = 0, pwdLen = 6, buf[16];
//	uint32_t hash = 0, dhash = 0; //idx
//	
//  nameLen = strlen( ( char * )pd );
//  nameLen = ( nameLen >= 10 ) ? 10 : nameLen;
//  
//  memcpy( &buf[0], &pd[0], nameLen );
//  memcpy( &buf[nameLen], &pd[10], 6 );
//  
//  hash = elf_hash( &buf[0], nameLen + pwdLen );
//  if( hash == 0 || hash == 0xFFFFFFFF ) {
//    hash = 0x55AA55AA;
//  }
//  
//  dhash = ( ( pd[19] << 24 ) | ( pd[18] << 16 ) | ( pd[17] << 8 ) | pd[16] );
//	
//  if( hash == dhash ) {
//    return 1; 
//  } else {
//    return 0; 
//  }
//}

//static uint32_t deviceGetHash( const uint8_t *pd )
//{
//  uint8_t nameLen = 0, pwdLen = 6, buf[16];
//	uint32_t hash = 0;
//	
//  nameLen = strlen( ( char * )pd );
//  nameLen = ( nameLen >= 10 ) ? 10 : nameLen;
//  
//  memcpy( &buf[0], &pd[0], nameLen );
//  memcpy( &buf[nameLen], &pd[10], 6 );
//  
//  hash = elf_hash( &buf[0], nameLen + pwdLen );
//  if( hash == 0 || hash == 0xFFFFFFFF ) {
//    hash = 0x55AA55AA;
//  }
//  
//  return hash; 
//}

//static uint32_t hardnameCheckHash( const uint8_t *pd )
//{
//	uint32_t hash = 0;
//	hash = elf_hash( &pd[0], 8 );
//  if( hash == 0 || hash == 0xFFFFFFFF ) {
//    hash = 0x55AA55AA;
//  }
//  return hash; 
//}

/* can to usb,can to uart2 */
CPU_INT08U exBuf[512];
#if defined(__LED_BT40__)
static CPU_INT32U BloothSendCnt = 0;
#endif
//static CPU_INT32U Led_on_timer = 0;


const CPU_INT08U request_cmd[16] = {0x55,0xaa,0x55,0xaa,0x09,0x01,0x04,0x00,0x01,0x02,0x03,0x04,0x66,0xcc,0x66,0xcc};
//static CPU_INT32U request_pr = 0;

int gs_cal_crc(unsigned char *ptr, unsigned char len)
{
	unsigned short crc;
	unsigned char da;
	unsigned char calen = len - 4;
	unsigned char * calptr = ptr;
	unsigned char temp;
	unsigned short crc_ta[16]={ /* CRC 余式表 */
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	};
	crc=0;

	while(calen--!=0)
	{
		da=(unsigned char)(((crc&0xf000)>>12)&0x000f); /* 暂存 CRC 的高4 位 */

		//da=((unsigned char)(crc/256))/16;            /* 暂存 CRC 的高四位 */
		crc<<=4;                                       /* CRC 右移4 位，相当于取CRC 的低12 位）*/
		temp = *calptr;
		crc^=crc_ta[da^(temp>> 4)&0x0f];               /*为什么右移之后会变成0xf#. */
		/* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，然后加上上一次 CRC 的余数 */

		da=(unsigned char)(((crc&0xf000)>>12)&0x000f); /* 暂存 CRC 的高4 位 */

		//da=((unsigned char)(crc/256))/16;            /* 暂存 CRC 的高4 位 */
		crc<<=4;                                       /* CRC 右移4 位， 相当于 CRC 的低12 位） */
		temp = *calptr;
		crc^=crc_ta[da^(temp&0x0f)];                   /* CRC 的高4 位和本字节的后半字节相加后查表计算CRC,然后再加上上一次 CRC 的余数 */
		calptr++;
	}

	temp = (unsigned char)(((crc&0xff00)>>8)&0x00ff);
	da   = (unsigned char)(crc & 0x00ff);
//	uart_printf(0,"0x%.2x\r\n",ptr[len-2]);
//	uart_printf(0,"0x%.2x\r\n",ptr[len-1]);
	if( ptr[len-3] == temp && ptr[len-4] == da &&ptr[len-2]==0xab&&ptr[len-1]==0xcd)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

