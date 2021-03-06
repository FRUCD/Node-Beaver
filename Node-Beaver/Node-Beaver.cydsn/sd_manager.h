#ifndef SD_MANAGER_H
#define SD_MANAGER_H


#include <project.h>

#include <stdio.h>
#include <stdlib.h>

#include "FS.h"
#include "data.h"
#include "time_manager.h"


/*
#define CAN_UNKNOWN 0x000 // 0
#define CAN_THROTTLE 0x205 // 517

#define ID_UNKNOWN CAN_UNKNOWN
#define ID_THROTTLE_1 CAN_THROTTLE

enum Types
{
	TYPE_UNKNOWN,
	TYPE_THROTTLE_1
};
*/

CY_ISR_PROTO(power_interrupt);

void sd_init(Time time);

void sd_push(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

void sd_stop(void);


#endif
