#ifndef CBeeConnector_H
#define CBeeConnector_H
/*
This lib gives coder/decoder an abstract protocol.
This lib will make use of different types of communication methods.
Because controller can communicate with XBee by
real world Serial, SPI,I2C
and even Xbee emulator which is used during debugging and developing
*/
#include <stdint.h>

typedef struct XBeePort{
	char* XBName[16];
	uint16_t XBShortAdd;
	uint64_t XBLongAdd;
	uint8_t XBBaud;
	uint32_t XBMqch;


}myXBeePort;

int XBConnect();
int XBSend();
int XBRec();



#endif