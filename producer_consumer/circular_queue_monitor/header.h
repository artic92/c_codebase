#ifndef HEADER_H
#define HEADER_H

#define OK_PRODUZIONE 0
#define OK_CONSUMO 1
#define NUM_CONDITIONS 2
#define DIM 2

#include "monitor.h"


typedef long msg;

typedef struct{
	int ok_produzione;
	int ok_consumo;
}cond;

void inizioProduzione(Monitor* m, cond* c);
void produttore(Monitor*, cond*, msg*, int*);
void fineProduzione(Monitor* m, cond* c);
void inizioConsumo(Monitor* m, cond* c);
void consumatore(Monitor*, cond*, msg*, int*);
void fineConsumo(Monitor* m, cond* c);

#endif
