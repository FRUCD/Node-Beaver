#ifndef DATA_H
#define DATA_H

#include <project.h>

#define CAN_QUEUE_LENGTH 1024
#define DATA_QUEUE_LENGTH 1024

 

typedef struct
{
	uint32_t millicounter;
	uint16_t id; // id is for tracking CAN ID
	uint8_t length;
	uint8_t data[8];
} DataPacket;

#endif
