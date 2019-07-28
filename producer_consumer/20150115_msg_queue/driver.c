#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include "header.h"

//Tempo impiegato: 1h:12m

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	//Allocazione IPCS
	key_t chiave_shared = ftok(PATH, SHM);
	int id_shared = shmget(chiave_shared, 2*sizeof(int), IPC_CREAT|0664);

	key_t chiave_queue = ftok(PATH, QUEUE);
	int id_queue = msgget(chiave_queue, IPC_CREAT|0664);

	key_t chiave_sem = ftok(PATH, SEM);
	int id_sem = semget(chiave_sem, 1, IPC_CREAT|0664);

	//Creazione processi client
	for(i = 0; i < NUMCLIENT; i++){
		pid = fork();
		if(pid == 0){
			execvp("./client", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Creazione processi server
	for(i = 0; i < NUMSERVER; i++){
		pid = fork();
		if(pid == 0){
			execvp("./server", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Attesa terminazione processi
	for(i = 0; i < (NUMCLIENT + NUMSERVER); i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Deallocazione IPCS
	semctl(id_sem, 0, IPC_RMID, NULL);
	shmctl(id_shared, IPC_RMID, 0);
	msgctl(id_queue, IPC_RMID, NULL);

	return 0;
}
