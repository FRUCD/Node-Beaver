#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#include <project.h>
#include <stdio.h>
#include "data.h"

#define UART_BUFF_LENGTH 512
#define START_B 0x7E
#define ESCAP_B 0x7D
#define XOR_B 0x20


void usb_init();

void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

inline void usb_escape(uint8_t* buffer, uint16_t* buff_end, uint8_t byte);

void usb_get();


#endif
