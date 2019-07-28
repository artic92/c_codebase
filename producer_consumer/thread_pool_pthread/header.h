#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

#define LIBERO 1
#define OCCUPATO 2
#define IN_USO 3

#define N 2
#define NUMTHRDS 8

extern pthread_mutex_t mutex;
extern pthread_cond_t ok_produzione_cond;
extern pthread_cond_t ok_consumo_cond;

typedef long msg;

typedef struct{
	int* stato;
	msg* buffer;
} shared;

int inizioConsumo(int *);
void* consumo(void *);
void fineConsumo(int *, int);
int inizioProduzione(int *);
void* produzione(void *);
void fineProduzione(int *, int);

#endif /* __HEADER_H__ */

