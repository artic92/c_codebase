#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("[%d] Server3 ready...\n", getpid());
	
	int i;
	
	//Allocazione coda
	key_t chiave = ftok(PATH, CHARS);
	int queue = msgget(chiave, IPC_CREAT|0664);
	
	//Definizione messaggio
	Messaggio m;
	
	while(1){

		msgrcv(queue, &m, sizeof(Messaggio)-sizeof(long), SERVER3, 0);
		if(m.pid == TERMINATE){
			printf("[Server3] Goodbye...\n");
			_exit(0);
		}
		
		printf("[Server3] Ricevuto messaggio dal client con PID %d\n", m.pid);
		
	}
	return 0;
}
