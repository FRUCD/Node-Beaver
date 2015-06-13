#include "sd_manager.h"



FS_FILE* pfile;
uint8_t sd_ok = 0;

const char set_time_file[] = "\\logs\\set_time.txt";



CY_ISR(power_interrupt)
{
	//test_Write(1);
	sd_stop();
	//test_Write(0);
} // CY_ISR(power_interrupt)




/* sd_init()
	Takes Time struct (time). Returns nothing.

	Initializes SD card filesystem.
	The following events will cause the sd_ok flag to be reset, which aborts all
	SD functions:
		- the SD card is not found
		- unable to create the "LOGS" directory
		- unable to create a directory named after the date
		- unable to create and open file for writing

	sd_ok is set when the SD card is functional
*/
void sd_init(Time time)
{
	/* power_isr note:
		Triggers unexpectedly due to floating pin/environmental voltages and
		capacitance. power isr is disabled for prototyping only.
	*/
	//power_isr_StartEx(power_interrupt);
	FS_Init();
	sd_ok = 1;
	char date_str[32], run_str[64];

	if(FS_GetNumVolumes() == 1)
	{
		FS_SetFileWriteMode(FS_WRITEMODE_FAST);

		if(FS_ATTR_DIRECTORY != FS_GetFileAttributes("logs")) // if logs not a dir
			if(FS_MkDir("logs"))
			{
				sd_ok = 0;
				return;
			} // if logs folder can't be created

		// insert time getting

		
		if((pfile = FS_FOpen(set_time_file, "r")))
		{
			char buffer[64];
			char *ptr;
			uint16_t num;
			Time tmp_time;


			/* Time Setting File (set_time.txt) Format
				Enter the following two lines of text in a file called "set_time.txt" in
				the /LOGS folder to set the time. The line breaks can consist of \r or
				\n characters.
				
				[month]/[day]/[year]
				[24-hour]:[minute]:[second]
			*/
			FS_Read(pfile, buffer, 64); // read entire file

			ptr = strtok(buffer, "/: \r\n"); // month
			num = atoi(ptr);
			if(num <= 12) tmp_time.month = num;
			
			ptr = strtok(NULL, "/: \r\n"); // day
			num = atoi(ptr);
			if(num <= 31) tmp_time.day = num;
			
			ptr = strtok(NULL, "/: \r\n"); // year
			num = atoi(ptr);
			if(num <= 99) tmp_time.year = num;  // 2 digit year
			else if(num >= 1900) tmp_time.year = num % 100; // 4 digit year
				
			ptr = strtok(NULL, "/: \r\n"); // 24-hour
			num = atoi(ptr);
			if(num <= 23) tmp_time.hour = num;

			ptr = strtok(NULL, "/: \r\n"); // minute
			num = atoi(ptr);
			if(num <= 59) tmp_time.minute = num;

			ptr = strtok(NULL, "/: \r\n"); // second 
			num = atoi(ptr);
			if(num <= 59) tmp_time.second = num;

			FS_FClose(pfile);

			time_set(tmp_time); // set the new time
			time = tmp_time; // use new time for file names
			
			FS_Remove(set_time_file); // delete file
		} // try to find file and set time



		// get time and date for naming day folder
		sprintf(date_str, "\\logs\\%u-%u-%u", time.month, time.day, time.year);

		if(FS_ATTR_DIRECTORY != FS_GetFileAttributes(date_str)) // if day not a dir
			if(FS_MkDir(date_str))
			{
				sd_ok = 0;
				return;
			} // if day folder can't be created

		sprintf(run_str, "%s\\%u-%u-%u.csv", date_str, time.hour, time.minute,
			time.second);

		pfile = FS_FOpen(run_str, "w"); // open test file

		if(pfile == NULL)
		{
			sd_ok = 0;
			return;
		} // if file does not exist

		// Set file time here
		FS_FILETIME file_time;
		unsigned long file_time_string;
		
		file_time.Year = 2000 + (uint16_t)time.year;
		file_time.Month = time.month;
		file_time.Day = time.day;
		file_time.Hour = time.hour;
		file_time.Minute = time.minute;
		file_time.Second = time.second;

		FS_FileTimeToTimeStamp(&file_time, &file_time_string);
		FS_SetFileTime(run_str, file_time_string);
	} // if a single file volume exists
  
  FS_Sync("");
/*
	FS_Write(pfile, "Type,Time,Value,ID\n", 19);

	// test data writing
	char buffer[128];
	short length = 0;

	// test write
	length = sprintf(buffer, "%u,%u,%llu,%u\n", 1,
			0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 517);
	FS_Write(pfile, buffer, length);

	sd_stop(); // for testing
	sd_ok = 0; // for testing
	*/
} // sd_init()



/* sd_push()
	Takes DataPacket queue (data_queue) with its head and tail indices.
	Returns nothing.

	Writes all messages in data_queue to the SD card. Synchronizes the filesystem
	after all messages are written.
*/
void sd_push(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
	if(!sd_ok) return;

	char buffer[128];
	short length = 0;
	uint16_t pos;

	for(pos=data_head; pos!=data_tail; pos=(pos+1)%DATA_QUEUE_LENGTH)
	{
		length = sprintf(buffer, "%u,%u,%X,%X,%X,%X,%X,%X,%X,%X\n",
			(unsigned)data_queue[pos].id,
			(unsigned)data_queue[pos].millicounter,
			(unsigned)data_queue[pos].data[0],
			(unsigned)data_queue[pos].data[1],
			(unsigned)data_queue[pos].data[2],
			(unsigned)data_queue[pos].data[3],
			(unsigned)data_queue[pos].data[4],
			(unsigned)data_queue[pos].data[5],
			(unsigned)data_queue[pos].data[6],
			(unsigned)data_queue[pos].data[7]);

		FS_Write(pfile, buffer, length); // write to SD
	} // for all messages in data queue

	FS_Sync(""); // sync to SD
} // sd_push()



/* sd_stop()
	Takes and returns nothing.

	Closes the file, synchronizes, and unmounts SD card to prevent corruption.
*/
void sd_stop(void)
{
	FS_FClose(pfile);
	FS_Sync("");
	FS_Unmount("");
} // sd_stop()

