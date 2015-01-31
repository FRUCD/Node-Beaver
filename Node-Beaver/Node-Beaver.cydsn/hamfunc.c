/* hamfunc.c
Includes functions to encode and decode a byte with Hamming (8,4)
LSB is towards the right of the number
*/
#include "hamfunc.h"



const uint8_t hams[16] =
{
	0x00, 0x87, 0x99, 0x1E, 0xAA, 0x2D, 0x33, 0xB4, 0x4B, 0xCC, 0xD2, 0x55, 0xE1,
	0x66, 0x78, 0xFF
}; // hams[]



void ham_byte(uint8_t byte, uint8_t* upper, uint8_t* lower)
{
	*upper = hams[byte >> 4];
	*lower = hams[byte & 0x0F];
} // ham_byte()



enum HAM unham_nibble(uint8_t byte, uint8_t* nibble)
{
	uint8_t pos = 0, code[8];
	enum HAM signal = INTACT;

	code[0] = byte & 0x01;
	code[1] = (byte >>= 1) & 0x01;
	code[2] = (byte >>= 1) & 0x01;
	code[3] = (byte >>= 1) & 0x01;
	code[4] = (byte >>= 1) & 0x01;
	code[5] = (byte >>= 1) & 0x01;
	code[6] = (byte >>= 1) & 0x01;
	code[7] = (byte >>= 1) & 0x01;

	if((code[2] ^ code[4] ^ code[6]) != code[0]) pos += 1;

	if((code[2] ^ code[5] ^ code[6]) != code[1]) pos += 2;

	if((code[4] ^ code[5] ^ code[6]) != code[3]) pos += 4;

	if(pos)
	{
		code[pos-1] ^= 1; // correct error
		signal = SINGLE;
	} // if single error occured

	if((code[0] ^ code[1] ^ code[2] ^ code[3] ^ code[4] ^ code[5] ^ code[6])
		!= code[7])
	{
		if(pos != 0) // if double error detected
			signal = DOUBLE;
		else
		{
			code[7] ^= 1;
			signal = SINGLE;
		} // else only last parity had error
	} // if last parity doesn't match

	*nibble = code[6];
	*nibble = (*nibble << 1) | code[5];
	*nibble = (*nibble << 1) | code[4];
	*nibble = (*nibble << 1) | code[2];
	return signal;
} // ham_chk_nib()



int unham_byte(uint8_t* output, uint8_t upper, uint8_t lower,
	enum HAM* signal_upper, enum HAM* signal_lower)
{
	uint8_t nibble;

	*signal_upper = unham_nibble(upper, &nibble);
	*output = nibble << 4;
	*signal_lower = unham_nibble(lower, &nibble);

	if(*signal_upper == DOUBLE || *signal_lower == DOUBLE)
		return 0;

	*output |= nibble;
	return 1; // returns 1 if no double error
} // ham_chk_byt()
