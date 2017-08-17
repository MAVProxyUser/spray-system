/******************************************************************************* 
*                AES Decrypt/Encrypt algorithm functions library               * 
*   file : aes.c                                                               *
*                                                                              *
*   history :                                                                  * 
*     v1.0 2010-07-26   soure from ourdev                                      *
*******************************************************************************/
#ifndef __AES_H__
#define __AES_H__

void aesDecInit(void);
//void aesDecrypt( unsigned char * buffer, unsigned char * chainBlock );
void aesDecryptBlock(unsigned char *buffer, unsigned int nSize);
//add 2010-10-19
void aesDecryptAt88(unsigned char *buffer);


//void aesEncInit();
//void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock );
//void aesEncryptBlock(unsigned char *buffer,unsigned int nSize);
//add 2010-10-19
//void aesEncryptAt88(unsigned char *buffer);
#endif

/*
********************************************************************************
*                        END
********************************************************************************
*/

