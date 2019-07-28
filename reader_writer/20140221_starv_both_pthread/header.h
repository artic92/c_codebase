#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

struct monitor_treno {
	int id_treno;
	int stazione;// riporta la stazione in cui si trova il treno (intero tra 0 e 10)
	int num_lettori;
	int occupato;
	int coda_lettori;
	int coda_scrittori;
	pthread_mutex_t mutex;
	pthread_cond_t ok_lettura_cond;//Se c'è lo scrittore nel monitor, i lettori si devono bloccare
	pthread_cond_t ok_scrittura_cond;//Se ci sono lettori, lo scrittore si deve bloccare
};

void inizializza(struct monitor_treno * m);
void rimuovi(struct monitor_treno * m);

int leggi_stazione(struct monitor_treno * m);
void scrivi_stazione(struct monitor_treno * m, int stazione);
void* capotreno(void*);
void* viaggiatore(void*);

#endif /* __HEADER_H__ */

