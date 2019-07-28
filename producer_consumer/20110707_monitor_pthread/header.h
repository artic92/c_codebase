#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

//Definizione variabili condition per il monitor
extern pthread_mutex_t mutex;
extern pthread_cond_t ok_push_cond;
extern pthread_cond_t ok_pop_cond;

typedef int Elem;

typedef struct Stack {
	Elem * dati;// puntatore ad un vettore dinamico di elementi
	int dim;// dimensione dello Stack
	int top;
} Stack;

void StackInit(Stack * s, int dim);//alloca dinamicamente "dim" elementi
void StackRemove(Stack * s);//dealloca gli elementi
void StackPush(Stack * s, Elem e); //inserimento nello Stack
Elem StackPop(Stack * s);//estrazione dallo Stack
int StackSize(Stack * s);//ritorna il numero attuale di elementi
void* produttore(void*);
void* consumatore(void*);

#endif /* __HEADER_H__ */

