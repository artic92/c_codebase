#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

int main (int argc, char *argv[])
{
	int i;
	
	//Allocazione coda
	key_t chiave = ftok(PATH, CHARB);
	int queue = msgget(chiave, IPC_CREAT|0664);
	
	//Definizione messaggio
	Messaggio m;
	m.type = CLIENT;
	m.pid = getpid();
	
	for(i = 0; i < 15; i++){
		msgsnd(queue, &m, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
		sleep(1);
	}
	
	return 0;
}
