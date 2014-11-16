#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <project.h>
#include <FS.h>
#include "data.h"


CY_ISR_PROTO(power_interrupt);
void sd_init();
void sd_push(DataPacket* data_queue, uint8_t data_pos);
void sd_stop();


#endif