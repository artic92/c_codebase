#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

#define NUMDVD 6

#define DISPONIBILE 0
#define AFFITTATO 1

struct DVD {
	int identificativo_film; 
	int identificativo_copia; 
	int stato;
};

struct Monitor {
	struct DVD dvd[NUMDVD]; // vettore di DVD da gestire
	int ok_affitto[3]; //#0: DVD1, #1: DVD2, #2: DVD3
	
	//Mutex per l'ingresso nel monitor e variabili conditions
	pthread_mutex_t mutex;
	pthread_cond_t ok_affitto_cond[3]; //#0: DVD1, #1: DVD2, #2: DVD3
};

void inizializza(struct Monitor*);
void distruggi(struct Monitor*);

int affitta(struct Monitor * m, int id_film);
void restituisci(struct Monitor * m, int id_film, int id_copia);
void stampa(struct Monitor * m);

void* cliente(void *);
void* visualizzatore(void *);

#endif /* __HEADER_H__ */

