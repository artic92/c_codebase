#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NUMCLIENTI 4

//Non completato e non compreso

int main (int argc, char *argv[])
{
	int i;
	pthread_t clienti[NUMCLIENTI];
	pthread_t thread_stampa;
	
	//Inizializzazione struttura monitor
	struct Monitor * m = (struct Monitor *)malloc(sizeof(struct Monitor));
	inizializza(m);
	
	//Rendo i threads joinable
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Creazione threads
	for(i = 0; i < NUMCLIENTI; i++){
		pthread_create(&clienti[i], &attr, cliente, (void*) m);
	}
	
	pthread_create(&thread_stampa, &attr, visualizzatore, (void*) m);
	
	//Attesa terminazione threads
	for(i = 0; i < NUMCLIENTI; i++)
		pthread_join(clienti[i], NULL);
		
	pthread_join(thread_stampa, NULL);
	
	//Deallocazione strutture di utilità
	distruggi(m);
	free(m);
	
	return 0;
}
