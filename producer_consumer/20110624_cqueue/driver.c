#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"

#define NUMUTENTI 5

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	//Allocazione del buffer
	key_t chiave = IPC_PRIVATE;
	int id_buffer = shmget(chiave, DIM*sizeof(Richiesta) + 2*sizeof(int), IPC_CREAT|0664);
	int* testa = (int*)(shmat(id_buffer, NULL, 0));
	int* coda = testa + 1;
	Richiesta* buffer = (Richiesta*)(coda + 1);

	//Allocazione del monitor e delle variabili condition in memoria condivisa
	int id_mon = shmget(chiave, sizeof(Monitor), IPC_CREAT|0664);
	Monitor* m = (Monitor*)(shmat(id_mon, NULL, 0));

	int id_cond = shmget(chiave, sizeof(Cond), IPC_CREAT|0664);
	Cond* c = (Cond*)(shmat(id_cond, NULL, 0));

	//Inizializzazione del monitor e delle variabili condition
	init_monitor(m, NUMCOND);
	c->ok_produzione = DIM;
	c->ok_consumo = 0;

	//Creazione processi utenti
	for(i = 0; i < NUMUTENTI; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Utente\n", getpid());
			utente(m, c, buffer, coda);
			_exit(0);
			}
	}

	//Creazione processo Scheduler
	pid = fork();
	if(pid == 0){
		printf("<%d> - Scheduler\n", getpid());
		schedulatore(m, c, buffer, testa);
		_exit(0);
	}

	//Attesa dei processi
	for(i = 0; i < NUMUTENTI + 1; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Deallocazione monitor
	remove_monitor(m);

	//Deallocazione risorse condivise
	shmctl(id_cond, IPC_RMID, NULL);
	shmctl(id_mon, IPC_RMID, NULL);
	shmctl(id_buffer, IPC_RMID, NULL);

	return 0;
}
