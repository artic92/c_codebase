#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NUMRICHIEDENTI 4
#define NUMELABORATORI 2

//Tempo impiegato: 2h:12m
//ATTENZIONE: quando i alcuni richiedenti richiedono un numero sbagliato di operandi gli elaboratori saranno in un attesa indefinita di una richiesta che non avverrà mai, quindi non consentendo al programma di terminare; tutto perchè non so cm fare in modo che i valori casuali si trovano tra 2 e 4.

int main (int argc, char *argv[])
{
	int i;
	pthread_t richiedenti[NUMRICHIEDENTI], elaboratori[NUMELABORATORI];

	// Allocate memory for the Monitor struct
    MonitorElaborazioni* m = (MonitorElaborazioni*)malloc(sizeof(MonitorElaborazioni));
    if (m == NULL) {
        perror("Failed to allocate memory for Monitor");
        return 1;
    }

	//Inizializzazione struttura di sincronizzazione
	inizializza(m);

	//Rendo i threads joinable
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Creazione threads
	for(i = 0; i < NUMRICHIEDENTI; i++){
			//Creo i threads richiedenti
			pthread_create(&richiedenti[i], &attr, richiedente, (void*) m);
	}

	for(i = 0; i < NUMELABORATORI; i++){
			//Creo i threads elaboratori
			pthread_create(&elaboratori[i], &attr, elaborazione, (void*) m);
	}

	//Attesa terminazione threads
	for(i = 0; i < NUMRICHIEDENTI; i++)
		pthread_join(richiedenti[i], NULL);

	for(i = 0; i < NUMELABORATORI; i++)
		pthread_join(elaboratori[i], NULL);

	//Deallocazione struttura di sincronizzazione
	distruggi(m);

	return 0;
}
