#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

#define NUMUTENTI 10
#define NUMLETTURE 10
#define NUMSCRITTURE 20

//Tempo impiegato 1h:02m

int main (int argc, char *argv[])
{
	int i, k, status;
	pid_t pid;

	//Allocazione struttura condivisa
	LettScrittMeteo* l;
	key_t chiave = IPC_PRIVATE;
	int id_shared = shmget(chiave, sizeof(LettScrittMeteo), IPC_CREAT|0664);
	l = (LettScrittMeteo*)(shmat(id_shared, NULL, 0));

	//Inizializzazione struttra condivisa
	inizializza(l);

	//Creazione processi utenti
	for(i = 0; i < NUMUTENTI; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Utente\n", getpid());
			sleep(1);
			for(k = 0; k < NUMLETTURE; k++){
				lettore(l);
				sleep(1);
			}
			_exit(0);
		}
	}

	//Creazione processo M
	pid = fork();
	if(pid == 0){
		printf("<%d> - Processo M\n", getpid());
		sleep(1);
		for(k = 0; k < NUMSCRITTURE; k++){
			scrittore(l);
			sleep(2);
		}
		_exit(0);
	}

	//Attesa terminazione processi
	for(i = 0; i < NUMUTENTI + 1; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n\n", pid, status);
	}

	//Deallocazione struttura condivisa
	distruggi(l);

	//Deallocazione shm
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
