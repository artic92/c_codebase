#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

#define DIM 3

typedef struct {
	int buffer[DIM];
	int testa; 
	int coda;
	int count;
	int coda_produttori_alta_prio;
	int coda_produttori_bassa_prio;
	pthread_mutex_t mutex;
	pthread_cond_t ok_produzione_alta_prio;
	pthread_cond_t ok_produzione_bassa_prio;
	pthread_cond_t ok_consumo_cond;
} PriorityProdCons;

void inizializza_prod_cons(PriorityProdCons * p);
void rimuovi_prod_cons(PriorityProdCons * p);
void produci_alta_prio(PriorityProdCons * p);
void produci_bassa_prio(PriorityProdCons * p);
void consuma(PriorityProdCons * p);

void* produttore_alta_prio(void *);
void* produttore_bassa_prio(void *);
void* consumatore(void *);

#endif /* __HEADER_H__ */

