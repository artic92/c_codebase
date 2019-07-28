#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "header.h"

#define NUMRICHIESTE 5

//Mutex per la mutua esclusione delle risposte
pthread_mutex_t mutex;

void client(){

	key_t chiave_richieste = ftok(PATH, CHARREQ), chiave_risposte = ftok(PATH, CHARREP);
	int queue_req = msgget(chiave_richieste, IPC_CREAT|0664);
	int queue_rep = msgget(chiave_risposte, IPC_CREAT|0664);

	int i;
	Messaggio richiesta, risposta;
	richiesta.pid = getpid();

	srand(time(NULL)+getpid());

	for(i = 0; i < NUMRICHIESTE; i++){

		//Prepara il messaggio
		richiesta.valore1 = rand() % 101;
		richiesta.valore2 = rand() % 101;

		//Invia la richiesta
		printf("[%d] Richiesta %d inviata (%d, %d)\n\n", getpid(), i, richiesta.valore1, richiesta.valore2);
		msgsnd(queue_req, &richiesta, sizeof(Messaggio)-sizeof(long), 0);

		//Il client si mette in attensa di messaggi indirizzati a lui
		msgrcv(queue_rep, &risposta, sizeof(Messaggio)-sizeof(long), richiesta.pid, 0);
		printf("[%d] Risposta %d ricevuta (%d)\n\n", getpid(), i, risposta.valore1);

	}
}

void* calcolaProdotto(void* r){
	Messaggio* m = (Messaggio*) r;

	key_t chiave_risposte = ftok(PATH, CHARREP);
	int queue_rep = msgget(chiave_risposte, IPC_CREAT|0664);

	//La risposta viene messa nel valore1 del messaggio
	m->valore1 = (m->valore1)*(m->valore2);

	printf("Thread pronto. Invio a %d del calcolo: %d\n\n", m->pid, m->valore1);

	//Invia la risposta in mutua esclusione
	pthread_mutex_lock(&mutex);
	msgsnd(queue_rep, m, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
	pthread_mutex_unlock(&mutex);

	free(m);
	pthread_exit(NULL);
}


