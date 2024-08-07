/*****PROBLEMA DEI PRODUTTORI-CONSUMATORI: soluzione mediante Threads*****/
/*Il programma sincronizza processi produttori e processi consumatori nell'accesso ad una variabile condivisa utilizzando i Pthreads.

 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "header.h"


//#define NUM_THREADS	10


int main(){
     int k;
     pthread_attr_t attr;
     pthread_t threads[NUM_THREADS];

     //inzializzazione mutex e condition
     pthread_mutex_init(&mutex,NULL);
     pthread_cond_init(&ok_prod_cv,NULL);
     pthread_cond_init(&ok_cons_cv,NULL);

     //impostazione dei thread come joinable
     pthread_attr_init(&attr);
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

     //generazione di scrittori e lettori
     for (k=0;k<NUM_THREADS; k++) {

	if (k%2)  {
	     printf("Sono il thread Consumatore, id %d \n",k);
	     pthread_create(&threads[k], &attr, Consumatore, (void *) k);
	   }else {
	     sleep(1);
             printf("Sono il thread Produttore, id %d \n",k);
	     pthread_create(&threads[k], &attr, Produttore, (void *) k);	
	    }
	}

     for (k=0; k<NUM_THREADS;k++){
       pthread_join(threads[k],NULL);
       printf ("Thread n.ro %d terminato\n ",k);
     }

     /*deallocazione risorse*/	
     pthread_attr_destroy(&attr);
     pthread_mutex_destroy(&mutex);
     pthread_cond_destroy(&ok_prod_cv);
     pthread_cond_destroy(&ok_cons_cv);
     pthread_exit(0);
}
