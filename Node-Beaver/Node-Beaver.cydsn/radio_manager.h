#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <project.h>
#include "data.h"

#define RADIO_UART 1
#define DATA_LEN 15  
#define ESCAPE 0x11
 
void myUART_Start(uint8_t option);
uint8_t checksum(uint8_t* msg);
void radio_get_bytes(const DataPacket* in_pkt,uint8_t* byte_ptr);   

void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

#endif
