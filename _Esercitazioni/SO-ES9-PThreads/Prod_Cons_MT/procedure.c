			/*-----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "header.h"

/*variabili di utilità per le condition*/
int ok_produzione = 1;
int ok_consumo = 0;

/*buffer condiviso tra produttori e consumatori*/
msg mess;

void InizioConsumo(){
	pthread_mutex_lock(&mutex);
	while (ok_consumo==0)
		pthread_cond_wait(&ok_cons_cv,&mutex);
}

void FineConsumo(){

        ok_produzione = 1;
	ok_consumo = 0;
	pthread_cond_signal(&ok_prod_cv);
	pthread_mutex_unlock(&mutex);
}


void InizioProduzione(){

	pthread_mutex_lock(&mutex);
	while (ok_produzione==0)
		pthread_cond_wait(&ok_prod_cv,&mutex);


}

void FineProduzione (){

	ok_consumo = 1;
	ok_produzione = 0;
	pthread_cond_signal(&ok_cons_cv);
	pthread_mutex_unlock(&mutex);
}

void *Produttore(void* id) {
		InizioProduzione();
	        /*********Produzione********/
                struct timeval t1;
                struct timezone t2;
                gettimeofday(&t1,&t2);//valore diverso ad ogni produzione
                msg val =t1.tv_usec;
	        int my_id = (int) id;
                mess = val;
	        printf("Thread#%d Valore PRODOTTO = <%d> \n", my_id, mess);
		FineProduzione();
		pthread_exit(NULL);
}

void * Consumatore (void * id) {

		InizioConsumo();
		int my_id = (int) id;
		/*********Consumo********/
	        printf("Thread #%d, valore CONSUMATO = <%d> \n", my_id, mess);
		FineConsumo();
		pthread_exit(NULL);
}


