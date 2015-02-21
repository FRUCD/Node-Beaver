#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <project.h>
#include "data.h"

#define RADIO_UART 1
#define DATA_LEN 15  
#define ESCAPE 0x11
#define STANDARD_LEN 
#define STARTER_DELIM 0x7e
#define _RECEIVER_ADDR 0x0013A20040C84FBF

#define XBEE_SPI 1

    
void _XBee_tx_req_(const DataPacket* msg);

void radio_init(void); 
void myUART_Start(uint8_t option);
uint8_t checksum(uint8_t* msg,int len);
void radio_get_bytes(const DataPacket in_pkt,uint8_t* byte_ptr);   

void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

#endif
