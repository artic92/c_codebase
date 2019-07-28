#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

#define NUMCAPITRENO 4
#define NUMVIAGGIATORI 10

//Tempo impiegato 2h:25m
//ATTENZIONE: i messaggi a video sono disordinati perchè non eseguiti in mutua esclusione

int main (int argc, char *argv[])
{
	int i;
	pthread_t capitreno[NUMCAPITRENO], viaggiatori[NUMVIAGGIATORI];
	
	struct monitor_treno* treni = (struct monitor_treno*)malloc(NUMCAPITRENO*sizeof(struct monitor_treno));
	
	srand(time(NULL));
	
	//Inizializzazione monitor per i singoli treni
	for(i = 0; i < NUMCAPITRENO; i++){
		inizializza(&treni[i]);
	}
		
	//Rendo i thread JOINABLE
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Creazione thread capotreno
	for(i = 0; i < NUMCAPITRENO; i++){
		pthread_create(&capitreno[i], &attr, capotreno, (void*) &treni[i]);
	}
	
	//Creazione thread viaggiatore
	for(i = 0; i < NUMVIAGGIATORI; i++){
		//Treno è un valore casuale da 0 a 3
		pthread_create(&viaggiatori[i], &attr, viaggiatore, (void*) &treni[(rand() % 4)]);
	}
	
	//Attesa terminazione threads capotreno
	for(i = 0; i < NUMCAPITRENO; i++)
		pthread_join(capitreno[i], NULL);
	
	//Attesa terminazione threads viaggiatori
	for(i = 0; i < NUMVIAGGIATORI; i++)
		pthread_join(viaggiatori[i], NULL);
	
	//Deallocazione dei monitor
	for(i = 0; i < NUMCAPITRENO; i++){
		rimuovi(&treni[i]);
	}
	
	pthread_attr_destroy(&attr);
	free(treni);
	
	return 0;
}
