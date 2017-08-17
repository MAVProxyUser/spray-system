//modify 2010-05-10
#include <lpc17xx.h>

//
#include "cm_lib.h"
#include "cm_i2c.h"
#include "cm_gpa.h"

// -------------------------------------------------------------------------------------------------
// Data
// -------------------------------------------------------------------------------------------------

uchar ucCM_DevGpaRegisters[16][Gpa_Regs];
uchar ucCM_DevEncrypt[16];
uchar ucCM_DevAuthenticate[16];
uchar ucCM_DevUserZone[16];
uchar ucCM_DevAntiTearing[16];

// High Level Function Prototypes

// Select Chip
uchar cm_SelectChip(uchar ucChipId)
{
	uchar ucChip, i;
	  
    if (ucChipId == 0xFF) {
    	// Clear All State
    	for (ucChip = 0; ucChip < 16; ++ucChip) {
    	    for (i = 0; i < 16; ++i) ucCM_DevGpaRegisters[ucChip][i] = 0;
            ucCM_DevEncrypt[ucChip] = 0;
            ucCM_DevAuthenticate[ucChip] = 0;
            ucCM_DevUserZone[ucChip] = 0;
            ucCM_DevAntiTearing[ucChip] = 0;
        }
    }
    else {
    	ucChip = CM_PORT_CFG.ucChipSelect;
    	if (ucChip != ucChipId) {
		    // Set Chip Select
            CM_PORT_CFG.ucChipSelect = ucChipId;
			
			// Shift Addresses
    	    ucChip = (ucChip>>4)&0xF0;
    	    ucChipId = (ucChipId>>4)&0xF0;
			
    	    // Swap GPA Registers
    	    for (i = 0; i < 16; ++i) {
			    ucCM_DevGpaRegisters[ucChip][i] = ucGpaRegisters[i];
				ucGpaRegisters[i] = ucCM_DevGpaRegisters[ucChipId][i];
			}
			
			//Save State 
            ucCM_DevEncrypt[ucChip] = ucCM_Encrypt;
            ucCM_DevAuthenticate[ucChip] = ucCM_Authenticate;
            ucCM_DevUserZone[ucChip] = ucCM_UserZone;
            ucCM_DevAntiTearing[ucChip] = ucCM_AntiTearing;
			
            // Restore Saved State
    	    ucCM_Encrypt = ucCM_DevEncrypt[ucChipId];
            ucCM_Authenticate = ucCM_DevAuthenticate[ucChipId];
            ucCM_UserZone = ucCM_DevUserZone[ucChipId];
            ucCM_AntiTearing = ucCM_DevAntiTearing[ucChipId];
        }
    }
	
	return SUCCESS;
}

// Global Data
uchar ucCM_Ci[8], ucCM_G_Sk[8];
uchar ucCM_Q_Ch[16], ucCM_Ci2[8];
// Local function prototypes
// Common code for both activating authentication and encryption
static uchar cm_AuthenEncrypt(uchar ucCmd1, uchar ucAddrCi, puchar pucCi, puchar pucG_Sk)
{
    uchar i;
	uchar ucReturn;
    
    // Generate chalange data
    //if (pucRandom) for (i = 0; i < 8; ++i) ucCM_Q_Ch[i] = pucRandom[i];
    //else           CM_LOW_LEVEL.RandomGen(ucCM_Q_Ch);

    for (i = 0; i < 8; ++i) ucCM_Q_Ch[i] = 0;
    cm_AuthenEncryptCal(pucCi, pucG_Sk, ucCM_Q_Ch, &ucCM_Q_Ch[8]);
    
    // Send chalange
    ucCM_InsBuff[0] = 0xb8;
    ucCM_InsBuff[1] = ucCmd1;
    ucCM_InsBuff[2] = 0x00;
    ucCM_InsBuff[3] = 0x10;
    if ((ucReturn = cm_WriteCommand(ucCM_InsBuff, ucCM_Q_Ch, 16)) != SUCCESS) return ucReturn;
                    
    // Give chips some clocks to do calculations
    CM_LOW_LEVEL.WaitClock(3);
                             
    // Verify result
    if ((ucReturn = cm_ReadConfigZone(ucAddrCi, ucCM_Ci2, 8)) != SUCCESS) return ucReturn;
    for(i=0; i<8; i++) if (pucCi[i]!=ucCM_Ci2[i]) return FAILED;
    
    // Done
    return SUCCESS;
}

// Activate Security
//
// When called the function:
// ∑	reads the current cryptogram (Ci) of the key set, 
// ∑	computes the next cryptogram (Ci+1) based on the secret key pucKey (GCi) and the random number selected,
// ∑	sends the (Ci+1) and the random number to the CryptoMemory© device, 
// ∑	computes (Ci+2) and compares its computed value the new cryptogram of the key set.
// ∑	If (Ci+2) matches the new cryptogram of the key set, authentication was successful.
// In addition, if ucEncrypt is TRUE the function:
// ∑	computes the new session key (Ci+3) and a challenge, 
// ∑	sends the new session key and the challenge to the CryptoMemory© device, 
// ∑	If the new session key and the challenge are correctly related, encryption is activated.
//                              1,             ucG,            FALSE
uchar cm_ActiveSecurity(uchar ucKeySet, puchar pucKey, uchar ucEncrypt)
{
    uchar i;
    uchar ucAddrCi;
	uchar ucReturn;
    
    // Read Ci for selected key set
    ucAddrCi = CM_Ci + (ucKeySet<<4);              // Ci blocks on 16 byte boundries
    if ((ucReturn = cm_ReadConfigZone(ucAddrCi, ucCM_Ci, 8)) != SUCCESS) return ucReturn;
    
    // Try to activate authentication
    for (i = 0; i < 8; ++i) ucCM_G_Sk[i] = pucKey[i];
    if ((ucReturn = cm_AuthenEncrypt(ucKeySet, ucAddrCi, ucCM_Ci, ucCM_G_Sk)) != SUCCESS) return ucReturn;
    ucCM_Authenticate = TRUE;
        
    // If Encryption required, try to activate that too
    if (ucEncrypt) {
        //if (pucRandom) pucRandom += 8;
        if ((ucReturn = cm_AuthenEncrypt(ucKeySet+0x10, ucAddrCi, ucCM_Ci, ucCM_G_Sk)) != SUCCESS) return ucReturn;
        ucCM_Encrypt = TRUE;
    }
    
    // Done
    return SUCCESS;
}

// Deactivate Security
uchar cm_DeactiveSecurity(void)
{
	uchar ucReturn;
	 
	if ((ucReturn = CM_LOW_LEVEL.SendCmdByte(0xb8)) != SUCCESS) return ucReturn;
	cm_ResetCrypto();
	
	return SUCCESS;
}

uchar ucCmdPassword[4] = {0xba, 0x00, 0x00, 0x03};
uchar ucPSW[3];

// Verify Password
//                       CM_PW,            7,          CM_PWWRITE)
uchar cm_VerifyPassword(puchar pucPassword, uchar ucSet, uchar ucRW)
{
	uchar i, j;
    uchar ucReturn;
	uchar ucAddr;
	
	// Build command and PAC address
    ucAddr = CM_PSW + (ucSet<<3);
	ucCmdPassword[1] = ucSet;
	if (ucRW != CM_PWWRITE) {
	  	ucCmdPassword[1] |= 0x10;
	  	ucAddr += 4;
	}
	  
	// Deal with encryption if in authenticate mode
	for (j = 0; j<3; j++) {
	    // Encrypt the password
	    if(ucCM_Authenticate) {
    	    for(i = 0; i<5; i++) cm_GPAGen(pucPassword[j]);
    		ucPSW[j] = Gpa_byte;
	  	}
	    // Else just copy it
		ucPSW[j] = pucPassword[j];
    }
	  
	// Send the command
	ucReturn = cm_WriteCommand(ucCmdPassword, ucPSW, 3);
     
    // Wait for chip to process password
    CM_LOW_LEVEL.WaitClock(3);
   
    // Read Password attempts counter to determine if the password was accepted
    if (ucReturn == SUCCESS) {
        ucReturn = cm_ReadConfigZone(ucAddr, ucPSW, 1);
	    if (ucPSW[0]!= 0xFF) ucReturn = FAILED;
	}
	if (ucCM_Authenticate && (ucReturn != SUCCESS)) cm_ResetCrypto();

    // Done
    return ucReturn;
}

// Reset Password
uchar cm_ResetPassword(void)
{
    return CM_LOW_LEVEL.SendCmdByte(0xba);
}

// Verify Secure Code
#define cm_VerifySecureCode(CM_PW) cm_VerifyPassword(CM_PW, 7, CM_PWWRITE)

// Read Configuration Zone
uchar cm_ReadConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
    uchar ucReturn, ucEncrypt;

    ucCM_InsBuff[0] = 0xb6;
    ucCM_InsBuff[1] = 0x00;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;
    
    // Three bytes of the command must be included in the polynominals
    cm_GPAcmd2(ucCM_InsBuff);
    
    // Do the read
    if ((ucReturn = cm_ReadCommand(ucCM_InsBuff, pucBuffer, ucCount)) != SUCCESS) return ucReturn;
	
    // Only password zone is ever encrypted
    ucEncrypt = ((ucCryptoAddr>= CM_PSW) && ucCM_Encrypt);

    // Include the data in the polynominals and decrypt if required
    cm_GPAdecrypt(ucEncrypt, pucBuffer, ucCount); 

    // Done
    return SUCCESS;
}

// Write Configuration Zone     0xB4 0x00 Addr N
uchar cm_WriteConfigZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount, uchar ucAntiTearing)
{
    uchar ucReturn, ucEncrypt;
	
    ucCM_InsBuff[0] = 0xb4;
    if(ucAntiTearing) ucCM_InsBuff[1] = 0x08;
    else              ucCM_InsBuff[1] = 0x00;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;

    // Three bytes of the command must be included in the polynominals
    cm_GPAcmd2(ucCM_InsBuff);
    
    // Only password zone is ever encrypted
    ucEncrypt = ((ucCryptoAddr>= CM_PSW) && ucCM_Encrypt);

    // Include the data in the polynominals and encrypt if required
    cm_GPAencrypt(ucEncrypt, pucBuffer, ucCount); 

    // Do the write
    ucReturn = cm_WriteCommand(ucCM_InsBuff, pucBuffer,ucCount);

    // when anti-tearing, the host should send ACK should >= 20ms after write command
    if (ucAntiTearing) CM_LOW_LEVEL.WaitClock(10);

    return ucReturn;
}

// Set User Zone
uchar cm_SetUserZone(uchar ucZoneNumber, uchar ucAntiTearing)
{
	uchar ucReturn;
	uchar p[8];
	
	ucCM_InsBuff[0] = 0xb4;
	if (ucAntiTearing) ucCM_InsBuff[1] = 0x0b;
	else 	           ucCM_InsBuff[1] = 0x03;
    ucCM_InsBuff[2] = ucZoneNumber;
	ucCM_InsBuff[3] = 0x00;

    // Only zone number is included in the polynomial
	cm_GPAGen(ucZoneNumber);
	
	if ((ucReturn = CM_LOW_LEVEL.SendCommand(ucCM_InsBuff))!= SUCCESS) return ucReturn;
	
	// save zone number and anti-tearing state
    ucCM_UserZone = ucZoneNumber;
    ucCM_AntiTearing = ucAntiTearing;

    // done	
	return  CM_LOW_LEVEL.ReceiveRet(p,0);
}

// Read User Zone
// The Read Large User Zone function is used to read data from CryptoMemory devices
// that have greater than 256 bytes in each user zone (AT88SC6416C, and larger)
uchar cm_ReadLargeZone(uint uiCryptoAddr, puchar pucBuffer, uchar ucCount)
{
    uchar ucReturn;

    ucCM_InsBuff[0] = 0xb2;
    ucCM_InsBuff[1] = (uchar)(uiCryptoAddr>>8);
    ucCM_InsBuff[2] = (uchar)uiCryptoAddr;
    ucCM_InsBuff[3] = ucCount;

    // Three bytes of the command must be included in the polynominals
    cm_GPAcmd3(ucCM_InsBuff);
    
    // Read the data
    if ((ucReturn = cm_ReadCommand(ucCM_InsBuff, pucBuffer, ucCount)) != SUCCESS) return ucReturn;
    
    // Include the data in the polynominals and decrypt if required
    cm_GPAdecrypt(ucCM_Encrypt, pucBuffer, ucCount); 

    return SUCCESS;
}

// Read Small User Zone     0xb2  addr  addr  N  Ndata
uchar cm_ReadSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;

    ucCM_InsBuff[0] = 0xb2;
    ucCM_InsBuff[1] = 0;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;

    // Two bytes of the command must be included in the polynominals
    cm_GPAcmd2(ucCM_InsBuff);
    
    // Read the data
    if ((ucReturn = cm_ReadCommand(ucCM_InsBuff, pucBuffer, ucCount)) != SUCCESS) return ucReturn;
	
    // Include the data in the polynominals and decrypt it required
    cm_GPAdecrypt(ucCM_Encrypt, pucBuffer, ucCount); 

    // Done
    return SUCCESS;
}

// Write User Zone
char cm_WriteLargeZone(uint uiCryptoAddr, puchar pucBuffer, uchar ucCount)
{
    uchar ucReturn;
	
    ucCM_InsBuff[0] = 0xb0;
    ucCM_InsBuff[1] = (uchar)(uiCryptoAddr>>8);
    ucCM_InsBuff[2] = (uchar)uiCryptoAddr;
    ucCM_InsBuff[3] = ucCount;

    // Three bytes of the command must be included in the polynominals
    cm_GPAcmd3(ucCM_InsBuff);
    
    // Include the data in the polynominals and encrypt it required
    cm_GPAencrypt(ucCM_Encrypt, pucBuffer, ucCount); 

    ucReturn = cm_WriteCommand(ucCM_InsBuff, pucBuffer, ucCount);

	// when anti-tearing, the host should send ACK should >= 20ms after write command
	if (ucCM_AntiTearing) CM_LOW_LEVEL.WaitClock(10);

	// Done
	return ucReturn;
}

// Write Small User Zone
uchar cm_WriteSmallZone(uchar ucCryptoAddr, puchar pucBuffer, uchar ucCount)
{
	uchar ucReturn;
	
	ucCM_InsBuff[0] = 0xb0;
    ucCM_InsBuff[1] = 0x00;
    ucCM_InsBuff[2] = ucCryptoAddr;
    ucCM_InsBuff[3] = ucCount;

    // Two bytes of the command must be included in the polynominals
    cm_GPAcmd2(ucCM_InsBuff);
    
    // Include the data in the polynominals and encrypt it required
    cm_GPAencrypt(ucCM_Encrypt, pucBuffer, ucCount); 

	// Write the data
	ucReturn = cm_WriteCommand(ucCM_InsBuff, pucBuffer,ucCount);

	//VCOM_printf(" ucReturn = %d",ucReturn);
	// when anti-tearing, the host should send ACK should >= 20ms after write command
	if (ucCM_AntiTearing) CM_LOW_LEVEL.WaitClock(10);

	return ucReturn;
}

uchar	ucCmdWrChk[4] = {0xb4, 0x02, 0x00, 0x02};

// Send Checksum
uchar cm_SendChecksum(void)
{
	uchar ucReturn;
	uchar ucChkSum[2];

    // Get Checksum if required
	cm_CalChecksum(ucChkSum);

	// Send the command
	ucReturn = cm_WriteCommand(ucCmdWrChk, ucChkSum, 2);

    // Give the CyrptoMemory some processing time
    CM_LOW_LEVEL.WaitClock(5);
	
	// Done
    return ucReturn;
}

uchar ucCmdRdChk[4] = {0xb6, 0x02, 0x00, 0x02};

// Read Checksum
uchar cm_ReadChecksum(puchar pucChkSum)
{
    uchar ucDCR[1];
	uchar ucReturn;

    // 20 0x00s (10 0x00s, ignore first byte, 5 0x00s, ignore second byte, 5 0x00s  
    cm_GPAGenN(20);
	  
    // Read the checksum                  
    if((ucReturn = cm_ReadCommand(ucCmdRdChk, pucChkSum, 2))!= SUCCESS) return ucReturn;
	
    // Check if unlimited reads allowed
    if ((ucReturn = cm_ReadConfigZone(DCR_ADDR, ucDCR, 1)) != SUCCESS) return ucReturn;
    if ((ucDCR[0]&DCR_UCR)) cm_ResetCrypto();
	
    return SUCCESS;
}

uchar ucCmdRdFuze[4] = {0xb6, 0x01, 0x00, 0x01};

// Read Fuse Byte
uchar cm_ReadFuse(puchar pucFuze)
{
	uchar ucReturn;
	
  // 5 0x00, A2 (0x00), 5 0x00, N (0x01)	
	cm_GPAGenNF(11, 0x01);

	if((ucReturn = cm_ReadCommand(ucCmdRdFuze,pucFuze,1)) != SUCCESS) return ucReturn;
	
	cm_GPAGen(*pucFuze);         // fuze byte
    cm_GPAGenN(5);               // 5 0x00s
  
	return SUCCESS;
}

uchar ucCmdWrFuze[4] = {0xb4, 0x01, 0x00, 0x00};
//fuse ID
//0x07 SEC ”…Atmel…’∂œÀ¸£¨À¸À¯∂®¡ÀLot History Code£¨Õ¨ ±‘ –Ì∏˘æ›Access Registers»∑∂®∑√Œ ”√ªß«¯µƒ»®œﬁ
//0x06 FAB ’‚∏ˆ»€ÀøÀ¯∂®¡À≈‰÷√«¯µƒAnswer To Reset∫ÕFab Code¡Ω≤ø∑÷
//0x04 CMA ’‚∏ˆ»€ÀøÀ¯∂®¡À≈‰÷√«¯µƒCard Manufacturer Code
//0x00 PER’‚∏ˆ»€ÀøÀ¯∂®¡À≈‰÷√«¯µƒ £”‡≤ø∑÷

// Burn Fuse
uchar cm_BurnFuse(uchar ucFuze)
{
	uchar ucReturn;
	uchar p[8];
	
  // Burn Fuze
	ucCmdWrFuze[2] = ucFuze;
	if((ucReturn = CM_LOW_LEVEL.SendCommand(ucCmdWrFuze))!= SUCCESS) return ucReturn;
	
  // done	
	return  CM_LOW_LEVEL.ReceiveRet(p,0);
}
