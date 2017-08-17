// CryptoMemory Encryption Header File
//
// This library is used in all of the low level hardware interface for I2C communications.
//
// Revision Date      Changes
// -------- --------- -----------------------------------------------------------------
//    0.1   07 Nov 04 First Release
//    0.2   18 Nov 04 Major revision to fix bugs and make CM_GPA.C more readable

// Protect Library against multiple inclusion
#ifndef CM_GPA_H
#define CM_GPA_H

// -------------------------------------------------------------------------------------------------
// Other includes required by this header file
// -------------------------------------------------------------------------------------------------

#include "cm_lib.h"

// Macros for all of the registers
#define RA       (ucGpaRegisters[0])
#define RB       (ucGpaRegisters[1])
#define RC       (ucGpaRegisters[2])
#define RD       (ucGpaRegisters[3])
#define RE       (ucGpaRegisters[4])
#define RF       (ucGpaRegisters[5])
#define RG       (ucGpaRegisters[6])
#define TA       (ucGpaRegisters[7])
#define TB       (ucGpaRegisters[8])
#define TC       (ucGpaRegisters[9])
#define TD       (ucGpaRegisters[10])
#define TE       (ucGpaRegisters[11])
#define SA       (ucGpaRegisters[12])
#define SB       (ucGpaRegisters[13])
#define SC       (ucGpaRegisters[14])
#define SD       (ucGpaRegisters[15])
#define SE       (ucGpaRegisters[16])
#define SF       (ucGpaRegisters[17])
#define SG       (ucGpaRegisters[18])
#define Gpa_byte (ucGpaRegisters[19])
#define Gpa_Regs (20)


// Defines for constants used
#define CM_MOD_R (0x1F)
#define CM_MOD_T (0x1F)
#define CM_MOD_S (0x7F)

// Macros for common operations
#define cm_Mod(x,y,m) ((x+y)>m?(x+y-m):(x+y))
#define cm_RotT(x)    (((x<<1)&0x1e)|((x>>4)&0x01))
#define cm_RotR(x)    (((x<<1)&0x1e)|((x>>4)&0x01))
#define cm_RotS(x)    (((x<<1)&0x7e)|((x>>6)&0x01))

// Externals
extern uchar ucGpaRegisters[Gpa_Regs];

// Function Prototypes
void cm_ResetCrypto(void);
uchar cm_GPAGen(uchar Datain);
void cm_CalChecksum(uchar *Ck_sum);
void cm_AuthenEncryptCal(uchar *Ci, uchar *G_Sk, uchar *Q, uchar *Ch);
void cm_GPAGenN(uchar Count);
void cm_GPAGenNF(uchar Count, uchar DataIn);
void cm_GPAcmd2(puchar pucInsBuff);
void cm_GPAcmd3(puchar pucInsBuff);
void cm_GPAdecrypt(uchar ucEncrypt, puchar pucBuffer, uchar ucCount);
void cm_GPAencrypt(uchar ucEncrypt, puchar pucBuffer, uchar ucCount); 

// end of multiple inclusion protection
#endif
