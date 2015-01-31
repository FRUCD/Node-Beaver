#ifndef CBeeDebug_H
#define CBeeDebug_H
/*This library is for debugging. This lirary has an internal message queue and ....err output
*/

#include "data.h"
#include "stdio.h"
#include "stdlib.h"
#define DEBUG_QUEUE_SIZE 256;

typedef struct CBeeDebug{
	int errCount;
	char** errQueue;
}myDebug;


void Debug_init(myDebug* obj,int queueSize);

#endif