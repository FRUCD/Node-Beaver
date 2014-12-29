#ifndef HAMFUNC_H
#define HAMFUNC_H

#include <stdint.h>

enum HAM
{
	INTACT, SINGLE, DOUBLE
}; /* enum HAM */

void ham_byte(uint8_t input, uint8_t* upper, uint8_t* lower);

enum HAM unham_nibble(uint8_t byte, uint8_t* nibble);

int unham_byte(uint8_t* output, uint8_t upper, uint8_t lower, enum HAM*
	signal_upper, enum HAM* signal_lower);

#endif
