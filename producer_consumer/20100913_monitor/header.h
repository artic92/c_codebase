#ifndef __HEADER_H__
#define __HEADER_H__

#include "monitor.h"

#define NELEM 100

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct{
	unsigned int id_fornitore;
	unsigned int stato;
} Scaffale;

//Definizione delle variabili_condition per il monitor

#define NUMCOND 2
#define OK_FORNITURA 0
#define OK_ACQUISTO 1

typedef struct{
	int livello_scorte;
} Cond;

int inizioFornitura(Monitor*, Cond*, Scaffale*);
void fornitura(Monitor*, Cond*, Scaffale*);
void fineFornitura(Monitor*, Cond*);
int inizioAcquisto(Monitor*, Cond*, Scaffale*);
void acquisto(Monitor*, Cond*, Scaffale*);
void fineAcquisto(Monitor*, Cond*);

#endif /* __HEADER_H__ */

