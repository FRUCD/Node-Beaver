#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include <stdio.h>
#include "data.h"



void can_get(DataPacket* data_queue, uint16_t* data_head, uint16_t* data_tail);

void can_init(void);

//uint8_t can_test_send();


#endif
