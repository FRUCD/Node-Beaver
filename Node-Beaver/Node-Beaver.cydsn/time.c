#include "time.h"



volatile Time current_time;



CY_ISR(timer_interrupt)
{
	current_time.millisecond++;

	if(current_time.millisecond >= 1000)
		time_refresh();
} // CY_ISR(timer_interrupt)



void time_init()
{
	time_isr_StartEx(timer_interrupt);
	Timer_1_Start();
} // time_init()



void time_refresh()
{
	//get time from rtc
} // time_refresh()



void time_get(Time* time)
{
	*time = current_time;
} // time_get()
