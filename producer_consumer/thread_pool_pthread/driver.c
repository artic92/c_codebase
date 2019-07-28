#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "header.h"

int main (int argc, char *argv[])
{
	key_t id_status, id_buffer;
	pthread_t threads[NUMTHRDS];
	int i, stato;
	int* status;
	msg* buffer;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&ok_produzione_cond, NULL);
	pthread_cond_init(&ok_consumo_cond, NULL);
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	id_status = shmget(IPC_PRIVATE, N*sizeof(int), IPC_CREAT|0664);
	id_buffer = shmget(IPC_PRIVATE, N*sizeof(msg), IPC_CREAT|0664);
	
	status = (int*)(shmat(id_status, NULL, 0));
	buffer = (msg*)(shmat(id_buffer, NULL, 0));
	
	for(i = 0; i < N; i++)
		status[i] = LIBERO;
	
	shared s;
	s.stato = status;
	s.buffer = buffer;
	
	for(i = 0; i < NUMTHRDS; i++){
		if(i%2 == 0){
			printf("Sono il thread produttore con id %d\n", i);
			pthread_create(&threads[i], &attr, produzione, &s);
		}
		else{
			printf("Sono il thread consumatore con id %d\n", i);
			pthread_create(&threads[i], &attr, consumo, &s);
		}
	}
	
	for(i = 0; i < NUMTHRDS; i++){
		pthread_join(threads[i], (void**) &stato);
		printf("Thread %d terminato con stato %d\n", i, stato);
	}
	
	shmctl(id_status, IPC_RMID, NULL);
	shmctl(id_buffer, IPC_RMID, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&ok_produzione_cond);
	pthread_cond_destroy(&ok_consumo_cond);
	pthread_attr_destroy(&attr);
	
	return 0;
}
