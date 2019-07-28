#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NUMVOLTE 4

int main (int argc, char *argv[])
{
	printf("[%d] Client ready...\n", getpid());

	int i;

	//Alloca coda
	key_t chiave = ftok(PATH, QUEUE);
	int id_queue = msgget(chiave, IPC_CREAT|0664);

	//Preparazione messaggio
	Messaggio m;
	m.processo = getpid();

	srand(time(NULL) + getpid());

	//Ripeti tutto NUMVOLTE volte
	for(i = 0; i < NUMVOLTE; i++){

		//Scegli due valori casuali da 1 a 10
		m.valore1 = (rand() % 10) + 1;
		m.valore2 = (rand() % 10) + 1;

		//Invia questi valori al server
		int risp = msgsnd(id_queue, &m, sizeof(Messaggio)-sizeof(long), 0);
		if(risp < 0){
			perror("Errore invio messaggio\n\n");
			_exit(0);
		}

		printf("[%d] Client - inviato messaggio (%d, %d)\n\n", getpid(), m.valore1, m.valore2);

		//Attendi un tempo casuale tra 1 e 2 secondi
		int sleep_time = (rand() % 2) + 1;

		printf("[%d] Attendo %d secondo/i\n\n", getpid(), sleep_time);

		sleep(sleep_time);
	}


	return 0;
}
