#ifndef HEADER_H
#define HEADER_H

#define OK_SCRITTURA 0
#define OK_LETTURA 1
#define NUM_CONDITIONS 2

#include "monitor.h"

typedef long msg;

typedef struct{
	int occupato;
	int num_lettori;
} cond;

void inizioLettura(Monitor*, cond*);
void lettura(Monitor*, cond*, msg*);
void fineLettura(Monitor*, cond*);
void inizioScrittura(Monitor*, cond*);
void scrittura(Monitor*, cond*, msg*);
void fineScrittura(Monitor*, cond*);

#endif
