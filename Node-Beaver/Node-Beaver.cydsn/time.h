#ifndef TIME_H
#define TIME_H

#include <project.h>
#include "data.h"

// RTC (DS3231) I2C address
#define RTC_ADDR 0x68

// RTC register addresses
#define RTC_CONFIG 0x0E
#define RTC_HOURS 0x02

// This is a "CAN ID" but it is never anounced to the CAN network
#define ID_TIME 0x123



typedef struct
{
	uint8_t month, day, hour, minute, second;
	uint16_t millicounter;
	uint16_t year;
} Time;



CY_ISR_PROTO(time_refresh_vector);

void time_init(void);

Time time_get(void);

void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail);

Time time_retreive(void); // retreives full time from RTC



#endif
