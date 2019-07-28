#include <stdio.h>
#include "header.h"

int main (int argc, char *argv[])
{
	int i, status;
	pthread_t threads[NUMTHRDS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&ok_scrittura_cond, NULL);
	pthread_cond_init(&ok_lettura_cond, NULL);

	pthread_attr_t attr;
	pthread_attr_init(&attr);//ATTENZIONE A QUESTA ISTRUZIONE!!!!
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i = 0; i < NUMTHRDS; i++){
		if(i%2 == 0){
			printf("Scrittore %d\n", i);
			pthread_create(&threads[i], &attr, scrittore, (void*) &i);
		}else{
			printf("Lettore %d\n", i);
			pthread_create(&threads[i], &attr, lettore, (void*) &i);
		}
	}

	for(i = 0; i < NUMTHRDS; i++){
		pthread_join(threads[i], (void**) &status);
		printf("Thread n.ro %d terminato con stato\n", i, status);
	}

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&ok_scrittura_cond);
	pthread_cond_destroy(&ok_lettura_cond);
	pthread_exit(NULL);
}
