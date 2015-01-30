#ifndef TIME_H
#define TIME_H

#include <project.h>
#include <stdlib.h>
#include "data.h"

#define RTC_ADDR 0x68
#define RTC_CONFIG 0x0E
#define RTC_HOURS 0x02



CY_ISR_PROTO(time_one_sec_vector);
CY_ISR_PROTO(time_refresh_vector);
void time_init();
Time time_get();
void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail);
Time time_retreive(); // retreives full time from RTC



#endif
