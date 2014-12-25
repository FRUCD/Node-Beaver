#include <stdio.h>
#include <stdlib.h>
#include "TransmitorSimu.h"

int main(int argc, char** argv){
	int option = 1;
	if (argc==2){ //no argument
		option = atoi(argv[1]);   
	}
	if (option==0){  //send mode
		char message[MSGSZ];
		TranSimu_init((key_t)1234,SENDERFLG);
		while(1){
			printf("Please input msg that you want to send: ");
			scanf("%s",message);
			TranSimu_send(message);
		}
		return 0;


	}else{  //rec mode
		TranSimu_init((key_t)1234,RECFLG);
		TranSimu_recLoop(1);
		return 0;
	}
	return 0;
}