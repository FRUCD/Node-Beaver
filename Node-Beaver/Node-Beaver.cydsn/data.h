#ifndef DATA_H
#define DATA_H

#include <project.h>

// SD queue must be longer than can queue
#define CAN_QUEUE_LENGTH 32
#define SD_QUEUE_LENGTH 64
  
typedef struct
{
  uint32_t time;
  uint16_t type;
  uint64_t value;
} DataPacket;

#endif
