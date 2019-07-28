#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("Server ready...\n");
	
	key_t chiave_richieste = ftok(PATH, CHARREQ), chiave_risposte = ftok(PATH, CHARREP);
	int queue_req = msgget(chiave_richieste, IPC_CREAT|0664);
	int queue_rep = msgget(chiave_risposte, IPC_CREAT|0664);
	
	pthread_mutex_init(&mutex, NULL);	
	
	while(1){
		
		//SE NON ALLOCHI DINAMICAMENTE DAI A TUTTI I THREAD LA STESSA STRUTTURA (VEDI CORREZIONE PROF)
		//CIOÈ L'ULTIMA RICEVUTA
		Messaggio* risposta = malloc(sizeof(Messaggio));
		risposta->pid = 0;
		risposta->valore1 = 0;
		risposta->valore2 = 0;
		
		msgrcv(queue_req, risposta, sizeof(Messaggio)-sizeof(long), 0, 0);
		if(risposta->valore1 == -1 && risposta->valore2 == -1){
			printf("[Server] Goodbye...\n");
			pthread_mutex_destroy(&mutex);
			_exit(0);
		}
		
		//Crea dei thread NON joinable passandogli i valori appena ottenuti
		pthread_t thread;
		pthread_create(&thread, NULL, calcolaProdotto, (void*) risposta);

	}	
	
	return 0;
}
