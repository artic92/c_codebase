#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

#define NUMCLIENTI 4
#define NUMSTAMPE 10

//Tempo impiegato: 1h:35m

int main (int argc, char *argv[])
{
	int i, k, status;
	pid_t pid;

	//Allocazione in shm della struttura di sincronizzazione
	key_t chiave = IPC_PRIVATE;
	int id_shared = shmget(chiave, sizeof(struct Monitor), IPC_CREAT|0664);
	struct Monitor* m;
	m = (struct Monitor*)(shmat(id_shared, NULL, 0));

	//Inizializzazione della struttura di sincronizzazione
	inizializza(m);

	//Creazione processi clienti
	for(i = 0; i < NUMCLIENTI; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Cliente\n", getpid());
			for(k = 0; k < 3; k++){
				sleep(1);
				int copia = affitta(m, k+1);
				printf("<%d> - Richiesta della copia %d del film %d\n\n", getpid(), copia, k+1);
				sleep(1);
				printf("<%d> - Restituzione della copia %d del film %d\n\n", getpid(), copia, k+1);
				restituisci(m, k+1, copia);
			}
			_exit(0);
		}
	}

	//Creazione processo visualizzatore
	pid = fork();
	if(pid == 0){
		printf("<%d> - Visualizzatore\n", getpid());
		sleep(1);
		for(k = 0; k < NUMSTAMPE; k++){
			stampa(m);
			sleep(1);
		}
		_exit(0);
	}

	//Attesa terminazione processi
	for(i = 0; i < NUMCLIENTI + 1; i++){
		sleep(1);
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	distruggi(m);

	//Deallocazione struttura di sincronizzazione
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
