#ifndef __HEADER_H__
#define __HEADER_H__

//Definizioni variabili condizioni 3, una per ogni id_film
#define NUMCOND 3

#include "monitor.h"

#define DISPONIBILE 0
#define AFFITTATO 1

#define TOTDVD 6

struct DVD {
	int identificativo_film; // un intero tra 1 e 3
	int identificativo_copia; // un intero tra 1 e 2
	int stato; // DISPONIBILE=0, oppure AFFITTATO=1
};

struct Monitor {
	struct DVD dvd[TOTDVD]; // vettore di DVD da gestire
	int ok_affitto[3];
	Monitor mon;
};

//Inizializzazione struttura di sincronizzazione
void inizializza(struct Monitor*);
//Deallocazione struttura di sincronizzazione
void distruggi(struct Monitor*);

//Restituisce l'id_copia del film id_film ponendo il suo stato in AFFITTATO, se non la trova si mette in attesa
int affitta(struct Monitor * m, int id_film);
//Cambia lo stato dell'id_copia riferito ad id_film in DISPONIBILE e risveglia un eventuale processo in attesa 
void restituisci(struct Monitor * m, int id_film, int id_copia);
//Mostra lo stato dei DVD del negozio
void stampa(struct Monitor * m);

#endif /* __HEADER_H__ */

