#include "sd_manager.h"



FS_FILE* pfile;
uint8_t sd_ok = 0;



CY_ISR(power_interrupt)
{
	sd_stop();
} // CY_ISR(power_interrupt)



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
		// FS_SetFileTime()
	} // if a single file volume exists

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
	
} // sd_init()



void sd_push(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
	if(!sd_ok)
		return;
	//push to queue

	char buffer[128];
	short length = 0;


	uint16_t pos;
	for(pos=data_head; pos!=data_tail; pos=(pos+1)%DATA_QUEUE_LENGTH)
	{
		//write
		length = sprintf(buffer, "%u,%u,%llu,%u\n",
			(unsigned)data_queue[pos].type,
			(unsigned)data_queue[pos].time,
			(unsigned long long)data_queue[pos].value,
			(unsigned)data_queue[pos].id);
		FS_Write(pfile, buffer, length);
	} // for all messages in data queue
} // sd_push()



void sd_stop()
{
	FS_FClose(pfile);
	FS_Sync("");
	FS_Unmount("");
} // sd_stop()

