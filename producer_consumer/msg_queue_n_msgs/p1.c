//Processo CLIENT p1

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main (int argc, char *argv[])
{
	printf("Processo P1 avviato\n");
	key_t chiave = ftok(PATH, LETTER);
	int i, queue, risp;
	Messaggio m;
	m.id = P1;

	queue = msgget(chiave, IPC_CREAT|0664);
	if(queue < 0){
		perror("Errore creazione coda!\n");
		_exit(-1);
	}

	srand(time(NULL));

	for(i = 0; i < 11; i++){
		m.val = generaFloat(0, 10);//non funziona genFloat!!dovrebbe uscire un val da 1 a 10
		printf("Invio messaggio <%d,%d>\n", m.id, m.val);
		risp = msgsnd(queue, (void *)&m, sizeof(Messaggio)-sizeof(long), 0);
		if(risp < 0){
			perror("Errore invio!\n");
			_exit(-1);
		}
		sleep(1);
	}

	return 0;
}
