#ifndef TIME_H
#define TIME_H

#include <project.h>



typedef struct
{
	uint8_t month, day, hour, minute, second, millisecond;
	uint16_t year;
} Time;




CY_ISR_PROTO(timer_interrupt);
void time_init();
void time_get();
void time_refresh();



#endif
