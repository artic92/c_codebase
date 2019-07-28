//Processo CLIENT p2

#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main (int argc, char *argv[])
{
	printf("Processo P2 avviato\n");
	key_t chiave = ftok(PATH, LETTER);
	int i, queue, risp;
	Messaggio m;
	m.id = P2;
	m.val = 0;

	queue = msgget(chiave, IPC_CREAT|0664);
	if(queue < 0){
		perror("Errore creazione coda!\n");
		_exit(-1);
	}

	srand(time(NULL));

	for(i = 0; i < 11; i++){
		m.val = generaFloat(2, 20);//non funziona genFloat!!dovrebbe uscire un val da 2 a 20
		printf("Invio messaggio <%d,%d>\n", m.id, m.val);
		risp = msgsnd(queue, (void *)&m, sizeof(Messaggio)-sizeof(long), 0);
		if(risp < 0){
			perror("Errore invio!\n");
			_exit(-1);
		}
		sleep(2);
	}

	return 0;
}
