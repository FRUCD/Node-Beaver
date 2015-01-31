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
#include "data.h"
#include "TransmitorSimu.h"  //XBVIRT
#define XBSERIAL 0
#define XBI2C 1
#define XBSPI 2
#define XBVIRT 3


typedef struct XBeePort{
	int XBType;
	char* XBName[16];
	uint16_t XBShortSelfAdd;
	uint64_t XBLongSelfAdd;
	uint16_t XBShortTgtAdd;
	uint64_t XBLongTgtAdd;
	uint8_t XBBaud;
	key_t XBMqch;
	char message[256];


}myXBeePort;

int XBConnect(myXBeePort* passport);
int XBSend(char message[256]);
int XBRec(char *message);

//virutal connection with XBeeEmulator
//connect XBee by I2C
//connect XBee by SPI
//connect XBee by Serial



#endif