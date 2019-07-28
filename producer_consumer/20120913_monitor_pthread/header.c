#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

#define TOTPRODALTAPRIO 3
#define TOTPRODBASSAPRIO 3
#define TOTCONSUMAZIONI 12

void* produttore_alta_prio(void * m){
	PriorityProdCons* p = (PriorityProdCons*) m;
	int i;
	
	for(i = 0; i < TOTPRODALTAPRIO; i++){
		produci_alta_prio(p);
		sleep(2);
	}	
	
	pthread_exit(NULL);
}

void* produttore_bassa_prio(void * m){
	PriorityProdCons* p = (PriorityProdCons*) m;
	int i;
	
	for(i = 0; i < TOTPRODBASSAPRIO; i++){
		produci_bassa_prio(p);
		sleep(1);
	}	
	
	pthread_exit(NULL);
}

void* consumatore(void *m){
	PriorityProdCons* p = (PriorityProdCons*) m;
	int i;

	for(i = 0; i < TOTCONSUMAZIONI; i++){
		consuma(p);
		sleep(1);
	}	
	
	pthread_exit(NULL);
}

void inizializza_prod_cons(PriorityProdCons * p){
	p->testa = 0;
	p->coda = 0;
	p->count = 0;
	p->coda_produttori_alta_prio = 0;
	p->coda_produttori_bassa_prio = 0;
	pthread_mutex_init(&(p->mutex), NULL);
	pthread_cond_init(&(p->ok_produzione_alta_prio), NULL);
	pthread_cond_init(&(p->ok_produzione_bassa_prio), NULL);
	pthread_cond_init(&(p->ok_consumo_cond), NULL);	
}

void rimuovi_prod_cons(PriorityProdCons * p){
	pthread_mutex_destroy(&(p->mutex));
	pthread_cond_destroy(&(p->ok_produzione_alta_prio));
	pthread_cond_destroy(&(p->ok_produzione_bassa_prio));
	pthread_cond_destroy(&(p->ok_consumo_cond));
}

void produci_alta_prio(PriorityProdCons * p){
	int valore;
	
	pthread_mutex_lock(&(p->mutex));
	//Il produttore alta prio si deve bloccare se il buffer è pieno
	while(p->count == DIM){
		p->coda_produttori_alta_prio++;
		printf("Produttore ALTA priorità in attesa\n\n");
		pthread_cond_wait(&(p->ok_produzione_alta_prio), &(p->mutex));
		p->coda_produttori_alta_prio--;
	}

	//Produzione
	srand(time(NULL));
	//Produce un valore casuale da 0 a 12
	valore = rand() % 13;
	p->buffer[p->testa] = valore;
	p->testa = (p->testa + 1) % DIM;
	(p->count)++;
	printf("Produzione ALTA priorità: %d\n\n", valore);
	
	pthread_cond_signal(&(p->ok_consumo_cond));
	pthread_mutex_unlock(&(p->mutex));
}

void produci_bassa_prio(PriorityProdCons * p){
	pthread_mutex_lock(&(p->mutex));
	//Il produttore a bassa prio si deve bloccare se il buffer è pieno o se ci sono scrittori ad alta prio 	
	while(p->count == DIM || p->coda_produttori_alta_prio > 0){
		p->coda_produttori_bassa_prio++;
		printf("Produttore BASSA priorità in attesa\n\n");
		pthread_cond_wait(&(p->ok_produzione_bassa_prio), &(p->mutex));
		p->coda_produttori_bassa_prio++;
	}
	
	//Produzione
	srand(time(NULL));
	//Produce un valore casuale da 13 a 25
	int valore = (rand() % 13) + 13;
	p->buffer[p->testa] = valore;
	p->testa = (p->testa + 1) % DIM;
	(p->count)++;
	printf("Produzione BASSA priorità: %d\n\n", valore);
	
	pthread_cond_signal(&(p->ok_consumo_cond));
	pthread_mutex_unlock(&(p->mutex));
}

void consuma(PriorityProdCons * p){
	pthread_mutex_lock(&(p->mutex));
	//Il consumatore si deve bloccare se il buffer è vuoto
	while(p->count == 0)
		pthread_cond_wait(&(p->ok_consumo_cond), &(p->mutex));
	
	//Consumo
	int valore = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	(p->count)--;
	printf("Consumo valore: %d\n\n", valore);
	
	if(p->coda_produttori_alta_prio > 0){
		printf("Consumatore risveglia produttore alta priorità\n\n");
		pthread_cond_signal(&(p->ok_produzione_alta_prio));
	}else if(p->coda_produttori_bassa_prio > 0){
		printf("Consumatore risveglia produttore bassa priorità\n\n");
		pthread_cond_signal(&(p->ok_produzione_bassa_prio));
	}
	pthread_mutex_unlock(&(p->mutex));
}
