#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("[%d] Printer ready...\n", getpid());
	
	key_t chiave = ftok(PATH, CHAR2);
	int queue = msgget(chiave, IPC_CREAT|0664);
	
	//Numero di messaggi ricevuti dal server
	int nmessaggi = 1;
	
	Messaggio m;
	
	while(1){
		msgrcv(queue, &m, sizeof(Messaggio)-sizeof(long), 0, 0);
		if(m.type == TERMINATE){
			printf("[Printer] Goodbye ...\n");
			_exit(0);
		}else{
			printf("{Printer}	[%d] %d\n", nmessaggi, m.pid);
			nmessaggi++;
		}
	}
	
	return 0;
}
