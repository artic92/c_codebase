#ifndef __HEADER_H__
#define __HEADER_H__

#define NUMCOND 3
#define OK_CONSUMO 0
#define OK_PRODUZIONE_ALTAP 1
#define OK_PRODUZIONE_BASSAP 2
#define DIM 3

#include "monitor.h"

typedef struct {
	int buffer[DIM];
	int testa; 
	int coda;
	int ok_consumo;
	int ok_produzione;
	int num_produttori_alta_prio;
	Monitor m;
} PriorityProdCons;

void inizializza_prod_cons(PriorityProdCons * p);
void produci_alta_prio(PriorityProdCons * p);
void produci_bassa_prio(PriorityProdCons * p);
void consuma(PriorityProdCons * p);
void rimuovi_prod_cons(PriorityProdCons * p);

#endif /* __HEADER_H__ */

