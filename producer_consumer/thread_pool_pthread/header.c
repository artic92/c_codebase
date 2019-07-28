#include <sys/time.h>
#include <stdio.h>
#include "header.h"

pthread_mutex_t mutex;
pthread_cond_t ok_produzione_cond;
pthread_cond_t ok_consumo_cond;

//ATTENZIONE queste due istruzioni sono buone solo perchè usi thread che condividono lo spazio di inidirizzamento
//del processo. In un applicazione MULTIPROCESSO queste variabili sarrebbero state copiate per ciascun processo
//e non sarebbero state utili come variabili di sincronizzazione
int ok_consumo = 0;
int ok_produzione = N;

int inizioConsumo(int* ptr_s){
	int indice = 0;
	pthread_mutex_lock(&mutex);
	while(ok_consumo == 0)
		pthread_cond_wait(&ok_consumo_cond, &mutex);
	ok_consumo--;//wait(MEX_DISP)
	while(indice < N && ptr_s[indice] != OCCUPATO){
		indice++;
	}
	ptr_s[indice] = IN_USO;
	pthread_mutex_unlock(&mutex);
	return indice;
}

void* consumo(void * s){
	shared* sh = (shared*) s;
	int indice = inizioConsumo(sh->stato);
	int mex = sh->buffer[indice];
	printf("CONSUMO il valore %d in posizione %d\n", mex, indice);
	fineConsumo(sh->stato, indice);
	pthread_exit(0);
}

void fineConsumo(int * ptr_s, int indice){
	ptr_s[indice] = LIBERO;
	pthread_mutex_lock(&mutex);
	ok_produzione++;//signal(SPAZIO_DISP)
	pthread_cond_signal(&ok_produzione_cond);
	pthread_mutex_unlock(&mutex);
}

int inizioProduzione(int* ptr_s){
	int indice = 0;
	pthread_mutex_lock(&mutex);
	while(ok_produzione == 0)
		pthread_cond_wait(&ok_produzione_cond, &mutex);
	ok_produzione--;//wait(SPAZIO_DISP)
	while(indice < N && ptr_s[indice] != LIBERO)
		indice++;
	ptr_s[indice] = IN_USO;
	pthread_mutex_unlock(&mutex);
	return indice;
}

void* produzione(void * s){
	shared* sh = (shared*) s;
	int indice = inizioProduzione(sh->stato);
	struct timeval t1;
	gettimeofday(&t1, NULL);
	sh->buffer[indice] = t1.tv_usec;
	printf ("PRODUZIONE in corso di %d, indice = <%d>\n", sh->buffer[indice], indice);
	fineProduzione(sh->stato, indice);
	pthread_exit(0);
}

void fineProduzione(int * ptr_s, int indice){
	ptr_s[indice] = OCCUPATO;
	pthread_mutex_lock(&mutex);
	ok_consumo++;//signal(MEX_DISP)
	pthread_cond_signal(&ok_consumo_cond);
	pthread_mutex_unlock(&mutex);
}
