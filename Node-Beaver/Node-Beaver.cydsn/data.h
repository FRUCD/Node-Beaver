#ifndef DATA_H
#define DATA_H

#include <project.h>
  
typedef struct
{
  uint32_t time;
  uint16_t type;
  uint32_t value;
} DataPacket;

#endif
