#include "CBeeDebug.h"
#define DEBUG_QUEUE_SIZE 256;


void Debug_init(myDebug* obj,int queueSize){
	int i=0;
	*obj.errCount=0;
	*obj.errQueue=(char**)malloc(DEBUG_QUEUE_SIZE*sizeof(char*));
	for (i=0;i<DEBUG_QUEUE_SIZE;i++){
		*obj.errQueue[i]=(char*)malloc(DEBUG_QUEUE_SIZE*sizeof(char));
	}
}


