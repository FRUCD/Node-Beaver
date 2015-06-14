#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <project.h>
#include "data.h"

#define RADIO_UART 1
#define DATA_LEN (15)  
#define ESCAPE (0x11)
#define STANDARD_LEN 
#define _RECEIVER_ADDR (0x0013A20040C84FBF)
#define BROADCAST_ADDR (0x000000000000FFFF)

#define STARTER_DELIM   (0x7E)
#define ESCAPE_CTRL     (0x7D)
#define SOFT_FLOW_CTRL1 (0x11)
#define SOFT_FLOW_CTRL2 (0x13)

#define ESCAPER (0x20)

#define XBEE_SPI 1

void radio_init(void); 
void dummy_put();
void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

#endif
