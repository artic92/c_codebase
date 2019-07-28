#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NUMTHRDS 5
#define DIM 4

//Tempo impiegato 1h:22m

int main (int argc, char *argv[])
{
	int i;
	pthread_t threads[NUMTHRDS];

	//Inizializzazione struttura stack
	Stack *s = (Stack*)malloc(sizeof(Stack));
	StackInit(s, DIM);
	//La struttura non è allocata dinamic. poichè i thread fanno parte dello stesso processo

	//Rendo i thread joinable
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Creazione threads produttori
	for(i = 0; i < NUMTHRDS; i++){
		printf("<%d> - Produttore\n", i);
		pthread_create(&threads[i], &attr, produttore, (void*) s);
	}

	//Creazione thread consumatore
	pthread_t thread;
	pthread_create(&thread, &attr, consumatore, (void*) s);
	printf("Consumatore\n");

	//Attesa terminazione threads
	for(i = 0; i < NUMTHRDS + 1; i++)
		pthread_join(threads[i], NULL);

	//Deallocazione struttura stack
	StackRemove(s);

	return 0;
}
