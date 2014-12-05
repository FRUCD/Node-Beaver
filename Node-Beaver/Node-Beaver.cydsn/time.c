#include "time.h"



volatile Time current_time;
uint8_t blink = 0;



CY_ISR(timer_interrupt)
{
	current_time.millisecond += 10;


	if(current_time.millisecond >= 1000)
	{
		LED_Write(blink);
		if(blink==1)
			blink = 0;
		else
			blink =1;
		current_time.millisecond = 0;
	}
} // CY_ISR(timer_interrupt)



void time_init()
{
	time_refresh(); // get time from rtc
	time_isr_StartEx(timer_interrupt);
	Timer_1_Start();
} // time_init()



void time_refresh()
{
	//get time from rtc
} // time_refresh()



uint32_t time_get()
{
	return 0; // returns UNIX time
} // time_get()
