#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

#define NUMCLIENT 10

int main (int argc, char *argv[])
{
	printf("[%d] Server ready...\n", getpid());
	
	//Buffer dove vengono salvati i pid dei client che inviano messaggi
	pid_t buffer[NUMCLIENT];
	
	//Allocazione code per comunicare col printer e con i client
	key_t chiave1 = ftok(PATH, CHAR), chiave2 = ftok(PATH, CHAR2);
	int queue2 = msgget(chiave2, IPC_CREAT|0664);
	int queue1 = msgget(chiave1, IPC_CREAT|0664);
	
	int i;
	Messaggio m1, m2;
	m2.type = SERVER;
	
	while(1){
		//Al riempimento del buffer invia messaggi al printer
		for(i = 0; i < NUMCLIENT; i++){
			msgrcv(queue1, &m1, sizeof(Messaggio)- sizeof(long), 0, 0);
			//Se il messaggio è quello di terminazione inviato dal padre
			if(m1.type == TERMINATE){
				m2.type = TERMINATE;
				msgsnd(queue2, &m2, sizeof(Messaggio)-sizeof(long), 0);
				printf("[Server] Goodbye...\n");
				_exit(0);
			}
			//Altrimenti riempi il buffer
			buffer[i] = m1.pid;
		}
		
		//Invia il contenuto del buffer al printer
		for(i = 0; i < NUMCLIENT; i++){
			m2.pid = buffer[i];
			msgsnd(queue2, &m2, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
		}
	}
	
	return 0;
}
