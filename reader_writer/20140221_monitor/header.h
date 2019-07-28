#ifndef __HEADER_H__
#define __HEADER_H__

#include "monitor.h"

#define NUMCOND 2

#define OK_LETTURA 0
#define OK_SCRITTURA 1

struct monitor_treno {
	int stazione;// riporta la stazione in cui si trova il treno (intero tra 0 e 10)
	int num_lettori;
	int occupato;
	Monitor mon;
};

void inizializza(struct monitor_treno * m);
void rimuovi(struct monitor_treno * m);
//Permette a più lettori di consultare la posizione del treno
int leggi_stazione(struct monitor_treno * m);
//Permette ad un solo capotreno per volta di modificare la posizione del treno
void scrivi_stazione(struct monitor_treno * m, int stazione);

#endif /* __HEADER_H__ */

