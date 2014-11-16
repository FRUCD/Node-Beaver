#include "sd_manager.h"



FS_FILE* pfile;
uint8_t sd_ok = 0;
DataPacket write_queue[32];
uint8_t queue_pos;




void sd_init()
{
	FS_Init();

	if(FS_GetNumVolumes() == 1)
	{
		if(FS_ATTR_DIRECTORY != FS_GetFileAttributes("logs"))
			FS_MkDir("logs"); // create logs directory

		pfile = FS_FOpen("\\logs\\test.txt", "w"); // open test file
		sd_ok = 1;
	} // if a single file volume exists
} // sd_init()



void sd_push()
{
	//push to queue
	//if queue is full, write to sd
} // sd_push()



void sd_write()
{
	// write queue to sd and clear queue
	queue_pos = 0;
} // sd_write()



void sd_stop()
{
	sd_write();
	FS_FClose(pfile);
} // sd_stop()

