#ifndef DATA_H
#define DATA_H

#include <project.h>

// SD queue must be longer than can queue
#define CAN_QUEUE_LENGTH 32
#define DATA_QUEUE_LENGTH CAN_QUEUE_LENGTH
#define USB_QUEUE_LENGTH CAN_QUEUE_LENGTH
#define SD_QUEUE_LENGTH 128

#define CAN_UNKNOWN 0x000 // 0
#define CAN_THROTTLE 0x205 // 517

enum Types
{
	TYPE_UNKNOWN,
	TYPE_THROTTLE
};


  
typedef struct
{
  uint32_t time;
  uint16_t type, id;
  uint64_t value;
} DataPacket;

#endif
