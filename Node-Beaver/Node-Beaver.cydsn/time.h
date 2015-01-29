#ifndef TIME_H
#define TIME_H

#include <project.h>
#include <stdlib.h>
#include "data.h"



CY_ISR_PROTO(time_one_sec_vector);
CY_ISR_PROTO(time_refresh_vector);
void time_init();
Time time_get();
void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail);
void time_retreive(); // retreives full time from RTC



#endif
