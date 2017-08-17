// CryptoMemory I2C Header File
//
// This library is used in all of the low level hardware interface for I2C communications.
//
// Revision Date      Changes
// -------- --------- -----------------------------------------------------------------
//    0.1   26 Oct 04 First Release

// Protect Library against multiple inclusion
#ifndef CM_I2C_H
#define CM_I2C_H

// -------------------------------------------------------------------------------------------------
// Other includes required by this header file
// -------------------------------------------------------------------------------------------------
//modify 2010-08-05
#include <lpc17xx.h>
#include "..\..\bsp\bsp.h"
#include "cm_lib.h"

// -------------------------------------------------------------------------------------------------
// I/O Port Grouping
//
// Note: the "PORTx" in the header is the last address in the group of three port address
//输入寄存器偏移地址  方向寄存器偏移地址  输出寄存器偏移地址
// -------------------------------------------------------------------------------------------------
//#define IO_PORT_IN  (-2)
//#define IO_PORT_DIR (-1)
//#define IO_PORT_OUT (0)

// -------------------------------------------------------------------------------------------------
// Macros
// -------------------------------------------------------------------------------------------------

//888888888888888888 modify 2010-05-10

//// Define control of the secure memory in terms of the pins defined in CM_PORT_CFG
////SCL所在IO口方向寄存器地址
//#define CM_CLK_PD     (*(volatile unsigned char *)(CM_PORT_CFG.ucClockPort+IO_PORT_DIR))
////SCL所在IO口输出寄存器地址
//#define CM_CLK_PO     (*(volatile unsigned char *)(CM_PORT_CFG.ucClockPort+IO_PORT_OUT))
////SCL所在IO口第几脚
//#define CM_CLK_PIN    (CM_PORT_CFG.ucClockPin)

//使用I2C 0口作为加密芯片口,将该端口初始化为gpio,使用gpio来模拟I2C
//SCL所在IO口设置为输出
//#ifdef __XP60_HARDWARE_V1__
////add 2010-05-10
////set as gpio p0.28 
//#define CM_CLK_GPIO   PINSEL1 &= ~(3ul << 24) 
////set gaio p0.28 as output      
//#define CM_CLK_OUT    FIO0DIR |=  (1ul << 28)  //set gaio p0.28 as output 
//#define CM_CLK_HI     FIO0SET  =  (1ul << 28)  //SCL0所在IO口输出高电平p0.28
////SCL0所在IO口输出低电平p0.28
////#define CM_CLK_LO     FIO0CLR  =  (1ul << 28) 
//#define CM_CLK_LO     FIO0CLR  =  (1ul << 28)  //SCL0所在IO口输出低电平p0.28
////SCL0所在IO口输出高电平p0.28
////#define CM_CLK_HI     FIO0SET  =  (1ul << 28)
//
////888888888888 modify 2010-05-10
////SDA所在IO口输入寄存器地址
////#define CM_DATA_PI    (*(volatile unsigned char *)(CM_PORT_CFG.ucDataPort+IO_PORT_IN))
//////SDA所在IO口方向寄存器地址
////#define CM_DATA_PD    (*(volatile unsigned char *)(CM_PORT_CFG.ucDataPort+IO_PORT_DIR))
//////SDA所在IO口输出寄存器地址
////#define CM_DATA_PO    (*(volatile unsigned char *)(CM_PORT_CFG.ucDataPort+IO_PORT_OUT))
//////SDA所在IO口第几脚
////#define CM_DATA_PIN   (CM_PORT_CFG.ucDataPin)
////SDA所在IO口设置为输出
//// add 2010-05-10 set p0.27 as gpio 
//#define CM_DATA_GPIO  PINSEL1 &= ~(3ul << 22)
////set p0.27 as output
////#define CM_DATA_OUT   FIO0DIR |=  (1ul << 27)
//#define CM_DATA_OUT   FIO0DIR |=  (1ul << 27)  //set p0.27 as output
////SDA所在IO口设置为输入 p0.27
////#define CM_DATA_IN    FIO0DIR &= ~(1ul << 27)
//#define CM_DATA_IN    FIO0DIR &= ~(1ul << 27)  //SDA所在IO口设置为输入 p0.27
////SDA所在IO口输出高电平	p0.27
////#define CM_DATA_HI     FIO0SET =  (1ul << 27)
//#define CM_DATA_HI    FIO0SET  =  (1ul << 27)  //SDA所在IO口输出高电平 p0.27
////SDA所在IO口输出低电平	p0.27
////#define CM_DATA_LO     FIO0CLR =  (1ul << 27)
//#define CM_DATA_LO    FIO0CLR  =  (1ul << 27)  //SDA所在IO口输出低电平 p0.27
////SDA所在IO口读数据p0.27 
////#define CM_DATA_RD    (FIO0PIN & (1ul << 27))
////#define CM_DATA_BIT   ((FIO0PIN >> 27) & 1)
//
//#define CM_DATA_RD    (FIO0PIN & (1ul << 27))
//#define CM_DATA_BIT   ((FIO0PIN >> 27) & 1)
//
//#else

//	#define CM_CLK_GPIO   PINSEL4 &= ~(3ul << 14)  //set as gpio p2.7   
//	#define CM_CLK_OUT    FIO2DIR |=  (1ul << 7)   //set gaio p0.28 as output 
//	#define CM_CLK_HI     FIO2SET  =  (1ul << 7)   //SCL0所在IO口输出高电平p2.7
//	#define CM_CLK_LO     FIO2CLR  =  (1ul << 7)   //SCL0所在IO口输出低电平p2.7

	#define CM_CLK_GPIO   PINSEL1 &= ~(3ul << 14)  //set as gpio p0.23  
	#define CM_CLK_OUT    FIO0DIR |=  (1ul << 23)   //set gaio p0.23as output 
	#define CM_CLK_HI     FIO0SET  =  (1ul << 23)   //SCL0所在IO口输出高电平p0.23
	#define CM_CLK_LO     FIO0CLR  =  (1ul << 23)   //SCL0所在IO口输出低电平p0.23
	
	#define CM_DATA_GPIO  PINSEL4 &= ~(3ul << 12)  //set p2.6 as gpio
	#define CM_DATA_OUT   FIO2DIR |=  (1ul << 6)   //set p2.6 as output
	#define CM_DATA_IN    FIO2DIR &= ~(1ul << 6)   //SDA所在IO口设置为输入 p2.6
	#define CM_DATA_HI    FIO2SET  =  (1ul << 6)   //SDA所在IO口输出高电平 p2.6
	#define CM_DATA_LO    FIO2CLR  =  (1ul << 6)   //SDA所在IO口输出低电平 p2.6
	
	#define CM_DATA_RD    (FIO2PIN & (1ul << 6))
	#define CM_DATA_BIT   ((FIO2PIN >> 6) & 1)
//#endif
//888888888888 modify 2010-05-10
//#define CM_DETECT_PI  (*(volatile unsigned char *)(CM_PORT_CFG.ucCardSensePort+IO_PORT_IN))
//#define CM_DETECT_PD  (*(volatile unsigned char *)(CM_PORT_CFG.ucCardSensePort+IO_PORT_DIR))
//#define CM_DETECT_PIN (CM_PORT_CFG.ucCardSensePin)
//#define CM_DETECT_POL (CM_PORT_CFG.ucCardSensePolarity)
//#define CM_DETECT_IN  CM_DETECT_PD&=~(1<<CM_DETECT_PIN)
//#define CM_DETECT_RD  CM_DETECT_PI&(1<<CM_DETECT_PIN)
//
//#define CM_POWER_PD  (*(volatile unsigned char *)(CM_PORT_CFG.ucPowerPort+IO_PORT_DIR))
//#define CM_POWER_PO  (*(volatile unsigned char *)(CM_PORT_CFG.ucPowerPort+IO_PORT_OUT))
//#define CM_POWER_PIN (CM_PORT_CFG.ucPowerPin)
//#define CM_POWER_POL (CM_PORT_CFG.ucPowerPolarity)
//#define CM_POWER_OUT CM_POWER_PD|=(1<<CM_POWER_PIN)
//#define CM_POWER_HI  CM_POWER_PO|=(1<<CM_POWER_PIN)
//#define CM_POWER_LO  CM_POWER_PO&=~(1<<CM_POWER_PIN)

//set gpio p0.27 p0.28 as output
#define CM_DIR_INIT   CM_CLK_OUT; CM_DATA_OUT

#define CM_TIMER      (CM_PORT_CFG.ucDelayCount)
#define CM_START_TRIES (CM_PORT_CFG.ucStartTries)

// -------------------------------------------------------------------------------------------------
// Macros that replace small common function
// -------------------------------------------------------------------------------------------------

#define CM_CLOCKHIGH  cm_Delay(1);(CM_CLK_HI);cm_Delay(1)
#define CM_CLOCKLOW   cm_Delay(1);(CM_CLK_LO);cm_Delay(1)
#define CM_CLOCKCYCLE cm_Delay(1);(CM_CLK_LO);cm_Delay(2);(CM_CLK_HI);cm_Delay(1)


// Constants used in low level functions
// Power on clocks (spec call for 5, but California Card uses 15)
      //88888888888888888888888
	  //重点要注意的,嵌入式应用应该改为5个clock吗?
#define CM_PWRON_CLKS (15)


// Mid-Level Functions
uchar cm_ReadCommand(puchar pucInsBuff, puchar pucRetVal, uchar ucLen);
uchar cm_WriteCommand(puchar pucInsBuff, puchar pucSendVal, uchar ucLen);

// functions in CM_LOW.C used internally by other low level functions
void cm_Delay(uchar ucDelay);

// Functions in CM_I2C.C used internally by other low level functions
void cm_Clockhigh(void);
void cm_Clocklow(void);
void cm_ClockCycle(void);
void cm_ClockCycles(uchar ucCount);
void cm_Start(void);
void cm_Stop(void);
uchar cm_Write(uchar ucData);
void cm_Ack(void);
void cm_N_Ack(void);
uchar cm_Read(void);
void cm_WaitClock(uchar loop);
uchar cm_SendCommand(puchar pucInsBuff);
uchar cm_ReceiveRet(puchar pucRecBuf, uchar ucLen);
void cm_RandomGen(puchar pucRanddat);

// -------------------------------------------------------------------------------------------------
// Low Level Function Prototypes
// -------------------------------------------------------------------------------------------------

// Placeholder function that always returns TRUE
uchar cm_TRUE(void);

// Placeholder function that always returns SUCCESS
uchar cm_SUCCESS(void);

// Card Detect
uchar cm_CardDetect(void);

// Power On Functions
void cm_FullPowerOn(void);   
void cm_PowerOn(void);   

// Power Off Functions
void cm_FullPowerOff(void);   
void cm_PowerOff(void);

// Receive Data
uchar cm_ReceiveData(puchar pucRecBuf, uchar ucLen);

// Send Data 
uchar cm_SendData(puchar pucSendBuf, uchar ucLen);

// Random
void cm_RandGen(puchar pucRandomData);

// Send Command Byte
uchar cm_SendCmdByte(uchar ucCommand);

// end of multiple inclusion protection
#endif
