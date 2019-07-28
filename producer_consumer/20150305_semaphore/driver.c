#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

//Tempo impiegato: 1h:46m

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	//Allocazione set di semafori per la sincronizzazione
	key_t chiave_sem = ftok(PATH, CHARSEM);
	int id_sem = semget(chiave_sem, 2, IPC_CREAT|0664);

	//Allocazione delle shm contenenti i buffers e relativo attacco
	key_t chiave_shm1 = ftok(PATH, CHARSHM1);
	int id_shm1 = shmget(chiave_shm1, sizeof(buffer), IPC_CREAT|0664);
	buffer* msg1 = (buffer*)(shmat(id_shm1, NULL, 0));

	key_t chiave_shm2 = ftok(PATH, CHARSHM2);
	int id_shm2 = shmget(chiave_shm2, sizeof(buffer), IPC_CREAT|0664);
	buffer* msg2 = (buffer*)(shmat(id_shm2, NULL, 0));

	//Inizializzazione buffers nello stato LIBERO
	msg1->valore = 0;
	msg1->stato = LIBERO;

	msg2->valore = 0;
	msg2->stato = LIBERO;

	//Inizializzazione semafori, inizialmente i buffer sono vuoti
	semctl(id_sem, SPAZIO_DISP, SETVAL, 2);
	semctl(id_sem, MEX_DISP, SETVAL, 0);

	//Creazione processo produttore
	pid = fork();
	if(pid == 0){
		printf("<%d> Produttore\n\n", getpid());
		sleep(1);
		execlp("./produttore", "./produttore", NULL);

		//Se la exec va a buon fine queste istruzioni non verranno mai eseguite
		perror("Exec fallita\n\n");
		_exit(0);
	}

	//Creazione processo consumatore
	pid = fork();
	if(pid == 0){
		printf("<%d> Consumatore\n\n", getpid());
		sleep(1);
		execlp("./consumatore", "./consumatore", NULL);

		//Se la exec va a buon fine queste istruzioni non verranno mai eseguite
		perror("Exec fallita\n\n");
		_exit(0);
	}

	//Attesa terminazione processi
	for(i = 0; i < 2; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n\n", pid, status);
	}

	//Deallocazione shm
	shmctl(id_shm1, IPC_RMID, NULL);
	shmctl(id_shm2, IPC_RMID, NULL);

	//Deallocazione semafori
	semctl(id_sem, 0, IPC_RMID, NULL);

	return 0;
}


