/******************************************************************************* 
*                AES Decrypt/Encrypt algorithm functions liboray               * 
*   file : aes.c                                                               *
*                                                                              *
*   history :                                                                  * 
*     v1.0 2010-07-26   soure from ourdev                                      *
*******************************************************************************/
#define BPOLY 0x1b   //!< Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
#define BLOCKSIZE 16 //!< Block size in number of bytes.

#define KEY_COUNT 1

#if KEY_COUNT == 1
  #define KEYBITS 128 //!< Use AES128.
#elif KEY_COUNT == 2
  #define KEYBITS 192 //!< Use AES196.
#elif KEY_COUNT == 3
  #define KEYBITS 256 //!< Use AES256.
#else
  #error Use 1, 2 or 3 keys!
#endif

#if KEYBITS == 128
  #define ROUNDS 10 //!< Number of rounds.
  #define KEYLENGTH 16 //!< Key length in number of bytes.
#elif KEYBITS == 192
  #define ROUNDS 12 //!< Number of rounds.
  #define KEYLENGTH 24 //!< // Key length in number of bytes.
#elif KEYBITS == 256
  #define ROUNDS 14 //!< Number of rounds.
  #define KEYLENGTH 32 //!< Key length in number of bytes.
#else
  #error Key must be 128, 192 or 256 bits!
#endif

/* 注意AES的密钥要在XP60中更改 */

//AES key,the same with wkm osd
const unsigned char kTable[32] =  
{
  	0x96, 0x70, 0x9a, 0xD3, 0x26, 0x67, 0x4A, 0xC3,
  	0x82, 0xB6, 0x69, 0x27, 0xE6, 0xd8, 0x84, 0x21,
  	0x25, 0xF8, 0xA8, 0x8c, 0x29, 0x2A, 0x15, 0x93,
  	0xD5, 0xA5, 0xA3, 0xB2, 0x7B, 0x91, 0x28, 0x67,
};

//AES key for at88	
const unsigned char kTableAt88[32] =	  
{
  	0x82, 0x31, 0x4E, 0x66, 0xE1, 0xD1, 0xF5, 0x13,
  	0xB6, 0x53, 0xd2, 0xC6, 0x93, 0x7F, 0x39, 0x72,
  	0xC1, 0xa8, 0x3f, 0x8c, 0x29, 0x55, 0x15, 0xC6,
  	0x9B, 0xb3, 0x66, 0x28, 0x0a, 0x26, 0xe1, 0x2F,
};

unsigned char block1[256]; //!< Workspace 1.
unsigned char block2[256]; //!< Worksapce 2.
unsigned char tempbuf[256];
unsigned char chainBlock[16];

unsigned char *powTbl;  //!< Final location of exponentiation lookup table.
unsigned char *logTbl;  //!< Final location of logarithm lookup table.
unsigned char *sBox;    //!< Final location of s-box.
unsigned char *sBoxInv; //!< Final location of inverse s-box.
unsigned char *expandedKey; //!< Final location of expanded key.

void CalcPowLog(unsigned char *powTbl, unsigned char *logTbl)
{
  	unsigned char i = 0;
  	unsigned char t = 1;

  	do {
    	// Use 0x03 as root for exponentiation and logarithms.
    	powTbl[i] = t;
    	logTbl[t] = i;
    	i++;

    	// Muliply t by 3 in GF(2^8).
    	t ^= (t << 1) ^ (t & 0x80 ? BPOLY : 0);
  	}while( t != 1 ); // Cyclic properties ensure that i < 255.

  	powTbl[255] = powTbl[0]; // 255 = '-0', 254 = -1, etc.
}

//获得sBOX表
void CalcSBox( unsigned char * sBox )
{
  	unsigned char i, rot;
  	unsigned char temp;
  	unsigned char result;

  	// Fill all entries of sBox[].
  	i = 0;
  	do {
    	//Inverse in GF(2^8).
    	if( i > 0 ) 
    	{
      		temp = powTbl[ 255 - logTbl[i] ];
    	} 
    	else 
    	{
      		temp = 0;
    	}

    	// Affine transformation in GF(2).
    	result = temp ^ 0x63; // Start with adding a vector in GF(2).
    	for( rot = 4; rot > 0; rot-- )
    	{
      		// Rotate left.
      		temp = (temp<<1) | (temp>>7);

      		// Add rotated byte in GF(2).
      		result ^= temp;
    	}

    	// Put result in table.
    	sBox[i] = result;
  	} while( ++i != 0 );
}

void CalcSBoxInv( unsigned char * sBox, unsigned char * sBoxInv )
{
  	unsigned char i = 0;
  	unsigned char j = 0;

  	// Iterate through all elements in sBoxInv using  i.
  	do {
	// Search through sBox using j.
    	do {
      		// Check if current j is the inverse of current i.
      		if( sBox[ j ] == i )
      		{
        		// If so, set sBoxInc and indicate search finished.
        		sBoxInv[ i ] = j;
        		j = 255;
      		}
    	} while( ++j != 0 );
  	} while( ++i != 0 );
}



void CycleLeft( unsigned char * row )
{
  	// Cycle 4 bytes in an array left once.
  	unsigned char temp = row[0];

  	row[0] = row[1];
  	row[1] = row[2];
  	row[2] = row[3];
  	row[3] = temp;
}


void CalcCols(unsigned char *col)
{
  	unsigned char i;

  	for(i = 4; i > 0; i--)
  	{
    	*col = (*col << 1) ^ (*col & 0x80 ? BPOLY : 0);
    	col++;
  	}
}

void InvMixColumn( unsigned char * column )
{
  	unsigned char r[4];

  	r[0] = column[1] ^ column[2] ^ column[3];
  	r[1] = column[0] ^ column[2] ^ column[3];
  	r[2] = column[0] ^ column[1] ^ column[3];
  	r[3] = column[0] ^ column[1] ^ column[2];

  	/*column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
  	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
  	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
  	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);*/
  	CalcCols(column);

  	r[0] ^= column[0] ^ column[1];
  	r[1] ^= column[1] ^ column[2];
  	r[2] ^= column[2] ^ column[3];
  	r[3] ^= column[0] ^ column[3];

  	/*column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
  	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
  	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
  	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);*/
  	CalcCols(column);

  	r[0] ^= column[0] ^ column[2];
  	r[1] ^= column[1] ^ column[3];
  	r[2] ^= column[0] ^ column[2];
  	r[3] ^= column[1] ^ column[3];

  	/*column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
  	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
  	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
  	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);*/
  	CalcCols(column);

  	column[0] ^= column[1] ^ column[2] ^ column[3];
  	r[0] ^= column[0];
  	r[1] ^= column[0];
  	r[2] ^= column[0];
  	r[3] ^= column[0];
  
  	column[0] = r[0];
  	column[1] = r[1];
  	column[2] = r[2];
  	column[3] = r[3];

  	//CopyBytes(column, r, 4);
}

void SubBytes( unsigned char * bytes, unsigned char count )
{
	do {
	    *bytes = sBox[ *bytes ]; // Substitute every byte in state.
	    bytes++;
	} while( --count );
}

void InvSubBytesAndXOR( unsigned char * bytes, unsigned char * key, unsigned char count )
{
	do {
	    // *bytes = sBoxInv[ *bytes ] ^ *key; // Inverse substitute every byte in state and add key.
	    *bytes = block2[ *bytes ] ^ *key; // Use block2 directly. Increases speed.
	    bytes++;
	    key++;
	} while( --count );
}

void InvShiftRows( unsigned char * state )
{
  	unsigned char temp;

  	// Note: State is arranged column by column.

  	// Cycle second row right one time.
  	temp = state[ 1 + 3*4 ];
  	state[ 1 + 3*4 ] = state[ 1 + 2*4 ];
  	state[ 1 + 2*4 ] = state[ 1 + 1*4 ];
  	state[ 1 + 1*4 ] = state[ 1 + 0*4 ];
  	state[ 1 + 0*4 ] = temp;

  	// Cycle third row right two times.
  	temp = state[ 2 + 0*4 ];
  	state[ 2 + 0*4 ] = state[ 2 + 2*4 ];
  	state[ 2 + 2*4 ] = temp;
  	temp = state[ 2 + 1*4 ];
  	state[ 2 + 1*4 ] = state[ 2 + 3*4 ];
  	state[ 2 + 3*4 ] = temp;

  	// Cycle fourth row right three times, ie. left once.
  	temp = state[ 3 + 0*4 ];
  	state[ 3 + 0*4 ] = state[ 3 + 1*4 ];
  	state[ 3 + 1*4 ] = state[ 3 + 2*4 ];
  	state[ 3 + 2*4 ] = state[ 3 + 3*4 ];
  	state[ 3 + 3*4 ] = temp;
}

/*
void InvMixColumns( unsigned char * state )
{
  	InvMixColumn( state + 0*4 );
  	InvMixColumn( state + 1*4 );
  	InvMixColumn( state + 2*4 );
  	InvMixColumn( state + 3*4 );
}
*/

void XORBytes( unsigned char * bytes1, unsigned char * bytes2, unsigned char count )
{
	do {
	    *bytes1 ^= *bytes2; // Add in GF(2), ie. XOR.
	    bytes1++;
	    bytes2++;
	} while( --count );
}

void CopyBytes( unsigned char * to, unsigned char * from, unsigned char count )
{
	do {
	    *to = *from;
	    to++;
	    from++;
	} while( --count );
}

void KeyExpansion( unsigned char * expandedKey )
{
  	unsigned char temp[4];
  	unsigned char i;
  	unsigned char Rcon[4] = { 0x01, 0x00, 0x00, 0x00 }; // Round constant.

  	const unsigned char * key = kTable;

  	// Copy key to start of expanded key.
  	i = KEYLENGTH;
  	do {
    	*expandedKey = *key;
    	expandedKey++;
    	key++;
  	} while( --i );

  	// Prepare last 4 bytes of key in temp.
  	/*expandedKey -= 4;
  	temp[0] = *(expandedKey++);
  	temp[1] = *(expandedKey++);
  	temp[2] = *(expandedKey++);
  	temp[3] = *(expandedKey++);*/
  	CopyBytes(temp, expandedKey-4, 4);

  	// Expand key.
  	i = KEYLENGTH;
  	//j = BLOCKSIZE*(ROUNDS+1) - KEYLENGTH;
  	while( i < BLOCKSIZE*(ROUNDS+1) ) 
  	{
    	// Are we at the start of a multiple of the key size?
    	if( (i % KEYLENGTH) == 0 )
    	{
      		CycleLeft( temp ); // Cycle left once.
      		SubBytes( temp, 4 ); // Substitute each byte.
      		XORBytes( temp, Rcon, 4 ); // Add constant in GF(2).
      		*Rcon = (*Rcon << 1) ^ (*Rcon & 0x80 ? BPOLY : 0);
    	}

    	// Keysize larger than 24 bytes, ie. larger that 192 bits?
    	#if KEYLENGTH > 24
    	// Are we right past a block size?
    	else if( (i % KEYLENGTH) == BLOCKSIZE ) {
      		SubBytes( temp, 4 ); // Substitute each byte.
    	}
    	#endif

    	// Add bytes in GF(2) one KEYLENGTH away.
    	XORBytes( temp, expandedKey - KEYLENGTH, 4 );

    	// Copy result to current 4 bytes.
    	*(expandedKey++) = temp[ 0 ];
    	*(expandedKey++) = temp[ 1 ];
    	*(expandedKey++) = temp[ 2 ];
    	*(expandedKey++) = temp[ 3 ];
    	//CopyBytes(expandedKey, temp, 4);
    	//expandedKey += 4;

    	i += 4; // Next 4 bytes.
  	}
}

void KeyExpansionAt88( unsigned char * expandedKey )
{
  	unsigned char temp[4];
  	unsigned char i;
  	unsigned char Rcon[4] = { 0x01, 0x00, 0x00, 0x00 }; // Round constant.

  	const unsigned char * key = kTableAt88;

  	// Copy key to start of expanded key.
  	i = KEYLENGTH;
  	do {
    	*expandedKey = *key;
    	expandedKey++;
    	key++;
  	} while( --i );

  	// Prepare last 4 bytes of key in temp.
  	/*expandedKey -= 4;
  	temp[0] = *(expandedKey++);
  	temp[1] = *(expandedKey++);
  	temp[2] = *(expandedKey++);
  	temp[3] = *(expandedKey++);*/
  	CopyBytes(temp, expandedKey-4, 4);

  	// Expand key.
  	i = KEYLENGTH;
  	//j = BLOCKSIZE*(ROUNDS+1) - KEYLENGTH;
  	while( i < BLOCKSIZE*(ROUNDS+1) ) 
  	{
    	// Are we at the start of a multiple of the key size?
    	if( (i % KEYLENGTH) == 0 )
    	{
      		CycleLeft( temp ); // Cycle left once.
      		SubBytes( temp, 4 ); // Substitute each byte.
      		XORBytes( temp, Rcon, 4 ); // Add constant in GF(2).
      		*Rcon = (*Rcon << 1) ^ (*Rcon & 0x80 ? BPOLY : 0);
    	}

    	// Keysize larger than 24 bytes, ie. larger that 192 bits?
    	#if KEYLENGTH > 24
    	// Are we right past a block size?
    	else if( (i % KEYLENGTH) == BLOCKSIZE ) {
      		SubBytes( temp, 4 ); // Substitute each byte.
    	}
    	#endif

    	// Add bytes in GF(2) one KEYLENGTH away.
    	XORBytes( temp, expandedKey - KEYLENGTH, 4 );

    	// Copy result to current 4 bytes.
    	*(expandedKey++) = temp[ 0 ];
    	*(expandedKey++) = temp[ 1 ];
    	*(expandedKey++) = temp[ 2 ];
    	*(expandedKey++) = temp[ 3 ];
    	//CopyBytes(expandedKey, temp, 4);
    	//expandedKey += 4;

    	i += 4; // Next 4 bytes.
  	}
}

void InvCipher( unsigned char * block, unsigned char * expandedKey )
{
  	unsigned char i, j;
  	unsigned char round = ROUNDS-1;
  	expandedKey += BLOCKSIZE * ROUNDS;

  	XORBytes( block, expandedKey, 16 );
  	expandedKey -= BLOCKSIZE;

  	do {
    	InvShiftRows( block );
    	InvSubBytesAndXOR( block, expandedKey, 16 );
    	expandedKey -= BLOCKSIZE;
    	//InvMixColumns( block );
    	for(i = 4, j = 0; i > 0; i--, j+=4)
      	InvMixColumn( block + j );
  	} while( --round );

  	InvShiftRows( block );
  	InvSubBytesAndXOR( block, expandedKey, 16 );
}

void aesDecInit(void)
{
  	powTbl = block1;
  	logTbl = block2;
  	CalcPowLog( powTbl, logTbl );

  	sBox = tempbuf;
  	CalcSBox( sBox );

  	expandedKey = block1;
  	KeyExpansion( expandedKey );

  	sBoxInv = block2; // Must be block2.
  	CalcSBoxInv( sBox, sBoxInv );
}

void aesDecrypt( unsigned char * buffer, unsigned char * chainBlock )
{
  	CopyBytes( tempbuf, buffer, BLOCKSIZE );
  	InvCipher( buffer, expandedKey );
  	XORBytes( buffer, chainBlock, BLOCKSIZE );
  	CopyBytes( chainBlock, tempbuf, BLOCKSIZE );
}

//decrypt block
/*
** Note : nSize must be times of 16
*/
void aesDecryptBlock(unsigned char *buffer, unsigned int nSize)
{
  	unsigned int n;
	//add 2010-07-27
	for(n = 0; n < 16; n++)
		chainBlock[n] = 0;

  	for(n = nSize; n > 0; n -= 16)
  	{
    	CopyBytes( tempbuf, buffer, BLOCKSIZE );
    	InvCipher( buffer, expandedKey );
		// 在aes加密的基础上，增加了一异或
    	XORBytes( buffer, chainBlock, BLOCKSIZE );
    	CopyBytes( chainBlock, tempbuf, BLOCKSIZE );
    	buffer += 16;
  	}
}

//add 2010-10-19
void aesDecryptAt88(unsigned char *buffer)
{
  	unsigned int n;

	//add 2010-10-19
	powTbl = block1;
  	logTbl = block2;
  	CalcPowLog( powTbl, logTbl );

  	sBox = tempbuf;
  	CalcSBox( sBox );

  	expandedKey = block1;
  	KeyExpansionAt88( expandedKey );

  	sBoxInv = block2; // Must be block2.
  	CalcSBoxInv( sBox, sBoxInv );

	//add 2010-07-27
	for(n = 0; n < 16; n++)
		chainBlock[n] = 0;

    CopyBytes( tempbuf, buffer, BLOCKSIZE );
    InvCipher( buffer, expandedKey );
	// 在aes加密的基础上，增加了一异或
    XORBytes( buffer, chainBlock, BLOCKSIZE );
}

unsigned char Multiply( unsigned char num, unsigned char factor )
{
  	unsigned char mask = 1;
  	unsigned char result = 0;

  	while( mask != 0 ) 
  	{
    	// Check bit of factor given by mask.
    	if( mask & factor ) 
    	{
      		// Add current multiple of num in GF(2).
      		result ^= num;
    	}

    	// Shift mask to indicate next bit.
    	mask <<= 1;

    	// Double num.
    	num = (num << 1) ^ (num & 0x80 ? BPOLY : 0);
  	}

  	return result;
}

unsigned char DotProduct( const unsigned char * vector1, unsigned char * vector2 )
{
  	unsigned char result = 0 ,i;

  	//result ^= Multiply( *vector1++, *vector2++ );
  	//result ^= Multiply( *vector1++, *vector2++ );
  	//result ^= Multiply( *vector1++, *vector2++ );
  	//result ^= Multiply( *vector1  , *vector2   );

  	for(i = 4; i > 0; i--)
    	result ^= Multiply( *vector1++, *vector2++ );

  	return result;
}
  
const unsigned char row[8] = {
    0x02, 0x03, 0x01, 0x01,
    0x02, 0x03, 0x01, 0x01
}; // Prepare first row of matrix twice, to eliminate need for cycling.
void MixColumn( unsigned char * column )
{
  	unsigned char result[4];

  	// Take dot products of each matrix row and the column vector.
  	result[0] = DotProduct( row+0, column );
  	result[1] = DotProduct( row+3, column );
  	result[2] = DotProduct( row+2, column );
  	result[3] = DotProduct( row+1, column );

  	// Copy temporary result to original column.
  	//column[0] = result[0];
  	//column[1] = result[1];
  	//column[2] = result[2];
  	//column[3] = result[3];
  	CopyBytes(column, result, 4);
}

/*
void MixColumns( unsigned char * state )
{
  	MixColumn( state + 0*4 );
  	MixColumn( state + 1*4 );
  	MixColumn( state + 2*4 );
  	MixColumn( state + 3*4 );
}
*/
void ShiftRows( unsigned char * state )
{
  	unsigned char temp;
  	
	// Note: State is arranged column by column.

  	// Cycle second row left one time.
  	temp = state[ 1 + 0*4 ];
  	state[ 1 + 0*4 ] = state[ 1 + 1*4 ];
  	state[ 1 + 1*4 ] = state[ 1 + 2*4 ];
  	state[ 1 + 2*4 ] = state[ 1 + 3*4 ];
  	state[ 1 + 3*4 ] = temp;

  	// Cycle third row left two times.
  	temp = state[ 2 + 0*4 ];
  	state[ 2 + 0*4 ] = state[ 2 + 2*4 ];
  	state[ 2 + 2*4 ] = temp;
  	temp = state[ 2 + 1*4 ];
  	state[ 2 + 1*4 ] = state[ 2 + 3*4 ];
  	state[ 2 + 3*4 ] = temp;

  	// Cycle fourth row left three times, ie. right once.
  	temp = state[ 3 + 3*4 ];
  	state[ 3 + 3*4 ] = state[ 3 + 2*4 ];
  	state[ 3 + 2*4 ] = state[ 3 + 1*4 ];
  	state[ 3 + 1*4 ] = state[ 3 + 0*4 ];
  	state[ 3 + 0*4 ] = temp;
}

void Cipher( unsigned char * block, unsigned char * expandedKey )
{
  	unsigned char i, j;
  	unsigned char round = ROUNDS-1;

  	XORBytes( block, expandedKey, 16 );
  	expandedKey += BLOCKSIZE;

  	do {
    	SubBytes( block, 16 );
    	ShiftRows( block );
    	//MixColumns( block );
    	for(i = 4, j = 0; i > 0; i--, j+=4)
      	MixColumn( block + j );
    	XORBytes( block, expandedKey, 16 );
    	expandedKey += BLOCKSIZE;
  	} while( --round );

  	SubBytes( block, 16 );
  	ShiftRows( block );
  	XORBytes( block, expandedKey, 16 );
}

void aesEncInit(void)
{
  	powTbl = block1;
  	logTbl = block2;
  	CalcPowLog( powTbl, logTbl );

  	sBox = block2;
  	CalcSBox( sBox );

  	expandedKey = block1;
  	KeyExpansion( expandedKey );
}

void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock )
{
  	XORBytes( buffer, chainBlock, BLOCKSIZE );
  	Cipher( buffer, expandedKey );
  	CopyBytes( chainBlock, buffer, BLOCKSIZE );
}

//EncryptBlock
/*
** Note : nSize must be times of 16
*/
void aesEncryptBlock(unsigned char *buffer,unsigned int nSize)
{
	unsigned int n;

	//add 2010-07-27
	for(n = 0; n < 16; n++)
		chainBlock[n] = 0;

	for(n = nSize;n > 0;n -= 16)
	{
		XORBytes( buffer, chainBlock, BLOCKSIZE );
  		Cipher( buffer, expandedKey );
  		CopyBytes( chainBlock, buffer, BLOCKSIZE );
		buffer += 16;
	}
}


//add 2010-10-19
void aesEncryptAt88(unsigned char *buffer)
{
	unsigned int n;

	powTbl = block1;
  	logTbl = block2;
  	CalcPowLog( powTbl, logTbl );

  	sBox = block2;
  	CalcSBox( sBox );

  	expandedKey = block1;
  	KeyExpansionAt88( expandedKey );
	//add 2010-07-27
	for(n = 0; n < 16; n++)
		chainBlock[n] = 0;

	XORBytes( buffer, chainBlock, BLOCKSIZE );
  	Cipher( buffer, expandedKey );
  	CopyBytes( chainBlock, buffer, BLOCKSIZE );
}

/*
********************************************************************************
*                        END
********************************************************************************
*/
