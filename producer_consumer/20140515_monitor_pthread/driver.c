#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NUMFATTORINI 3
#define NUMPIZZAIOLI 1

//Tempo impiegato 1h:22m

int main (int argc, char *argv[])
{
	int i;
	pthread_t pizzaioli[NUMPIZZAIOLI], fattorini[NUMFATTORINI];

	// Allocate memory for the Monitor struct
    struct Monitor* m = (struct Monitor*)malloc(sizeof(struct Monitor));
    if (m == NULL) {
        perror("Failed to allocate memory for Monitor");
        return 1;
    }

	//Inizializzazione struttra condivisa
	inizializzaMonitor(m);

	//Rendo i threads joinable
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Creazione threads fattorini
	for(i = 0; i < NUMFATTORINI; i++){
		pthread_create(&fattorini[i], &attr, fattorino, (void*) m);
	}

	//Creazione threads pizzaioli
	for(i = 0; i < NUMPIZZAIOLI; i++){
		pthread_create(&pizzaioli[i], &attr, pizzaiolo, (void*) m);
	}

	//Attesa threads fattorini
	for(i = 0; i < NUMFATTORINI; i++)
		pthread_join(fattorini[i], NULL);

	//Attesa threads pizzaioli
	for(i = 0; i < NUMPIZZAIOLI; i++)
		pthread_join(pizzaioli[i], NULL);

	//Deallocazione struttura condivisa
	distruggiMonitor(m);

	return 0;
}
