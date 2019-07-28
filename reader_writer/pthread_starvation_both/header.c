#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include "header.h"

pthread_mutex_t mutex;//mutex d'ingresso al monitor
pthread_cond_t ok_scrittura_cond;
pthread_cond_t ok_lettura_cond;

int occupato = 0;
int num_lettori = 0;
int coda_lettori = 0;
int coda_scrittori = 0;
msg mex = 0;

void inizioLettura(){
	pthread_mutex_lock(&mutex);

	while(occupato == 1 && num_lettori == 0){
		coda_lettori++; //Variabile diversa da num_lettori altrimenti i lettori non si bloccano
				//quando ci sono gli scrittori
		printf("Scrittura in corso; aumento coda_lettori e blocco (%d)\n\n", coda_lettori);
		pthread_cond_wait(&ok_lettura_cond, &mutex);
		coda_lettori--;  //NON CAPISCO QND VIENE ESEGUITA!!!!!!!!!
		printf("Riduzione coda_lettori (%d)\n\n", coda_lettori);
	}

	num_lettori++;

	if(num_lettori == 1)
		occupato = 1;

	pthread_cond_signal(&ok_lettura_cond);

	pthread_mutex_unlock(&mutex);
}

void* lettore(void* d){
	sleep(1);
	int id_t = *((int*)d);
	inizioLettura();
	sleep(1);
	printf("<%d> - LETTURA valore %d, # di lettori %d\n", id_t, mex, num_lettori);

	fineLettura();
	pthread_exit(0);
}

void fineLettura(){
	pthread_mutex_lock(&mutex);

	num_lettori--;

	if(num_lettori == 0){
		occupato = 0;
		pthread_cond_signal(&ok_scrittura_cond);
	}

	pthread_mutex_unlock(&mutex);
}

void inizioScrittura(){
	pthread_mutex_lock(&mutex);

	while(occupato == 1){
		coda_scrittori++;
		pthread_cond_wait(&ok_scrittura_cond, &mutex);
		coda_scrittori--; //NON CAPISCO QND VIENE ESEGUITA!!!!!!!!!
	}

	occupato = 1;

	pthread_mutex_unlock(&mutex);
}

void* scrittore(void* d){
	int id_t = *((int*)d);
	inizioScrittura();
	sleep(2);
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1, &t2);
	msg val = t1.tv_usec;
	mex = val;
	printf("<%d> - SCRITTURA valore %d\n", id_t, val);

	fineScrittura();
	pthread_exit(0);
}

void fineScrittura(){
	pthread_mutex_lock(&mutex);

	if(coda_scrittori > 0)
		pthread_cond_signal(&ok_scrittura_cond);
	else if(coda_lettori > 0)
		pthread_cond_signal(&ok_lettura_cond);

	occupato = 0;

	pthread_mutex_unlock(&mutex);
}
