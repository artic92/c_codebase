#ifndef __HEADER_H__
#define __HEADER_H__

#include "monitor.h"

//Definizione variabili condizione
#define NUMCOND 2
#define OK_PRODUZIONE 0
#define OK_CONSUMO 1

typedef struct{
	int ok_produzione;
	int ok_consumo;
} Cond;

//Dimensione buffer
#define DIM 10
#define NUMRICHIESTE 5

typedef struct {
	unsigned int posizione;
	pid_t processo;
} Richiesta;

void utente(Monitor*, Cond*, Richiesta*, int*);
void schedulatore(Monitor*, Cond*, Richiesta*, int*);

#endif /* __HEADER_H__ */

