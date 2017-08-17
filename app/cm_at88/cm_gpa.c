// Encryption Functions
//
// Note: the naming conventions in this module do not match those used in all other modules. This
//       is because the name used in this module are intended to be as close to those used in the
//       Atmel documentation to make verification of these functions simpler.

//add 2010-05-10
//#include "..\app\includes.h"

#include "cm_gpa.h"

// -------------------------------------------------------------------------------------------------
// Data
// -------------------------------------------------------------------------------------------------

uchar ucGpaRegisters[Gpa_Regs];

// -------------------------------------------------------------------------------------------------
// Functions
// -------------------------------------------------------------------------------------------------

// Reset the cryptographic state
void cm_ResetCrypto(void)
{
    uchar i;
    
    for (i = 0; i < Gpa_Regs; ++i) ucGpaRegisters[i] = 0;
    ucCM_Encrypt = ucCM_Authenticate = FALSE;
}

// Generate next value
uchar cm_GPAGen(uchar Datain)
{
	uchar Din_gpa;
	uchar Ri, Si, Ti;
	uchar R_sum, S_sum, T_sum;
	
	// Input Character
	Din_gpa = Datain^Gpa_byte;
	Ri = Din_gpa&0x1f;   			                //Ri[4:0] = Din_gpa[4:0]
	Si = ((Din_gpa<<3)&0x78)|((Din_gpa>>5)&0x07);   //Si[6:0] = {Din_gpa[3:0], Din_gpa[7:5]}
	Ti = (Din_gpa>>3)&0x1f;  		                //Ti[4:0] = Din_gpa[7:3];
       
	//R polynomial
	R_sum = cm_Mod(RD, cm_RotR(RG), CM_MOD_R);
	RG = RF;
	RF = RE;
	RE = RD;
	RD = RC^Ri;
	RC = RB;
	RB = RA;
	RA = R_sum;
	
	//S ploynomial
	S_sum = cm_Mod(SF, cm_RotS(SG), CM_MOD_S);
	SG = SF;
	SF = SE^Si;
	SE = SD;
	SD = SC;
	SC = SB;
	SB = SA;
	SA = S_sum;
	
	//T polynomial
	T_sum = cm_Mod(TE,TC,CM_MOD_T);
	TE = TD;
	TD = TC;
	TC = TB^Ti;
	TB = TA;
	TA = T_sum;

    // Output Stage
    Gpa_byte =(Gpa_byte<<4)&0xF0;                                  // shift gpa_byte left by 4
    Gpa_byte |= ((((RA^RE)&0x1F)&(~SA))|(((TA^TD)&0x1F)&SA))&0x0F; // concat 4 prev bits and 4 new bits
	return Gpa_byte;
}

// Do authenticate/encrypt chalange encryption
void cm_AuthenEncryptCal(uchar *Ci, uchar *G_Sk, uchar *Q, uchar *Ch)
{	
    uchar i, j;
    
    // Reset all registers
    cm_ResetCrypto();
    
    // Setup the cyptographic registers
    for(j = 0; j < 4; j++) {
	    for(i = 0; i<3; i++) cm_GPAGen(Ci[2*j]);	
	    for(i = 0; i<3; i++) cm_GPAGen(Ci[2*j+1]);
	    cm_GPAGen(Q[j]);
    }
	
    for(j = 0; j<4; j++ ) {
	    for(i = 0; i<3; i++) cm_GPAGen(G_Sk[2*j]);
	    for(i = 0; i<3; i++) cm_GPAGen(G_Sk[2*j+1]);
	    cm_GPAGen(Q[j+4]);
    }
	
    // begin to generate Ch
    cm_GPAGenN(6);                    // 6 0x00s
    Ch[0] = Gpa_byte;

    for (j = 1; j<8; j++) {
	    cm_GPAGenN(7);                // 7 0x00s
	    Ch[j] = Gpa_byte;
    }

    // then calculate new Ci and Sk, to compare with the new Ci and Sk read from eeprom
    Ci[0] = 0xff;		              // reset AAC 
    for(j = 1; j<8; j++) {
	    cm_GPAGenN(2);                // 2 0x00s
	      Ci[j] = Gpa_byte;
    }

    for(j = 0; j<8; j++) {
	     cm_GPAGenN(2);                // 2 0x00s
	     G_Sk[j] = Gpa_byte;
    }
   
	cm_GPAGenN(3);                    // 3 0x00s
}

// Calaculate Checksum
void cm_CalChecksum(uchar *Ck_sum)
{
	cm_GPAGenN(15);                    // 15 0x00s
	Ck_sum[0] = Gpa_byte;
	cm_GPAGenN(5);                     // 5 0x00s
	Ck_sum[1] = Gpa_byte;	
}

// The following functions are "macros" for commonly done actions

// Clock some zeros into the state machine
void cm_GPAGenN(uchar Count)
{
    while(Count--) cm_GPAGen(0x00);
}

// Clock some zeros into the state machine, then clock in a byte of data
void cm_GPAGenNF(uchar Count, uchar DataIn)
{
    cm_GPAGenN(Count);                             // First ones are allways zeros
    cm_GPAGen(DataIn);                             // Final one is sometimes different
}

// Include 2 bytes of a command into a polynominal
void cm_GPAcmd2(puchar pucInsBuff)
{
	  cm_GPAGenNF(5, pucInsBuff[2]);
	  cm_GPAGenNF(5, pucInsBuff[3]);
}
    
// Include 3 bytes of a command into a polynominal
void cm_GPAcmd3(puchar pucInsBuff)
{
	  cm_GPAGenNF(5, pucInsBuff[1]);
	  cm_GPAcmd2(pucInsBuff);
}
    
// Include the data in the polynominals and decrypt it required
void cm_GPAdecrypt(uchar ucEncrypt, puchar pucBuffer, uchar ucCount)
{
	  uchar i;
	   
	  for (i = 0; i < ucCount; ++i) {
          if (ucEncrypt) pucBuffer[i] = pucBuffer[i]^Gpa_byte;
 	      cm_GPAGen(pucBuffer[i]);
          cm_GPAGenN(5);        // 5 clocks with 0x00 data
    }
}

// Include the data in the polynominals and encrypt it required
void cm_GPAencrypt(uchar ucEncrypt, puchar pucBuffer, uchar ucCount)
{
    uchar i, ucData; 

  	for (i = 0; i<ucCount; i++) {
  		cm_GPAGenN(5);                          // 5 0x00s
  		ucData = pucBuffer[i];
  		if (ucEncrypt) pucBuffer[i] = pucBuffer[i]^Gpa_byte;
  		cm_GPAGen(ucData);
  	}
}


