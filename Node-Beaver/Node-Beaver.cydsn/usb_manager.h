#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#define USB_DEBUGGING

#include <project.h>
#include <stdio.h>
#include "data.h"
#include "hamfunc.h"

#ifdef USB_DEBUGGING
#include "can_manager.h" // for probing can_queue head/tail
#endif


void usb_init();

void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

inline void usb_prebuff_val(const DataPacket* datum, uint8_t* prebuffer,
	uint16_t* prebuff_end);

inline void usb_hamming(uint8_t* buffer, uint16_t* buff_end, uint8_t* prebuffer,
	uint16_t* prebuff_end);

void usb_get();


#endif
