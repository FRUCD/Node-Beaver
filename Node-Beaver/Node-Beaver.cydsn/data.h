#ifndef DATA_H
#define DATA_H

#include <project.h>

#define CAN_QUEUE_LENGTH 128
#define DATA_QUEUE_LENGTH 256
#define USB_QUEUE_LENGTH 256

#define CAN_UNKNOWN 0x000 // 0
#define CAN_THROTTLE 0x205 // 517

#define ID_UNKNOWN CAN_UNKNOWN
#define ID_THROTTLE_1 CAN_THROTTLE
    
#define DATA_PAC_LENGTH 15

enum Types
{
	TYPE_UNKNOWN,
	TYPE_THROTTLE_1
};


  
typedef struct
{
    uint32_t time;
    uint8_t type;
    uint16_t id; // id is for tracking CAN ID
    uint64_t value;
} DataPacket;

#endif
