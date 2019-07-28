#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include "header.h"

#define NUMSTUDENTI  10
#define NUMDOCENTI 1

//Tempo impiegato 1h:41m

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;
	int num_processi = NUMDOCENTI + NUMSTUDENTI;

	//Allocazione struttre condivise
	key_t chiave_sem = ftok(PATH, CHARS), chiave_appelli = ftok(PATH, CHARA);
	int id_sem = semget(chiave_sem, NUMSEM, IPC_CREAT|0664);
	int id_shared = shmget(chiave_appelli, sizeof(Appello), IPC_CREAT|0664);
	Appello* a = (Appello*)(shmat(id_shared, NULL, 0));

	//Inizializzazione semafori e shm
	semctl(id_sem, SYNC, SETVAL, 1);
	semctl(id_sem, MUTEX, SETVAL, 1);
	strcpy(a->data_appello,"01/01/2000");
	a->num_prenotati = 0;
	a->num_studenti = 0;

	//Creazione processi studenti
	for(i = 0; i < NUMSTUDENTI; i++){
		pid = fork();
		if(pid == 0){
			execvp("./studente", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Creazione processi docenti
	for(i = 0; i < NUMDOCENTI; i++){
		pid = fork();
		if(pid == 0){
			execvp("./docente", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Attesa terminazione processi
	for(i = 0; i < num_processi; i++){
		pid = wait(&status);
/*		printf("Processo %d terminato con stato %d\n", pid, status);*/
	}

	//Dellocazione strutture condivise
	semctl(id_sem, 0, IPC_RMID, NULL);
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
