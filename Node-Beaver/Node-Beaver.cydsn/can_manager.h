#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include "data.h"



typedef struct
{
	uint8_t id;
	uint8_t data[8];
} CanMessage;



void can_init();
uint8_t can_test_send();
uint8_t can_get(DataPacket* data_queue, uint8_t data_pos);



#endif