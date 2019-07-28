#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMTHRDS 5

void * printHello(void * thread_id){
	int id = *((int*) thread_id);
	printf("Sono il thread # %d\n", id);
	pthread_exit(0);
}

int main (int argc, char *argv[])
{
	int i, status;
	pthread_t threads[NUMTHRDS];

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i = 0; i < NUMTHRDS; i++){
		printf("Creating thread %d\n", i);
		pthread_create(&threads[i], &attr, printHello, (void*) &i);
	}

	for(i = 0; i < NUMTHRDS; i++){
		pthread_join(threads[i], (void**) &status);
		printf("Thread n.ro %d terminato con stato %d\n", i, status);
	}

	pthread_exit(NULL);
}

