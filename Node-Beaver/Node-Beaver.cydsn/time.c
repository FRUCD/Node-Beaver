#include "time.h"



volatile Time current_time;
volatile uint8_t init_status = 0, refresh_status = 0;
uint8_t blink = 0;



CY_ISR(time_one_sec_vector)
{
	// if need to initialize, start ms at 0 and set init to 1
	// snap ms counter to next ms
	// calculate next ms
	// get time from RTC
	// write new time to struct

	/*
	current_time.millisecond += 10;


	if(current_time.millisecond >= 1000)
	{
		//LED_Write(blink);
		if(blink==1)
			blink = 0;
		else
			blink =1;
		current_time.millisecond = 0;
	}
	*/
} // CY_ISR(time_one_sec_vector)



CY_ISR(time_refresh_vector)
{
	// get UNIX Time with milli counter ready for injection into data_queue
	refresh_status = 1;
} // CY_ISR(time_refresh_vector)



void time_init()
{
	// fill struct for debugging
	current_time.month = 12;
	current_time.day = 30;
	current_time.year = 14;
	current_time.hour = 1;
	current_time.minute = 2;
	current_time.second = 3;
	current_time.millisecond = 4;

	time_one_sec_isr_StartEx(time_one_sec_vector); // enable rtc isr
	while(!init_status); // wait for second synchronization
	time_refresh_isr_StartEx(time_refresh_vector); // enable 10 second isr


	// Start timers
	millis_timer_Start();
	time_refresh_timer_Start();
} // time_init()



void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail)
{
	if(refresh_status);
		// inject prepared time packet into queue
} // time_refresh()



uint32_t time_get_unix()
{
	return 0; // returns UNIX time
} // time_get_unix()



Time time_get()
{
	return current_time;
} // time_get()
