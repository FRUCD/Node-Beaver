#ifndef TIME_H
#define TIME_H

#include <project.h>
#include "data.h"



typedef struct
{
	uint8_t month, day, hour, minute, second;
	uint16_t millisecond;
	uint16_t year;
} Time;




CY_ISR_PROTO(time_one_sec_vector);
CY_ISR_PROTO(time_refresh_vector);
void time_init();
uint32_t time_get_unix();
Time time_get();
void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail);



#endif
