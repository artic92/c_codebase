#ifndef __HEADER_H__
#define __HEADER_H__

#define PATH "."
#define CHARREQ 'a'
#define CHARREP 'b'

#define NUMCLIENT 3

#include <pthread.h>

//Mutex per la mutua esclusione delle risposte
extern pthread_mutex_t mutex;

typedef struct
{
	long pid;
	int valore1;
	int valore2;
} Messaggio;

void client();
void* calcolaProdotto(void* r);

#endif /* __HEADER_H__ */

