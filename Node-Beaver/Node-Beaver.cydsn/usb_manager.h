#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#include <project.h>
#include <stdio.h>
#include "data.h"
#include "can_manager.h" // for probing can_queue head/tail


void usb_init();
void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);
void usb_get();


#endif
