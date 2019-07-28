#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include "header.h"

#define NUMPUSH 4
#define NUMPOP 10

pthread_mutex_t mutex;
pthread_cond_t ok_push_cond;
pthread_cond_t ok_pop_cond;

void StackInit(Stack * s, int dim){
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&ok_push_cond, NULL);
	pthread_cond_init(&ok_pop_cond, NULL);
	s->dati = malloc(dim*sizeof(Elem));
	s->dim = dim;
	s->top = 0;
}

void StackRemove(Stack * s){
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&ok_push_cond);
	pthread_cond_destroy(&ok_pop_cond);
	free(s->dati);
}

void StackPush(Stack * s, Elem e){
	pthread_mutex_lock(&mutex);
	while(StackSize(s) == s->dim)
		pthread_cond_wait(&ok_push_cond, &mutex);

	//Il top punta al primo elemento VUOTO
	s->dati[s->top] = e;
	printf("\nElemento inserito %d\n", e);
	(s->top)++;

	pthread_cond_signal(&ok_pop_cond);
	pthread_mutex_unlock(&mutex);
}

Elem StackPop(Stack * s){
	pthread_mutex_lock(&mutex);
	while(StackSize(s) == 0)
		pthread_cond_wait(&ok_pop_cond, &mutex);

	(s->top)--;
	Elem val = s->dati[s->top];
	printf("\nElemento estratto: %d\n", val);

	pthread_cond_signal(&ok_push_cond);
	pthread_mutex_unlock(&mutex);
	return val;
}

int StackSize(Stack * s){
	return s->top;
}

void* produttore(void* s){
	Stack* stk = (Stack*) s;
	int i;

	srand(time(NULL) + getpid());

	for(i = 0; i < NUMPUSH; i++){
		Elem val = (rand() % 10) + 1;//Numero casuale da 0 a 10
		StackPush(stk, val);
		sleep(1);
	}

	pthread_exit(NULL);
}

void* consumatore(void* s){
	Stack* stk = (Stack*) s;
	int i;
	Elem elem1, elem2, somma;

	srand(time(NULL) + getpid());

	for(i = 0; i < NUMPOP; i++){
		sleep(1);
		elem1 = StackPop(stk);
		elem2 = StackPop(stk);
		somma = elem1+elem2;
		printf("\nSomma elementi estratti (%d, %d): %d\n", elem1, elem2, somma);
	}

	pthread_exit(NULL);
}
