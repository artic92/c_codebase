			/*-----HEADER FILE-----*/
#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

/*messaggio*/
typedef long msg;


/*conditions*/
pthread_cond_t ok_prod_cv;
pthread_cond_t ok_cons_cv;

/*mutex*/
pthread_mutex_t mutex;

#define NUM_THREADS	10

void * Consumatore(void *);
void * Produttore(void *);

void InizioConsumo();
void InizioProduzione();
void FineConsumo();
void FineProduzione();

#endif
