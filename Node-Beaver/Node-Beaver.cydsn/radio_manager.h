#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <project.h>
#include "data.h"

#define RADIO_UART 1
   
void myUART_Start(uint8_t option);
   
void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

#endif
