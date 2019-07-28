#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

#define NUMVIAGGIATORI 10
#define NUMTRENI 4
#define NUMCONSULTI 3
#define NUMSCRITTURE 10

//Tempo impiegato: 1h:18m

int main (int argc, char *argv[])
{
	int i, k, status;
	pid_t pid;

	//Allocazione memoria condivisa per la struttura di sincronizzazione
	key_t chiave = IPC_PRIVATE;
	int id_shared = shmget(chiave, NUMTRENI*sizeof(struct monitor_treno), IPC_CREAT|0664);
	struct monitor_treno* t = (struct monitor_treno*)(shmat(id_shared, NULL, 0));

	//Inizializzazione struttra condivisa
	for(i = 0; i < NUMTRENI; i++){
		inizializza(&t[i]);
	}

	int treno, posizione, sleep_time;
	//Creazione processi viaggiatori
	for(i = 0; i < NUMVIAGGIATORI; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Viaggiatore\n", getpid());
			sleep(1);
			srand(time(NULL)+getpid());
			for(k = 0; k < NUMCONSULTI; k++){
				//Valore casuale tra 0 e 3
				treno = rand() % 4;
				posizione = leggi_stazione(&t[treno]);
				printf("<%d> - Il treno %d si trova alla stazione %d\n\n", getpid(), treno+1, posizione);
				sleep_time = rand() % 7;
				printf("<%d> - Attendo %d secondi\n\n", getpid(), sleep_time);
				sleep(sleep_time);
			}
			_exit(0);
		}
	}

	//Creazione processi capotreno
	for(i = 0; i < NUMTRENI; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Capotreno\n", getpid());
			sleep(1);
			for(k = 0; k < NUMSCRITTURE; k++){
				printf("Capotreno del treno %d: modificata posizione treno %d in posizione %d\n\n", i+1, i+1, k+1);
				scrivi_stazione(&t[i], k+1);
				sleep(3);
			}
			_exit(0);
		}
	}

	//Attesa terminazione processi
	for(i = 0; i < NUMTRENI + NUMVIAGGIATORI; i++){
		sleep(1);
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n\n", pid, status);
	}

	//Distruzione strutture di sicronizzazione
	for(i = 0; i < NUMTRENI; i++){
		rimuovi(&t[i]);
	}

	//Deallocazione memoria condivisa
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
