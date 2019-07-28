#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "header.h"

#define NUMPRODALTAPRIO 1
#define NUMPRODBASSAPRIO 3
#define NUMCONSUMATORI 1

int main (int argc, char *argv[])
{
	int i;
	pthread_t produttori_alta[NUMPRODALTAPRIO], produttori_bassa[NUMPRODBASSAPRIO], consumatori[NUMCONSUMATORI];
	
	//Allocazione in memoria dinamica della struttura di sincronizzazione e inizializzazione
	PriorityProdCons* p = (PriorityProdCons*)malloc(sizeof(PriorityProdCons));
	inizializza_prod_cons(p);
	
	//Rendo i thread joinable
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//Creazione produttori alta prio
	for(i = 0; i < NUMPRODALTAPRIO; i++)
		pthread_create(&produttori_alta[i], &attr, produttore_alta_prio, (void*) p);
	
	//Creazione produttori bassa prio
	for(i = 0; i < NUMPRODBASSAPRIO; i++)
		pthread_create(&produttori_bassa[i], &attr, produttore_bassa_prio, (void*) p);
	
	//Creazione consumatori
	for(i = 0; i < NUMCONSUMATORI; i++)
		pthread_create(&consumatori[i], &attr, consumatore, (void*) p);
	
	//Attesa terminazione threads
	for(i = 0; i < NUMCONSUMATORI; i++)
		pthread_join(consumatori[i], NULL);
		
	for(i = 0; i < NUMPRODALTAPRIO; i++)
		pthread_join(produttori_alta[i], NULL);		
	
	for(i = 0; i < NUMPRODBASSAPRIO; i++)
		pthread_join(produttori_bassa[i], NULL);
		
	//Deallocazione struttura di sincronizzazione
	rimuovi_prod_cons(p);
	
	free(p);
	
	return 0;
}
