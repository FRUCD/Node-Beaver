#include "sd_manager.h"



FS_FILE* pfile;
uint8_t sd_ok = 0;
DataPacket sd_queue[SD_QUEUE_LENGTH];
uint8_t sd_pos = 0;



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
	char string[64];

	if(FS_GetNumVolumes() == 1)
	{
		if(FS_ATTR_DIRECTORY != FS_GetFileAttributes("logs"))
			if(FS_MkDir("logs")) // create logs directory
			{
				sd_ok = 0;
				return;
			} // if logs folder can't be created

		// get time and date for naming day folder
		/*
		if(sd_ok)
			sprintf(string[64], "%d/%d/%d", time.month, time.day, time.year);
			*/

		if(sd_ok && FS_ATTR_DIRECTORY != FS_GetFileAttributes("\\logs\\days"))
			if(FS_MkDir("\\logs\\day")) // create runs directory
			{
				sd_ok = 0;
				return;
			} // if day folder can't be created


		pfile = FS_FOpen("\\logs\\day\\run.csv", "w"); // open test file
	} // if a single file volume exists

	sd_stop(); // for testing
} // sd_init()



void sd_push(const DataPacket* data_queue, uint16_t data_head,
             uint16_t data_tail)
{
	if(!sd_ok)
		return;
	//push to queue
	//if queue is full, write to sd
} // sd_push()



void sd_write()
{
	// write queue to sd and clear queue
	sd_pos = 0;
} // sd_write()



void sd_stop()
{
	sd_write();
	FS_FClose(pfile);
} // sd_stop()

