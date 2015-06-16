#ifndef SD_MANAGER_H
#define SD_MANAGER_H


#include <project.h>

#include <stdio.h>
#include <stdlib.h>

#include "FS.h"
#include "data.h"
#include "time_manager.h"

#define DATE_STR_LEN 64


CY_ISR_PROTO(power_interrupt);

void sd_init(Time time);

void sd_push(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail);

void sd_stop(void);


#endif
