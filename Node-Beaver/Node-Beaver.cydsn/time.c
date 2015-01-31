#include "time.h"



volatile Time current_time;
volatile uint8_t init_status = 0, refresh_status = 0;
volatile uint32_t next_millis = 0;
volatile uint8_t blink = 0;



CY_ISR(time_one_sec_vector)
{
	LED_Write(blink^0x01);
	if(!init_status)
	{
		millis_timer_WriteCounter(0); // start ms at 0
		next_millis = 1000;
		init_status = 1;	
	} // if need to init
	else
	{
		millis_timer_WriteCounter(next_millis); // snap ms counter to next ms
		next_millis = (next_millis + 1000) & 0xFFFFFF; // calculate next ms and wrap
	} // else snap to next millis
} // CY_ISR(time_one_sec_vector)



CY_ISR(time_refresh_vector)
{
	// get UNIX Time with milli counter ready for injection into data_queue
	current_time = time_retreive(); // get time from rtc
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

	// configure RTC
	rtc_i2c_MasterSendStart(RTC_ADDR, 0);
	rtc_i2c_MasterWriteByte(RTC_CONFIG);
	rtc_i2c_MasterWriteByte(0x40);
	rtc_i2c_MasterSendStop();


	// Set 24 hour
	uint8_t byte;
	rtc_i2c_MasterSendStart(RTC_ADDR, 0); // Move to hours register
	rtc_i2c_MasterWriteByte(RTC_HOURS);
	rtc_i2c_MasterSendStop();

	rtc_i2c_MasterSendStart(RTC_ADDR, 1); // save hours register
	byte = rtc_i2c_MasterReadByte(0);
	rtc_i2c_MasterSendStop();

	rtc_i2c_MasterSendStart(RTC_ADDR, 0); // Set 24 hour bit
	rtc_i2c_MasterWriteByte(RTC_HOURS);
	rtc_i2c_MasterWriteByte(0x40 | byte);
	rtc_i2c_MasterSendStop();

	time_one_sec_isr_StartEx(time_one_sec_vector); // enable rtc isr
	
	while(!init_status); // wait for second synchronization

	time_refresh_isr_StartEx(time_refresh_vector); // enable 10 second isr

	// Start timers
	millis_timer_Start();
	time_refresh_timer_Start();

	current_time = time_retreive();
} // time_init()



void time_announce(DataPacket* data_queue, uint16_t* data_head,
	uint16_t* data_tail)
{
	/* Time Frame
		START COUNTER year_upper, year_lower, month, date, hour, minutes, seconds
	*/


	if(refresh_status)
	{
		data_queue[*data_tail].id = ID_TIME;
		data_queue[*data_tail].length = 8;
		data_queue[*data_tail].millicounter = current_time.millicounter;

		data_queue[*data_tail].data[0] = 0;
		data_queue[*data_tail].data[1] = current_time.year >> 8;
		data_queue[*data_tail].data[2] = current_time.year;
		data_queue[*data_tail].data[3] = current_time.month;
		data_queue[*data_tail].data[4] = current_time.day;
		data_queue[*data_tail].data[5] = current_time.hour;
		data_queue[*data_tail].data[6] = current_time.minute;
		data_queue[*data_tail].data[7] = current_time.second;

		*data_tail = (*data_tail + 1) % DATA_QUEUE_LENGTH; // increment data tail

		if(*data_tail == *data_head) // if need to roll data queue
			*data_head = (*data_head + 1) % DATA_QUEUE_LENGTH;
	} // if refresh status set by time_refresh isr
} // time_refresh()



Time time_get()
{
	current_time.millicounter = millis_timer_ReadCounter();
	return current_time;
} // time_get()



Time time_retreive()
{
	Time tmp_time;
	uint8_t byte;

	// Set register pointer to 0
	rtc_i2c_MasterSendStart(RTC_ADDR, 0);
	rtc_i2c_MasterWriteByte(0x00);
	rtc_i2c_MasterSendStop();

	

	rtc_i2c_MasterSendStart(RTC_ADDR, 1); // Begin receiving

	byte = rtc_i2c_MasterReadByte(1);
	tmp_time.second = byte & 0x0F; // seconds
	tmp_time.second += 10 * (byte >> 4); // 10 seconds

	byte = rtc_i2c_MasterReadByte(1);
	tmp_time.minute = byte & 0x0F; // minutes
	tmp_time.minute += 10 * (byte >> 4); // 10 seconds

	byte = rtc_i2c_MasterReadByte(1);
	tmp_time.hour = byte & 0x0F; // hours 
	tmp_time.hour += 10 * ((byte&0x3F) >> 4); // 10 hours

	byte = rtc_i2c_MasterReadByte(1); // skip day

	byte = rtc_i2c_MasterReadByte(1);
	tmp_time.day = byte & 0x0F; // date
	tmp_time.day += 10 * (byte >> 4); // 10 date

	byte = rtc_i2c_MasterReadByte(1);
	tmp_time.month = byte & 0x0F; // Month/Century 
	tmp_time.month += 10 * ((byte & 0x01) >> 4); // 10 Month

	byte = rtc_i2c_MasterReadByte(0);
	tmp_time.year = byte & 0x0F; // Year
	tmp_time.year += 10 * (byte >> 4); // 10 Years

	rtc_i2c_MasterSendStop(); // End Receiving

	tmp_time.millicounter = millis_timer_ReadCounter();

	return tmp_time; 
} // time_retreive()
