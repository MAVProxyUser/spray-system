/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : OSD_Max7456.uvproj
  * @File Name          : cfg_handler.c
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-11-26
  * @Version            : 1.00
  ******************************************************************************
  * @Description
  *	    cfg interface
  */
/* Includes ------------------------------------------------------------------*/
#include "sbl_config.h"
#include "../../drivers/drivers.h"
#include "../crc/MF_CRC16.h" 
#include "cfg_handler_a2.h"
#include "../../can/can_inc.h"
#include "cfg_inc.h"
//#include "../app/cfg/cfg_handler.h"
#include "../../IAP/sbl_iap.h"

/* extern function declaration -----------------------------------------------*/
void cfg_cmd_process_v2(CPU_INT08U *buf, uint16_t buf_len);
void cmd_handler_send_to_main (uint8_t *buf, unsigned short buf_len);
void cmd_handler_send_to_gps (uint8_t *buf, unsigned short buf_len);
void cmd_handler_send_to_imu(uint8_t *buf, unsigned short buf_len);
extern void can_upgrade_ack( uint8_t * buf , uint16_t len );
/* Private variables ---------------------------------------------------------*/
/* 带CRC校验的包结构 */
const CPU_INT08U CAN_CRC_PACK_HEADER1        = 0x55; /* 包头 */
const CPU_INT08U CAN_CRC_PACK_HEADER2        = 0xAA;
const CPU_INT08U CAN_CRC_PACK_HEADER3        = 0x55;
const CPU_INT08U CAN_CRC_PACK_HEADER4        = 0xAA;

const CPU_INT08U CAN_CRC_PACK_STATE_HEADER1  = 0x01; /* 解包状态 */
const CPU_INT08U CAN_CRC_PACK_STATE_HEADER2  = 0x02;
const CPU_INT08U CAN_CRC_PACK_STATE_HEADER3  = 0x03;
const CPU_INT08U CAN_CRC_PACK_STATE_HEADER4  = 0x04;
const CPU_INT08U CAN_CRC_PACK_STATE_LEN_L    = 0x05;
const CPU_INT08U CAN_CRC_PACK_STATE_LEN_H    = 0x06;
const CPU_INT08U CAN_CRC_PACK_STATE_DATA     = 0x07;
const CPU_INT08U CAN_CRC_PACK_STATE_DATA_SIM = 0x08;

/********************************** CAN cfg unpack *****************************/
const  uint16_t CFG_SEND_BUF_MAX_LEN   = 256;
const  uint16_t CFG_UNPACK_BUF_MAX_LEN = 512;
#pragma arm section zidata = "USB_RAM"
static uint16_t cfg_unpack_rdIdx       = 0;
static uint16_t cfg_unpack_len         = 0;
static uint8_t cfg_unpack_len_low     = 0;
static uint8_t cfg_unpack_state       = CAN_CRC_PACK_STATE_HEADER1;
//static CPU_INT08U cfg_unpack_complete    = 0;
CPU_INT08U cfg_unpack_buf[CFG_UNPACK_BUF_MAX_LEN] __attribute__ ((aligned (4))) = {0};
#pragma arm section zidata

/* 带CRC校验的数据包 解包 */
void cfg_unpack_tick_a2 (void)        
{
	uint32_t cnt  = 0;
	uint8_t byte = 0;

	cnt = CAN_RING_BUF_COUNT( &can1RxUpgradeRingBuf );

	while(cnt--)
	{
		byte = CAN_RING_BUF_RD( &can1RxUpgradeRingBuf );
		//uart_printf(0,"%.2x",byte);
		switch(cfg_unpack_state)
		{
			case CAN_CRC_PACK_STATE_HEADER1:
			{
				if(byte==CAN_CRC_PACK_HEADER1) cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER2;
			}break;
			
			case CAN_CRC_PACK_STATE_HEADER2:
			{
				if(byte==CAN_CRC_PACK_HEADER2) cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER3;  //new protocol
				else cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;				
			}break;
			
			case CAN_CRC_PACK_STATE_HEADER3:
			{
				if(byte==CAN_CRC_PACK_HEADER3) cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER4;
				else cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;
			}break;
			
			case CAN_CRC_PACK_STATE_HEADER4:
			{
				if(byte==CAN_CRC_PACK_HEADER4) {cfg_unpack_state = CAN_CRC_PACK_STATE_LEN_L;}
				else cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;	
			}break;
			
			case CAN_CRC_PACK_STATE_LEN_L:
			{
				cfg_unpack_len_low = byte;
				cfg_unpack_state   = CAN_CRC_PACK_STATE_LEN_H;	
			}break;
			
			case CAN_CRC_PACK_STATE_LEN_H:
			{
				cfg_unpack_len = (uint16_t)byte;
				cfg_unpack_len = (cfg_unpack_len << 8) |  cfg_unpack_len_low;

				/* for sim temp use */
				if(((cfg_unpack_len&0xff00)>>8 == 0x05) || ((cfg_unpack_len&0xff00)>>8 == 0xdd))
				{
					cfg_unpack_len    = cfg_unpack_len_low;
					cfg_unpack_rdIdx  = 2;
					cfg_unpack_state  = CAN_CRC_PACK_STATE_DATA_SIM;
					cfg_unpack_buf[0] = cfg_unpack_len_low;
					cfg_unpack_buf[1] = (CPU_INT08U)((cfg_unpack_len&0xff00)>>8);
					break;
				}
				
				if( cfg_unpack_len >= CFG_UNPACK_BUF_MAX_LEN )
				{
					//VCOM_printf_debug(" check point %s %s 0x%04x\r\n",__FUNCTION__,__LINE__,cmd_length);
					cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;
				}
				else
				{
					//VCOM_printf(" check point %s %s\r\n",__FUNCTION__,__LINE__);
					cfg_unpack_buf[0] = cfg_unpack_len_low;
					cfg_unpack_buf[1] = (CPU_INT08U)((cfg_unpack_len&0xff00)>>8);
					cfg_unpack_rdIdx  = 2;

					cfg_unpack_state  = CAN_CRC_PACK_STATE_DATA;
				}	
			}break;
			
			case CAN_CRC_PACK_STATE_DATA:
			{
				if(cfg_unpack_rdIdx < cfg_unpack_len) 	   /*Note: com_i begin with 2	*/
				{
					cfg_unpack_buf[cfg_unpack_rdIdx] = byte;
					++cfg_unpack_rdIdx;
				}
				if(cfg_unpack_rdIdx >= cfg_unpack_len)
				{
					cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;

					uart_printf(0,"47:%.2x \r\n",cfg_unpack_len);
//				    ctrl_to_wl_package_forward(1, &ctrl_unpack_buf[0], ctrl_unpack_len);
					if( 1 == valuate_checksum(cfg_unpack_buf, cfg_unpack_len) ) /*Checksum succeed*/
					{
//						uart_printf(0,"check sum here\r\n");
						//cfg_unpack_complete = 1;
						cfg_cmd_process_v2(cfg_unpack_buf, cfg_unpack_len);
					}
					else
					{
					    //uart_printf(0,"check sum error\r\n");
						/*TODO: Warn an err. */
					}
				}
			}break;

			default:
			{
			    cfg_unpack_state = CAN_CRC_PACK_STATE_HEADER1;
				break;
			}
		}

// 		if(1 == cfg_unpack_complete)
// 		{
// 		    cfg_unpack_complete = 0;
// 			break; /* 控制解包频率 */ 
// 		}
	} /*End while*/
}


void update_version(unsigned char* p_d)
{
//	p_d[0] = ((WKM_OSD_VERSION & 0xff000000) >> 24);
//	p_d[1] = ((WKM_OSD_VERSION & 0x00ff0000) >> 16);
//	p_d[2] = ((WKM_OSD_VERSION & 0x0000ff00) >> 8);
//	p_d[3] = ((WKM_OSD_VERSION & 0x000000ff) );
//
//	p_d[12] = 0;
}

void init_cmd_header(CMD_HEADER_t* p_head,uint16_t cmd_id,uint16_t bodylength)
{
	p_head->body_length = bodylength;
 	p_head->protocol_version = 40;//g_version;
	p_head->cmd_id = cmd_id;

	p_head->flag =0;   // update_flags();
	p_head->encryp_type = 0;
	update_version(p_head->device_id);
}

void deviceInfoInit (void)
{
  loaderVersion = sbl_get_loader_version();
  canDeviceId = MY_CAN_DEVICE_ID;  /* led 的编号为4 */
	fwVersion = ArgBoardFirmwareVersion;
	canSubDeviceId = 0;
}

void cmd_handler_scan(uint8_t* buf, uint16_t buf_len)
{
	CMD_SCAN_ACK_t cmd_scan_ack;
	CMD_SCAN_NEW_t *cmd_scan = (CMD_SCAN_NEW_t*)buf;

	//PC_log_output(2,"scan cmd len %d %d\r\n", sizeof(CMD_SCAN_NEW_t), buf_len);
  cmd_scan_ack.result = 0;

	if(buf_len-4 == sizeof(CMD_SCAN_NEW_t))
	{
		if(MY_CAN_DEVICE_ID == cmd_scan->canDeviceId)
		{
			init_cmd_header(&cmd_scan_ack.cmd_header,CFG_CMD_ID_SCAN_ACK,sizeof(cmd_scan_ack)+sizeof(uint32_t));
			cmd_scan_ack.result = 0;
			uart_printf( 0 ,"scan ack\r\n" );
			send_cmd(cmd_scan_ack.cmd_header.encryp_type, (uint8_t *)(&cmd_scan_ack), sizeof(cmd_scan_ack));
		}
	}
}
//
void cmd_handler_entry_upgrade_new(uint8_t*buf,uint16_t buf_len)
{
	uint32_t i;
	CMD_ENTRY_UPGRADE_t     *cmd_entry_upgrade = (CMD_ENTRY_UPGRADE_t*)buf;
	CMD_ENTRY_UPGRADE_ACK_t cmd_entry_upgrade_ack;

	if(MY_CAN_DEVICE_ID == cmd_entry_upgrade->canDeviceId)/*  */
	{
			erase_programming_done_flag();
			
		if( *( ( uint32_t * )PROGRAM_FLAG_ADDR ) == 0x0 ) {
			cmd_entry_upgrade_ack.cmd_header = cmd_entry_upgrade->cmd_header;

			init_cmd_header(&cmd_entry_upgrade_ack.cmd_header, CFG_CMDID_ENTRY_UPDATE_NEW_ACK, sizeof(cmd_entry_upgrade_ack)+sizeof(uint32_t) );
			update_version(cmd_entry_upgrade_ack.cmd_header.device_id);

			cmd_entry_upgrade_ack.result = 0;

			send_cmd(cmd_entry_upgrade_ack.cmd_header.encryp_type, (uint8_t*)(&cmd_entry_upgrade_ack), sizeof(cmd_entry_upgrade_ack));
			uart_printf(0,"enter upgrade\r\n");
			i = 50000;
			while(i--);

			WDT_Feed();  /*看门狗溢出时间同步*/
			//关中断,停止喂狗,看门狗复位进入bootloader模式
			while(1);
		}
	}
}

void cmd_handler_entry_upgrade_old(uint8_t*buf,U16 buf_len)
{
	return;
}

CPU_INT08U ledIdBuf[11];
CPU_INT32U loaderVersion;
CPU_INT32U fwVersion;
CPU_INT08U canDeviceId;
CPU_INT08U canSubDeviceId;
void cmd_handler_deviceinfo_set(uint8_t*buf, uint16_t buf_len)
{
  CPU_INT32U                    i;
	CMD_DEVICEINFO_SET_t         *deviceInfoSet = (CMD_DEVICEINFO_SET_t*)buf;
	CMD_DEVICEINFO_REQUEST_ACK_t  cmdACK;
	CPU_INT08U TmpId[11];

	if(deviceInfoSet->info.canDeviceId == MY_CAN_DEVICE_ID)
	{
		cmdACK.cmd_header = deviceInfoSet->cmd_header;
		init_cmd_header(&cmdACK.cmd_header, CFG_CMDID_DEVICEINFO_REQUEST_ACK, sizeof(cmdACK)+sizeof(uint32_t));

		cmdACK.result = 0;

		for(i=0; i<10; i++)
		{
			ledIdBuf[i] = deviceInfoSet->info.hardwareId[i];
		}
		ledIdBuf[10] = '\0';

	    if(hardware_id_write_flash(&ledIdBuf[0],10) == DEF_TRUE)
		{
		    hardware_id_read_flash(&TmpId[0],10);

			for(i=0; i<10; i++)
			{
			    if(TmpId[i] != ledIdBuf[i])break;
			}

			if(10 == i)
			{
			    uart_printf(0,"wkm led write info ok:%s\r\n",&ledIdBuf[0]);
				cmdACK.result = 0;
			}
			else
			{
			    uart_printf(0,"wkm led write info err:%s\r\n",&ledIdBuf[0]);
				cmdACK.result = 1;
			}
		}
		else
		{
		    uart_printf(0,"wkm led config err:%s\r\n",&ledIdBuf[0]);
			cmdACK.result = 1;
		}
	
		hardware_id_read_flash(&TmpId[0],10);
		
		memcpy(cmdACK.info.hardwareId,&TmpId[0],10);
	    cmdACK.info.canSubDeviceId= canSubDeviceId;
		cmdACK.info.canDeviceId   = canDeviceId;
		cmdACK.info.loaderVersion = loaderVersion;
		cmdACK.info.fwVersion     = fwVersion;
		send_cmd(cmdACK.cmd_header.encryp_type, (void *)(&cmdACK), sizeof(cmdACK));
	}
}

void cmd_handler_deviceinfo_check(uint8_t*buf,uint16_t buf_len)
{
    CMD_DEVICEINFO_CONFIG_CHECK_t     *config_check = (CMD_DEVICEINFO_CONFIG_CHECK_t*)buf;
	CMD_DEVICEINFO_CONFIG_CHECK_ACK_t  cmdACK;
	CPU_INT08U                         TmpId[10];

	cmdACK.cmd_header = config_check->cmd_header;
	init_cmd_header(&cmdACK.cmd_header, CFG_CMDID_DEVICEINFO_REQUEST_ACK, sizeof(cmdACK)+sizeof(uint32_t));

	cmdACK.result       = 0;
	cmdACK.config_state = 0x01;

	hardware_id_read_flash(&TmpId[0],10);

	uart_printf(0,"wkm led config check!\r\n");

	memcpy(&cmdACK.HardWareId[0],&TmpId[0],10);

	send_cmd(cmdACK.cmd_header.encryp_type, (void *)(&cmdACK), sizeof(cmdACK));
}

void cmd_handler_deviceinfo_request(uint8_t *buf,unsigned short buf_len)
{
	CMD_DEVICEINFO_REQUEST_t     *deviceInfoRequest = (CMD_DEVICEINFO_REQUEST_t*)buf;
	CMD_DEVICEINFO_REQUEST_ACK_t deviceInfoRequestAck;
	CPU_INT08U                   TmpId[11];

	deviceInfoRequestAck.cmd_header = deviceInfoRequest->cmd_header;
	init_cmd_header(&deviceInfoRequestAck.cmd_header, CFG_CMDID_DEVICEINFO_REQUEST_ACK, sizeof(deviceInfoRequestAck)+sizeof(unsigned int));

	deviceInfoRequestAck.result = 0;
	if(MY_CAN_DEVICE_ID == deviceInfoRequest->canDeviceId)
	{

		hardware_id_read_flash(&TmpId[0],10);
		memcpy(deviceInfoRequestAck.info.hardwareId,&TmpId[0],11);

		deviceInfoRequestAck.info.fwVersion      = fwVersion;
		deviceInfoRequestAck.info.loaderVersion  = loaderVersion;
		deviceInfoRequestAck.info.canDeviceId    = canDeviceId;
		deviceInfoRequestAck.info.canSubDeviceId = canSubDeviceId;
		deviceInfoRequestAck.info.reserved[0]    = 0; /* 1表示在loader模式下 */
	  send_cmd(deviceInfoRequestAck.cmd_header.encryp_type, (void *)(&deviceInfoRequestAck), sizeof(deviceInfoRequestAck));
	  uart_printf(0,"led info request:%.2x!\r\n",loaderVersion);
	}
}


//void cmd_handler_pacakage_forward (uint8_t *buf, unsigned short buf_len)
//{   
//	CPU_INT08U TmpBuf[4];

//	TmpBuf[0] = 0x55;
//	TmpBuf[1] = 0xAA;
//	TmpBuf[2] = 0x55;
//	TmpBuf[3] = 0xAA;

////	CAN_sendbytes_View(&excTxRingBuf, (void *)TmpBuf, 4);

////	CAN_sendbytes_View(&excTxRingBuf, (void *)buf, buf_len);
//}

void cmd_handler_upgrade_start(uint8_t* buf, uint16_t buf_len)
{
	CMD_UPGRADE_START_t* cmd_entry = (CMD_UPGRADE_START_t*)buf;

	if((CAN_DEVICE_ID_CTL == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_RCV == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_DBUS_EXT == cmd_entry->canDeviceId))		
	{
		can_upgrade_ack((void *)buf,buf_len);
	}
}

void cmd_handler_upgrade_data(uint8_t* buf, uint16_t buf_len)
{
	CMD_UPGRADE_DATA_t* cmd_entry = (CMD_UPGRADE_DATA_t*)buf;

	if((CAN_DEVICE_ID_CTL == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_RCV == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_DBUS_EXT == cmd_entry->canDeviceId))		
	{
		can_upgrade_ack((void *)buf,buf_len);
	}
}
void cmd_handler_upgrade_end(uint8_t* buf, uint16_t buf_len)
{
	CMD_UPGRADE_END_t* cmd_entry = (CMD_UPGRADE_END_t*)buf;

	if((CAN_DEVICE_ID_CTL == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_RCV == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_DBUS_EXT == cmd_entry->canDeviceId))		
	{
//		can_upgrade_ack((void *)buf,buf_len);
	}
}

void cmd_handler_upgrade_check(uint8_t* buf, uint16_t buf_len)
{
	CMD_UPGRADE_CHECK_t* cmd_entry = (CMD_UPGRADE_CHECK_t*)buf;

	if((CAN_DEVICE_ID_CTL == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_RCV == cmd_entry->canDeviceId) || (CAN_DEVICE_ID_DBUS_EXT == cmd_entry->canDeviceId))		
	{
		can_upgrade_ack((void *)buf,buf_len);
	}
}

const handler_pair cmd_handler_array[] =
{
	{cmd_handler_entry_upgrade_new    ,CFG_CMDID_ENTRY_UPDATE},
  {cmd_handler_entry_upgrade_new    ,CFG_CMDID_ENTRY_UPDATE_NEW},
	{cmd_handler_entry_upgrade_old    ,CFG_CMDID_ENTRY_UPDATE_OLD},
	{cmd_handler_deviceinfo_set       ,CFG_CMDID_DEVICEINFO_SET},
	{cmd_handler_deviceinfo_request   ,CFG_CMDID_DEVICEINFO_REQUEST},
	{cmd_handler_scan                 ,CFG_CMD_ID_SCAN},
	{cmd_handler_upgrade_start           , CFG_CMDID_UPGRADE_START},
	{cmd_handler_upgrade_data            , CFG_CMDID_UPGRADE_DATA},
	{cmd_handler_upgrade_end             , CFG_CMDID_UPGRADE_END},
	{cmd_handler_upgrade_check           , CFG_CMDID_UPGRADE_CHECK},
	{NULL,NULL}
};


ptr_func_handler cfg_get_handler(uint16_t cmd_id)
{
	int i = 0;

	for( i = 0; cmd_handler_array[i].handler != NULL; i++)
	{
		if(cmd_id == cmd_handler_array[i].cmd_id)
		{
			return cmd_handler_array[i].handler;
		}

	}

	return NULL;
}

void cfg_cmd_process_v2(uint8_t *buf, uint16_t buf_len)
{    
	CMD_HEADER_t     *cmd_header = NULL;
	ptr_func_handler cmd_handler = NULL;

	if(buf_len <sizeof(CMD_HEADER_t))
	{
		//TODO: test & warn
		return;
	}
	cmd_header = (CMD_HEADER_t*)(buf);

	//TODO: process with EncrypType, maybe will change buf_len
   uart_printf(0,"cmd 0x%04x\r\n",cmd_header->cmd_id);
	cmd_handler = cfg_get_handler(cmd_header->cmd_id);

	if(cmd_handler != NULL)
	{
		cmd_handler(buf,buf_len);
	}else
	{
		//to main board
     can_upgrade_ack((void *)buf,buf_len);
	}
}

int valuate_checksum(uint8_t *buf,  uint16_t buf_len)
{
	unsigned short wExpected = 0;

    if ((buf == NULL) || (buf_len <= 2))
    {
        return -1;
    }
    wExpected = Get_CRC16_Check_Sum ( (unsigned char*)buf, buf_len - 4, CRC_INIT);

    return ((char)(wExpected & 0xff) == buf[buf_len - 2] &&
		(char)((wExpected >> 8) & 0xff) == buf[buf_len - 1]);

	//TODO: new check sum algorithm
    //return 1;
}


int encryp_cmd(uint8_t *source, uint16_t source_len, uint8_t* des, uint16_t des_size, uint16_t* p_des_len)
{
	if( NULL == source ||
		NULL == des ||
		source == des ||
		des_size < source_len )
	{
			return -1;
	}

	//TODO: encrytion

	memcpy(des, source, source_len);

	*p_des_len =  source_len;

	return 1;

}
int check_sum_calculate(uint8_t* source,uint16_t source_len,uint16_t* p_des_len)
{
//	if( NULL == source )
//	{
//			return -1;
//	}
//
//	// TODO: check sum
//	*p_des_len = source_len+4;
//
//	return 1;
	unsigned short R_CRC;

	if( NULL == source )
	{
		return -1;
	}

	R_CRC = Get_CRC16_Check_Sum ( (unsigned char *)source, source_len, CRC_INIT );

    source[source_len+2] = (unsigned char)(R_CRC & 0x00ff);
    source[source_len+3] = (unsigned char)((R_CRC>>8)& 0x00ff);

	*p_des_len = source_len+4;

	return 1;
}


uint8_t send_buf[CFG_SEND_BUF_MAX_LEN] = {0};
void send_cmd(uint8_t encryp_type, uint8_t *buf, uint16_t buf_len)
{
	send_buf[0] = 0x55;
	send_buf[1] = 0xAA;
	send_buf[2] = 0x55;
	send_buf[3] = 0xAA;

	if(encryp_cmd(buf, buf_len, &send_buf[4], CFG_SEND_BUF_MAX_LEN - 4, &buf_len) < 0)
	{
		//TODO: warn
		return;
	}

	if( check_sum_calculate(&send_buf[4],buf_len,&buf_len) < 0)// syn header not check sum
	{
	    //TODO: warn
		return;
	}

	can_upgrade_ack( (void*)send_buf, buf_len + 4);  //4 is for 0x55AA55AA
}
/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

