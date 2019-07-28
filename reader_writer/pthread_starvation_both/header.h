#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

#define NUMTHRDS 10

typedef long msg;

extern pthread_mutex_t mutex;//mutex d'ingresso al monitor
extern pthread_cond_t ok_scrittura_cond;
extern pthread_cond_t ok_lettura_cond;

void* lettore(void*);
void* scrittore(void*);

void inizioLettura();
void fineLettura();
void inizioScrittura();
void fineScrittura();

#endif /* __HEADER_H__ */
