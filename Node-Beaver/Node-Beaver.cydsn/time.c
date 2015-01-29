#include "time.h"



volatile Time current_time;
volatile uint8_t init_status = 0, refresh_status = 0;
volatile uint32_t next_millis = 0;
uint8_t blink = 0;



CY_ISR(time_one_sec_vector)
{
	LED_Write(1);
	// if need to initialize, start ms at 0 and set init to 1
	if(!init_status)
	{
		millis_timer_WriteCounter(0);
		next_millis = 1000;
		init_status = 1;	
	} // if need to init
	else
	{
		millis_timer_WriteCounter(next_millis); // snap ms counter to next ms
		next_millis = (next_millis + 1000) & 0xFFFF; // calculate next ms and wrap
	} // else snap to next millis

	time_retreive(); // get time from rtc
} // CY_ISR(time_one_sec_vector)



CY_ISR(time_refresh_vector)
{
	// get UNIX Time with milli counter ready for injection into data_queue
	time_retreive(); // get time from rtc
	current_time.millicounter = millis_timer_ReadCounter();
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
	current_time.millicounter= 4;

	rtc_i2c_Start();
	//configure RTC

	rtc_i2c_MasterSendStart(RTC_ADDR, 0);
	rtc_i2c_MasterWriteByte(RTC_CONFIG);
	rtc_i2c_MasterWriteByte(0x40);
	rtc_i2c_MasterSendStop();


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
	if(refresh_status)
	{
		;// inject prepared time packet into queue
	}
} // time_refresh()



Time time_get()
{
	current_time.millicounter = millis_timer_ReadCounter();
	return current_time;
} // time_get()



Time time_retreive()
{
	Time tmp_time;
	// Set register pointer to 0
	rtc_i2c_MasterSendStart(RTC_ADDR, 0);
	rtc_i2c_MasterWriteByte(0x00);
	rtc_i2c_MasterSendStop();

	return tmp_time; 
} // time_retreive()
