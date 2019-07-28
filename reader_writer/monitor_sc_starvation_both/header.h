#ifndef __HEADER_H__
#define __HEADER_H__

#include "monitor.h"

#define OK_LETTURA 0
#define OK_SCRITTURA 1

#define NUMCOND 2

typedef long msg;

typedef struct{
	int occupato;
	int num_lettori;
} cond;

void inizioLettura(Monitor*, cond*);
void lettore(Monitor*, cond*, msg*);
void fineLettura(Monitor*, cond*);
void inizioScrittura(Monitor*, cond*);
void scrittore(Monitor*, cond*, msg*);
void fineScrittura(Monitor*, cond*);

#endif /* __HEADER_H__ */

