#ifndef TRANSMITORSIMU_H
#define TRANSMITORSIMU_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "data.h"
#define MSGSZ     128

#define SENDERFLG (IPC_CREAT | 0666)
#define RECFLG 0666


/*
 * Declare the message structure.
 */

typedef struct msgbuffer {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;



int TranSimu_send(char* message);
int TranSimu_init(key_t key, int msgflg); //Transimu_init ,return msqid;
int TranSimu_dirsend(key_t key,char* message);
int TranSimu_rec(char *message);
int TranSimu_recLoop(int sleepTime);

#endif