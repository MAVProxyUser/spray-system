// Functions that directly control the hardware

#include <stdlib.h>
//add 2010-05-10
//#include "..\app\includes.h"

#include "cm_lib.h"
#include "cm_i2c.h"
#include "cm_gpa.h"


// Data Structures that configure the low level CryptoMemory functions

// CryptoMemory Low Level Linkage
// 
cm_low_level CM_LOW_LEVEL = {
    cm_TRUE,         // Carddetect  卡检测
    cm_PowerOff,     // PowerOff    电源关闭
    cm_PowerOn,      // PowerOn     加电
    cm_SendCommand,  // SendCommand 发送命令
    cm_ReceiveData,  // ReceiveRet  接收数据
    cm_SendData,     // SendData    发送数据
    cm_RandGen,      // RandomGen   随机
    cm_WaitClock,    // WaitClock   等待时钟
    cm_SendCmdByte   // SendCmdByte 发送命令字节
};

// CryptoMemory Low Level Configuration
//
// Note: the port address is included in a manner that does not require a chip
//       specific header file. Note, the address of the port is the LAST address
//       of the group of three addresses of the port (the port output register).
//
/*cm_port_cfg  CM_PORT_CFG = {
    0xb0, // ucChipSelect        (0xb0 is default address for CryptoMemory)
    0x32, // ucClockPort         (0x32 is PORTD)
    0,    // ucClockPin          (SCL on bit 0)
    0x32, // ucDataPort          (0x32 is PORTD)
    2,    // ucDataPin           (SDA on bit 2)    
    0x32, // ucCardSensePort     (0x32 is PORTD)
    1,    // ucCardSensePin      (card sense switch, if any, on bit 2) 
    TRUE, // ucCardSensePolarity (TRUE -> "1" on bit in register means card is inserted)
    0x32, // ucPowerPort         (0x32 is PORTD)
    3,    // ucPowerPin          (power control, if any, on bit 3)
    TRUE, // ucPowerPolarity     (TRUE -> "1" on bit in register supplies power)
    100,  // ucDelayCount
    10    // ucStartTries
};*/

//延时是要重点改的地方 由100 改为1000 延时增加10 (atmegea 时钟8M; lpc2368 时钟72M)
cm_port_cfg  CM_PORT_CFG = {
    0xb0, // ucChipSelect        (0xb0 is default address for CryptoMemory)
    0x35, // ucClockPort         (0x35 is PORTC)
    5,    // ucClockPin          (SCL on bit 5)
    0x35, // ucDataPort          (0x35 is PORTD)
    4,    // ucDataPin           (SDA on bit 4)    
    0x32, // ucCardSensePort     (0x32 is PORTD)
    1,    // ucCardSensePin      (card sense switch, if any, on bit 2) 
    TRUE, // ucCardSensePolarity (TRUE -> "1" on bit in register means card is inserted)
    0x32, // ucPowerPort         (0x32 is PORTD)
    3,    // ucPowerPin          (power control, if any, on bit 3)
    TRUE, // ucPowerPolarity     (TRUE -> "1" on bit in register supplies power)
    //100,  // ucDelayCount
    100,
	15    // ucStartTries
};

// Data and Functions used by other low level functions
//
// Note: this module must be after all other low level functions in the library
//       to assure that any reference to functions in this library are satistified.

// functions in CM_LOW.C used internally by other low level functions

// Zone Data
uchar ucCM_UserZone;
uchar ucCM_AntiTearing;

// Chip state
uchar ucCM_Encrypt;
uchar ucCM_Authenticate;

// Global data
uchar ucCM_InsBuff[4];

// Delay
void cm_Delay(uchar ucDelay)
{
	uchar ucTimer;
	  
    while(ucDelay) {
    	ucTimer = CM_TIMER;
        while(ucTimer) ucTimer--;
    	ucDelay--;
    }	
}

// 1/2 Clock Cycle transition to HIGH
//
void cm_Clockhigh(void)
{
    cm_Delay(1);
    CM_CLK_HI;
    cm_Delay(1);
}

// 1/2 Clock Cycle transition to LOW
//
void cm_Clocklow(void)
{
    cm_Delay(1);
    CM_CLK_LO;
    cm_Delay(1);
}

// Do one full clock cycle
//
// Changed 1/19/05 to eliminate one level of return stack requirements
//
void cm_ClockCycle(void)
{
    cm_Delay(1);
    CM_CLK_LO;
    cm_Delay(2);
    CM_CLK_HI;
    cm_Delay(1);
}

// Do a number of clock cycles
//
void cm_ClockCycles(uchar ucCount)
{
    uchar i;
    
    for (i = 0; i < ucCount; ++i) cm_ClockCycle();
}

// Send a start sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
void cm_Start(void)
{
	  CM_DATA_OUT;                         // Data line must be an output to send a start sequence
	  cm_Clocklow();
	  CM_DATA_HI;
	  cm_Delay(4);
	  cm_Clockhigh();
	  cm_Delay(4);
	  CM_DATA_LO;
	  cm_Delay(8);
	  cm_Clocklow();
	  cm_Delay(8);
}

// Send a stop sequence
//
// Modified 7-21-04 to correctly set SDA to be an output
// 
void cm_Stop(void)
{
	CM_DATA_OUT;                         // Data line must be an output to send a stop sequence
	cm_Clocklow();
	CM_DATA_LO;
	cm_Delay(4);
	cm_Clockhigh();
	cm_Delay(8);
	CM_DATA_HI;
	cm_Delay(4);
}

// Write a byte
//
// Returns 0 if write successed, 1 if write fails failure
//
// Modified 7-21-04 to correctly control SDA
// 
uchar cm_Write(uchar ucData)
{
    uchar i;

    CM_DATA_OUT;                         // Set data line to be an output
    for(i=0; i<8; i++) {                 // Send 8 bits of data
        cm_Clocklow();
        if (ucData&0x80) CM_DATA_HI;
        else             CM_DATA_LO;
        cm_Clockhigh();
        ucData = ucData<<1;
    }
    cm_Clocklow();

    //add 2011-01-12
	i = 40;  //
	// wait for the ack
    CM_DATA_IN;                      // Set data line to be an input
    //cm_Delay(8);
    cm_Delay(16);
	cm_Clockhigh();
    while(i>1) {                    // loop waiting for ack (loop above left i == 8)
        //cm_Delay(2);
        cm_Delay(20);
		if (CM_DATA_RD) i--;        // if SDA is high level decrement retry counter
        else i = 0;
    }      
    cm_Clocklow();
    CM_DATA_OUT;                     // Set data line to be an output
    return i;
}

// Send a ACK or NAK or to the device
void cm_AckNak(uchar ucAck)
{
	CM_DATA_OUT;                         // Data line must be an output to send an ACK
	cm_Clocklow();
	if (ucAck) CM_DATA_LO;               // Low on data line indicates an ACK
	else       CM_DATA_HI;               // High on data line indicates an NACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
}

#ifdef PIGS_FLY

// ------------------------------------------------------------------------------------- 
// Original Version
// ------------------------------------------------------------------------------------- 

// Send a ACK to the device
void cm_Ack(void)
{
	CM_DATA_OUT;                         // Data line must be an output to send an ACK
	cm_Clocklow();
	CM_DATA_LO;                          // Low on data line indicates an ACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
	//SET_SDA;
}

// Send a NACK to the device
void cm_N_Ack(void)
{
	CM_DATA_OUT;                         // Data line must be an output to send an NACK
	cm_Clocklow();
	CM_DATA_HI;                          // High on data line indicates an NACK
	cm_Delay(2);
	cm_Clockhigh();
	cm_Delay(8);
	cm_Clocklow();
	//SET_SDA;
}

// ------------------------------------------------------------------------------------- 
// Version that uses one less level of call stack
// ------------------------------------------------------------------------------------- 

// Send a ACK to the device
void cm_Ack(void)
{
    CM_DATA_OUT;                         // Data line must be an output to send an ACK
    cm_Delay(1);
    CM_CLK_LO;
    cm_Delay(1);
    CM_DATA_LO;                          // Low on data line indicates an ACK
    cm_Delay(3);
    CM_CLK_HI;
    cm_Delay(9);
    cm_Clocklow();
}

// Send a NACK to the device
void cm_N_Ack(void)
{
    CM_DATA_OUT;                         // Data line must be an output to send an NACK
    cm_Delay(1);
    CM_CLK_LO;
    cm_Delay(1);
    CM_DATA_HI;                          // High on data line indicates an NACK
    cm_Delay(2);
    CM_CLK_HI;
    cm_Delay(9);
    cm_Clocklow();
}
#endif

//     Read a byte from device, MSB
//
// Modified 7-21-04 to correctly control SDA
// 
uchar cm_Read(void)
{
	uchar i;
	uchar rByte = 0;
	
	CM_DATA_IN;                          // Set data line to be an input
	CM_DATA_HI;
	for(i=0x80; i; i=i>>1)
	{
		cm_ClockCycle();
		if (CM_DATA_RD) rByte |= i;
		cm_Clocklow();
	}
	CM_DATA_OUT;                         // Set data line to be an output
	return rByte;
}

void cm_WaitClock(uchar loop)
{
	uchar i, j;
	
	CM_DATA_LO;
	for(j=0; j<loop; j++) {
		cm_Start();
		for(i = 0; i<15; i++) cm_ClockCycle();
		cm_Stop();
	}
}

// Send a command
//
uchar cm_SendCommand(puchar pucInsBuff)
{
    uchar i, ucCmd;
	
    i = CM_START_TRIES;
    ucCmd = (pucInsBuff[0]&0x0F)|CM_PORT_CFG.ucChipSelect;
    while (i) {
        cm_Start();
        if (cm_Write(ucCmd) == 0) break;
        if (--i == 0) return FAIL_CMDSTART;
    }
	
    for(i = 1; i< 4; i++) {
        if (cm_Write(pucInsBuff[i]) != 0) return FAIL_CMDSEND;
    }
    return SUCCESS;
}

uchar cm_ReceiveData(puchar pucRecBuf, uchar ucLen)
{
    int i;

    for(i = 0; i < (ucLen-1); i++) {
        pucRecBuf[i] = cm_Read();
        cm_AckNak(TRUE);
    }
    pucRecBuf[i] = cm_Read();
    cm_AckNak(FALSE);
    cm_Stop();
    return SUCCESS;
}

uchar cm_SendData(puchar pucSendBuf, uchar ucLen)
{
	int i;
	for(i = 0; i< ucLen; i++) {
		if (cm_Write(pucSendBuf[i])==1) return FAIL_WRDATA;
	}
	cm_Stop();
	
	return SUCCESS;
}

// Functions in CM_I2C.C used internally by other low level functions
void cm_RandomGen(puchar pucRanddat)
{

}

// -------------------------------------------------------------------------------------------------
// Low Level Function Prototypes
// -------------------------------------------------------------------------------------------------

// Placeholder function that always returns TRUE
uchar cm_TRUE(void)
{
    return TRUE;
}

// Placeholder function that always returns SUCCESS
uchar cm_SUCCESS(void)
{
    return SUCCESS;
}

// Function that detects if the card is present
// Card Detect
// add 2010-05-10 不用检测
uchar cm_CardDetect(void)
{
//     CM_DETECT_IN;                                       // Make detect pin an input
//     if (CM_DETECT_RD) return CM_DETECT_POL?TRUE:FALSE;  // Adjust pin HI for polarity
//     return CM_DETECT_POL?FALSE:TRUE;                    // Adjust pin LO for polarity
    return TRUE;	
}

// Functions that directly control the hardware
// Power On Functions
// Power On Chip  
//
// Returns 0 (SUCCESS) if no error
//
void cm_FullPowerOn(void)
{
	// Reset chip data
	cm_ResetCrypto();
    ucCM_UserZone = ucCM_AntiTearing = 0;
    
	// Power control
    //add 2010-05-10 不需要上电控制
//	CM_POWER_OUT;                           // Set power pin to be an output
	
	//add 2010-05-10 不需要上电控制
	//if (CM_POWER_POL) CM_POWER_LO; else CM_POWER_HI;   // Turn OFF power
    CM_DIR_INIT;                              // SDA, SCL both start as outputs
    CM_CLK_LO;                                // Clock should start LOW
    CM_DATA_HI;
	//add 2010-05-10 不需要上电控制                               // Data high during reset
	//if (CM_POWER_POL) CM_POWER_HI; else CM_POWER_LO;   // Turn ON power
    cm_Delay(100);                           // Give chip a chance stabilize after power is applied
	  
    // Sequence for powering on secure memory according to ATMEL spec
    cm_ClockCycles(CM_PWRON_CLKS);           // Give chip some clocks cycles to get started

    // Chip should now be in sync mode and ready to operate
}
// Functions control the logical power on/off for the chip
// Power On Chip  
//
// Returns 0 (SUCCESS) if no error
//
void cm_PowerOn(void)
{
	// Reset chip data
	cm_ResetCrypto();
    ucCM_UserZone = ucCM_AntiTearing = 0;
    
    // Sequence for powering on secure memory according to ATMEL spec
    CM_DATA_OUT;                              // SDA and SCL start as outputs
	CM_CLK_OUT;
    CM_CLK_HI;//CM_CLK_LO;                                // Clock should start LOW
    CM_DATA_HI;                               // Data high during reset
    cm_ClockCycles(CM_PWRON_CLKS);            // Give chip some clocks cycles to get started

    // Chip should now be in sync mode and ready to operate
}   

// Power Off Functions
// Shut down secure memory
//
void cm_FullPowerOff(void)
{
	cm_Delay(1);
	CM_CLK_LO;
	cm_Delay(6);
	
	//add 2010-05-10 不需要上电控制 
	//if (CM_POWER_POL) CM_POWER_LO; else CM_POWER_HI;   // Turn OFF power
}
// Shut down secure memory
//
void cm_PowerOff(void)
{
	cm_Delay(1);
	CM_CLK_LO;
	cm_Delay(6);
}

// Low quality random number generator
// Random
void cm_RandGen(puchar pucRandomData)
{
	uchar i;
	
//	srand(2);                      // need to introduce a source of entrophy
	for(i = 0; i < 8; i++) pucRandomData[i] = (uchar)rand();
}

// Send a command byte
//
uchar cm_SendCmdByte(uchar ucCommand)
{
    uchar i, ucCmd;
	
    i = CM_START_TRIES;

    ucCmd = (ucCommand&0x0F)|CM_PORT_CFG.ucChipSelect;
    while (i) {
      cm_Start();
	    if (cm_Write(ucCmd) == 0) break;
	    if (--i == 0) return FAIL_CMDSTART;
    }

    return SUCCESS;
}

// Mid-Level Functions
// Mid Level Utility Function: cm_ReadCommand()
//
// Note: this module must be after all low level functions in the library and
//       before all high level user function to assure that any reference to
//       this function in this library are satistified.
uchar cm_ReadCommand(puchar pucInsBuff, puchar pucRetVal, uchar ucLen)
{
    uchar ucReturn;
    
    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
    return CM_LOW_LEVEL.ReceiveRet(pucRetVal, ucLen);
}
// Mid Level Utility Function: cm_WriteCommand()
//
// Note: this module must be after all low level functions in the library and
//       before all high level user function to assure that any reference to
//       this function in this library are satistified.
uchar cm_WriteCommand(puchar pucInsBuff, puchar pucSendVal, uchar ucLen)
{
    uchar ucReturn;

    if ((ucReturn = CM_LOW_LEVEL.SendCommand(pucInsBuff)) != SUCCESS) return ucReturn;
    return CM_LOW_LEVEL.SendData(pucSendVal, ucLen);
}

