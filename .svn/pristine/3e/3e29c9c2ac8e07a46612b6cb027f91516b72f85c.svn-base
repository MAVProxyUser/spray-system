// CryptoMemory Header File
//
// Detailed descriptions of all functions and structures in the header file can be found in
// the "CryptoMemory Library User Manual".
//
// Revision Date      Changes
// -------- --------- -----------------------------------------------------------------
//    0.1   26 Oct 04 First Release
//    0.2   19 Nov 04 Major additions and corrections

// Protect Library against multiple inclusion
#ifndef CM_LIB_H
#define CM_LIB_H

// -------------------------------------------------------------------------------------------------
// Definations
// -------------------------------------------------------------------------------------------------

// Basic Definations (if not available elsewhere)
#ifndef FALSE
#define FALSE (0)
#define TRUE  (!FALSE)
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

// Device Configuration Register
#define DCR_ADDR      (0x18)
#define DCR_SME       (0x80)
#define DCR_UCR       (0x40)
#define DCR_UAT       (0x20)
#define DCR_ETA       (0x10)
#define DCR_CS        (0x0F)

// Cryptograms
#define CM_Ci         (0x50)
#define CM_Sk         (0x58)
#define CM_G          (0x90)

// Fuses
#define CM_FAB        (0x06)
#define CM_CMA        (0x04)
#define CM_PER        (0x00)

// Password
#define CM_PSW        (0xB0)
#define CM_PWREAD     (1)
#define CM_PWWRITE    (0)

// Return Code Defination
#define SUCCESS       (0)
#define FAILED        (1)
#define FAIL_CMDSTART (2)
#define FAIL_CMDSEND  (3)
#define FAIL_WRDATA   (4)
#define FAIL_RDDATA   (5)
// note: additional specific error codes may be added in the future


#define AT88SC_COMMAND_WUZ   0xB0   //AT88SC的命令 写用户区
#define AT88SC_COMMAND_RUZ   0xB2   //AT88SC的命令 读用户区
#define AT88SC_COMMAND_WSZ   0xB4   //AT88SC的命令 写系统区
#define AT88SC_COMMAND_RSZ   0xB6   //AT88SC的命令 读系统区
#define AT88SC_COMMAND_VC    0xB8   //AT88SC的命令 验证加密
#define AT88SC_COMMAND_VP    0xBA   //AT88SC的命令 验证读写密码
/*
#define AT88SC_COMMAND_WUZ   0x80   //AT88SC的命令 写用户区
#define AT88SC_COMMAND_RUZ   0x82   //AT88SC的命令 读用户区
#define AT88SC_COMMAND_WSZ   0x84   //AT88SC的命令 写系统区
#define AT88SC_COMMAND_RSZ   0x86   //AT88SC的命令 读系统区
#define AT88SC_COMMAND_VC    0x88   //AT88SC的命令 验证加密
#define AT88SC_COMMAND_VP    0x8A   //AT88SC的命令 验证读写密码
*/

#define AT88SC_ATR_ADDR      0x00   //AT88SC的ATR地址//复位应答地址
#define AT88SC_ATR_LEN       0x08   //AT88SC的ATR数据长度

#define AT88SC_FB_ADDR       0x08   //AT88SC的FB地址//工厂代码地址
#define AT88SC_FB_LEN        0x02   //AT88SC的FB数据长度

#define AT88SC_MTZ_ADDR      0x0A   //AT88SC的MTZ地址//测试区地址
#define AT88SC_MTZ_LEN       0x02   //AT88SC的MTZ数据长度

#define AT88SC_CMC_ADDR      0x0C   //AT88SC的CMC地址//制造商代码地址
#define AT88SC_CMC_LEN       0x04   //AT88SC的CMC数据长度

#define AT88SC_LHC_ADDR      0x10   //AT88SC的LHC地址//历史代码地址
#define AT88SC_LHC_LEN       0x08   //AT88SC的LHC数据长度

#define AT88SC_DCR_ADDR      0x18   //AT88SC的DCR地址//设备配置寄存器地址
#define AT88SC_DCR_LEN       0x01   //AT88SC的DCR数据长度

#define AT88SC_NC_ADDR       0x19   //AT88SC的NC地址//身份代码地址
#define AT88SC_NC_LEN        0x07   //AT88SC的NC数据长度

#define AT88SC_AR0_ADDR      0x20   //AT88SC的AR0地址//访问0寄存器地址
#define AT88SC_AR0_LEN       0x01   //AT88SC的AR0数据长度

#define AT88SC_PR0_ADDR      0x21   //AT88SC的PR0地址//密码密钥0寄存器地址
#define AT88SC_PR0_LEN       0x01   //AT88SC的PR0数据长度

	//add 2010-05-30
	#define AT88SC_AR0_PR0_ADDR      0x20
	#define	AT88SC_AR0_PR0_LEN		 0x02

#define AT88SC_AR1_ADDR      0x22   //AT88SC的AR1地址//访问1寄存器地址
#define AT88SC_AR1_LEN       0x01   //AT88SC的AR1数据长度

#define AT88SC_PR1_ADDR      0x23   //AT88SC的PR1地址//密码密钥1寄存器地址
#define AT88SC_PR1_LEN       0x01   //AT88SC的PR1数据长度

	//add 2010-05-30
	#define AT88SC_AR1_PR1_ADDR      0x22
	#define	AT88SC_AR1_PR1_LEN		 0x02

#define AT88SC_AR2_ADDR      0x24   //AT88SC的AR2地址//访问2寄存器地址
#define AT88SC_AR2_LEN       0x01   //AT88SC的AR2数据长度

#define AT88SC_PR2_ADDR      0x25   //AT88SC的PR2地址//密码密钥2寄存器地址
#define AT88SC_PR2_LEN       0x01   //AT88SC的PR2数据长度

	//add 2010-05-30
	#define AT88SC_AR2_PR2_ADDR      0x24
	#define	AT88SC_AR2_PR2_LEN		 0x02

#define AT88SC_AR3_ADDR      0x26   //AT88SC的AR3地址//访问3寄存器地址
#define AT88SC_AR3_LEN       0x01   //AT88SC的AR3数据长度

#define AT88SC_PR3_ADDR      0x27   //AT88SC的PR3地址//密码密钥3寄存器地址
#define AT88SC_PR3_LEN       0x01   //AT88SC的PR3数据长度

	//add 2010-05-30
	#define AT88SC_AR3_PR3_ADDR      0x26
	#define	AT88SC_AR3_PR3_LEN		 0x02

#define AT88SC_IC_ADDR       0x40   //AT88SC的IC地址//发行代码地址
#define AT88SC_IC_LEN        0x10   //AT88SC的IC数据长度

#define AT88SC_Ci0_ADDR      0x51   //AT88SC的Ci0地址//
#define AT88SC_Ci0_LEN       0x07   //AT88SC的Ci0数据长度

#define AT88SC_Sk0_ADDR      0x58   //AT88SC的Sk0地址//会话密钥0地址
#define AT88SC_Sk0_LEN       0x08   //AT88SC的Sk0数据长度

#define AT88SC_Ci1_ADDR      0x61   //AT88SC的Ci1地址//
#define AT88SC_Ci1_LEN       0x07   //AT88SC的Ci1数据长度

#define AT88SC_Sk1_ADDR      0x68   //AT88SC的Sk1地址//会话密钥1地址
#define AT88SC_Sk1_LEN       0x08   //AT88SC的Sk1数据长度

#define AT88SC_Ci2_ADDR      0x71   //AT88SC的Ci2地址//
#define AT88SC_Ci2_LEN       0x07   //AT88SC的Ci2数据长度

#define AT88SC_Sk2_ADDR      0x78   //AT88SC的Sk2地址//会话密钥2地址
#define AT88SC_Sk2_LEN       0x08   //AT88SC的Sk2数据长度

#define AT88SC_Ci3_ADDR      0x81   //AT88SC的Ci3地址//
#define AT88SC_Ci3_LEN       0x07   //AT88SC的Ci3数据长度

#define AT88SC_Sk3_ADDR      0x88   //AT88SC的Sk3地址//会话密钥3地址
#define AT88SC_Sk3_LEN       0x08   //AT88SC的Sk3数据长度

#define AT88SC_G0_ADDR       0x90   //AT88SC的G0地址//密钥种子0地址
#define AT88SC_G0_LEN        0x08   //AT88SC的G0数据长度

#define AT88SC_G1_ADDR       0x98   //AT88SC的G1地址//密钥种子1地址
#define AT88SC_G1_LEN        0x08   //AT88SC的G1数据长度

#define AT88SC_G2_ADDR       0xA0   //AT88SC的G2地址//密钥种子2地址
#define AT88SC_G2_LEN        0x08   //AT88SC的G2数据长度

#define AT88SC_G3_ADDR       0xA8   //AT88SC的G3地址//密钥种子3地址
#define AT88SC_G3_LEN        0x08   //AT88SC的G3数据长度

//add 2010-05-30
#define AT88SC_PW0_WRITE_ADDR   0xB1	//写password write0 
#define AT88SC_PW0_WRITE_LEN    0x03
#define AT88SC_PW0_READ_ADDR	0xB5
#define AT88SC_PW0_READ_LEN		0x03
	
	#define AT88SC_PW0_WRITE_READ_ADDR   0xB1
	#define AT88SC_PW0_WRITE_READ_LEN    0x07

#define AT88SC_PW1_WRITE_ADDR   0xB9	//写password write1 
#define AT88SC_PW1_WRITE_LEN    0x03
#define AT88SC_PW1_READ_ADDR	0xBD
#define AT88SC_PW1_READ_LEN		0x03
	
	#define AT88SC_PW1_WRITE_READ_ADDR   0xB9
	#define AT88SC_PW1_WRITE_READ_LEN    0x07

#define AT88SC_PW2_WRITE_ADDR   0xC1	//写password write2 
#define AT88SC_PW2_WRITE_LEN    0x03
#define AT88SC_PW2_READ_ADDR	0xC5
#define AT88SC_PW2_READ_LEN		0x03
	
	#define AT88SC_PW2_WRITE_READ_ADDR   0xC1
	#define AT88SC_PW2_WRITE_READ_LEN    0x07

#define AT88SC_PW3_WRITE_ADDR   0xC9	//写password write3 
#define AT88SC_PW3_WRITE_LEN    0x03
#define AT88SC_PW3_READ_ADDR	0xCD
#define AT88SC_PW3_READ_LEN		0x03
	
	#define AT88SC_PW3_WRITE_READ_ADDR   0xC9
	#define AT88SC_PW3_WRITE_READ_LEN    0x07

#define AT88SC_PW4_WRITE_ADDR   0xD1	//写password write4 
#define AT88SC_PW4_WRITE_LEN    0x03
#define AT88SC_PW4_READ_ADDR	0xD5
#define AT88SC_PW4_READ_LEN		0x03
	
	#define AT88SC_PW4_WRITE_READ_ADDR   0xD1
	#define AT88SC_PW4_WRITE_READ_LEN    0x07

#define AT88SC_PW5_WRITE_ADDR   0xD9	//写password write5 
#define AT88SC_PW5_WRITE_LEN    0x03
#define AT88SC_PW5_READ_ADDR	0xDD
#define AT88SC_PW5_READ_LEN		0x03
	
	#define AT88SC_PW5_WRITE_READ_ADDR   0xD9
	#define AT88SC_PW5_WRITE_READ_LEN    0x07

#define AT88SC_PW6_WRITE_ADDR   0xE1	//写password write6 
#define AT88SC_PW6_WRITE_LEN    0x03
#define AT88SC_PW6_READ_ADDR	0xE5
#define AT88SC_PW6_READ_LEN		0x03
	
	#define AT88SC_PW6_WRITE_READ_ADDR   0xE1
	#define AT88SC_PW6_WRITE_READ_LEN    0x07

#define AT88SC_PW7_WRITE_ADDR   0xE9	//写password write7 
#define AT88SC_PW7_WRITE_LEN    0x03
#define AT88SC_PW7_READ_ADDR	0xED
#define AT88SC_PW7_READ_LEN		0x03
	
	#define AT88SC_PW7_WRITE_READ_ADDR   0xE9
	#define AT88SC_PW7_WRITE_READ_LEN    0x07


#define AT88SC_KS_0          0xBA   //固定参数KS的长度是随意定义的(现定为4个字节)
#define AT88SC_KS_1          0xDC   //
#define AT88SC_KS_2          0xB4
#define AT88SC_KS_3          0x8A

// Basic Datatypes
typedef unsigned char  uchar;
typedef unsigned char *puchar;  
typedef signed char    schar;
typedef signed char   *pschar;  

//typedef unsigned int   uint;
//typedef unsigned int  *puint;  
//typedef signed int     sint;
//typedef signed int    *psint;
//add 2010-05-10
typedef unsigned short int  uint;
typedef unsigned short int *puint;  
typedef signed short int    sint;
typedef signed short int   *psint;    

// -------------------------------------------------------------------------------------------------

// High Level Function Prototypes

// Select Chip
uchar cm_SelectChip(uchar ucChipId);

// Activate Security
uchar cm_ActiveSecurity(uchar ucKeySet, puchar pucKey, uchar ucEncrypt);

// Deactivate Security
uchar cm_DeactiveSecurity(void);

// Verify Password
uchar cm_VerifyPassword(puchar pucPassword, uchar ucSet, uchar ucRW);

// Reset Password
uchar cm_ResetPassword(void);

// Verify Secure Code
#define cm_VerifySecureCode(CM_PW) cm_VerifyPassword(CM_PW, 7, CM_PWWRITE) 

// Read Configuration Zone
uchar cm_ReadConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);

// Write Configuration Zone
uchar cm_WriteConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount, uchar ucAntiTearing);

// Set User Zone
uchar cm_SetUserZone(uchar ucZoneNumber, uchar ucAntiTearing);

// Read User Zone
uchar cm_ReadLargeZone(uint uiCryptoAddr, puchar pucBuffer, uchar ucCount);

// Read Small User Zone
uchar cm_ReadSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);

// Write User Zone
char cm_WriteLargeZone(uint uiCryptoAddr, puchar pucBuffer, uchar ucCount);

// Write Small User Zone
uchar cm_WriteSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount);

// Send Checksum
uchar cm_SendChecksum(void);

// Read Checksum
uchar cm_ReadChecksum(puchar pucChkSum);

// Read Fuse Byte
uchar cm_ReadFuse(puchar pucFuze);

// Burn Fuse
uchar cm_BurnFuse(uchar ucFuze);

// -------------------------------------------------------------------------------------------------
// Configuration Structures
// -------------------------------------------------------------------------------------------------

// CryptoMemory Low Level Linkage
// 
typedef struct{
    uchar (*Carddetect)(void);
    void (*PowerOff)(void);
    void (*PowerOn)(void);
    uchar (*SendCommand)(puchar pucCommandBuffer);
    uchar (*ReceiveRet)(puchar pucReceiveData, uchar ucLength);
    uchar (*SendData)(puchar pucSendData, uchar ucLength);
    void (*RandomGen)(puchar pucRandomData);
    void (*WaitClock)(uchar ucLoop);
    uchar (*SendCmdByte)(uchar ucCommand);
} cm_low_level;

// CryptoMemory Low Level Configuration
// 
// If any of the supplied CryptoMemory low level library functions are used, this structure must be
// present in the user code. For a detailed description of the elements in the structure, please
// see the "CryptoMemory Library User Manual".
// 
typedef struct{
    uchar ucChipSelect;
    uchar ucClockPort;
    uchar ucClockPin;
    uchar ucDataPort;
    uchar ucDataPin;
    uchar ucCardSensePort;
    uchar ucCardSensePin; 
    uchar ucCardSensePolarity; 
    uchar ucPowerPort;
    uchar ucPowerPin;
    uchar ucPowerPolarity;
    uchar ucDelayCount;
    uchar ucStartTries;
} cm_port_cfg;

// -------------------------------------------------------------------------------------------------
// Externals for Configuration Structures
// -------------------------------------------------------------------------------------------------

extern cm_low_level CM_LOW_LEVEL;
extern cm_port_cfg  CM_PORT_CFG;

// -------------------------------------------------------------------------------------------------
// Other Externals
// -------------------------------------------------------------------------------------------------

extern uchar ucCM_Encrypt;
extern uchar ucCM_Authenticate;
extern uchar ucCM_UserZone;
extern uchar ucCM_AntiTearing;
extern uchar ucCM_InsBuff[4];

// end of multiple inclusion protection
#endif
