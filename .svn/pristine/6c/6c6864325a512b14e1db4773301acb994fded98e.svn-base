/*
********************************************************************************
**                       DJI xp40 code
** project: xp40.Uv2                        file: cm_app.c
** environment: keil mdk3.7/lpc2368/72M cclock
** history:
**   v1.0 2010-05-10   Motorfeng
********************************************************************************
*/
#ifndef __CM_APP_H__
#define __CM_APP_H__


#define AT88SC0104_LEN  (128)

#define AT88SC6416_LEN  (1024)

//AT88SC ZONE LENGTH
#define AT88SC0104_ZONE_LEN  (32)

#define AT88SC6416_ZONE_LEN  (128)

//AT88SC pagesize
#define AT88SC6416_PAGE_SIZE (16)


extern unsigned char SerialNumber_t[33];

void cm_init( void );
unsigned char cm_test( unsigned char *ucData );
void cm_process(unsigned short Cmd_sub);
unsigned char display_configzone(void);
int get_ProductId(unsigned char *ProductId);
unsigned char cm_read_data(unsigned char*BufferIn,unsigned short offset,unsigned short len);

void Active_At88sc(void);
int get_SerialNumber(unsigned char *SerialNumber,unsigned short int length);
#endif
/*
********************************************************************************
*                        THE END
********************************************************************************
*/
