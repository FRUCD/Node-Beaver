#include "sd_manager.h"



FS_FILE* pfile;
uint8_t sd_ok = 0;
DataPacket sd_queue[SD_QUEUE_LENGTH];
uint8_t sd_pos = 0;



CY_ISR(power_interrupt)
{
	sd_stop();
} // CY_ISR(power_interrupt)




void sd_init()
{
	power_isr_StartEx(power_interrupt);
	FS_Init();

	if(FS_GetNumVolumes() == 1)
	{
		if(FS_ATTR_DIRECTORY != FS_GetFileAttributes("logs"))
			FS_MkDir("logs"); // create logs directory

		pfile = FS_FOpen("\\logs\\test.txt", "w"); // open test file
		sd_ok = 1;
	} // if a single file volume exists

	sd_stop(); // for testing
} // sd_init()



void sd_push(const DataPacket* data_queue, uint16_t data_head,
             uint16_t data_tail)
{
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

