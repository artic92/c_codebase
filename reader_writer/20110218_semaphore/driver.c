#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"

#define NUMPREN 10

//Tempo impiegato 1h:51m

int main (int argc, char *argv[])
{
	int i, k;
	pid_t pid;

	//Creazione semafori
	key_t chiaveSem = ftok(PATH, CHARS);
	int id_sem = semget(chiaveSem, 2, IPC_CREAT|0664);

	//Inizializzazione semafori
	semctl(id_sem, MUTEX, SETVAL, 1);
	semctl(id_sem, SPAZIO_DISP, SETVAL, 80);

	//Allocazione in shared memory del vettore dei posti, +1 per memorizzare la disponibilità
	Posto* p;
	int* disponibilita;
	key_t chiaveShm = ftok(PATH, CHARSH);
	int id_shared = shmget(chiaveShm, NUMPOSTI*sizeof(Posto) + sizeof(int), IPC_CREAT|0664);
	disponibilita = (int*)(shmat(id_shared, NULL, 0));
	(*disponibilita) = NUMPOSTI;
	p = (Posto*)(disponibilita + 1);

	//Inizializzazione vettore di stato
	for(i = 0; i < NUMPOSTI; i++){
		p[i].id_cliente = 0;
		p[i].stato = LIBERO;
	}

	//Creazione processi clienti
	for(i = 0; i < NUMCLIENTI; i++){
		pid = fork();
		if(pid == 0){
			for(k = 0; k < NUMPREN; k++)//Effetta NUMPREN prenotazioni
				prenotazione(id_sem, p, disponibilita);
			_exit(0);
		}
	}

	//Generazione Visualizzatore
	pid = fork();
	if(pid == 0){
		execvp("./visualizzatore", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	//Attesa di tutti i processi, compreso il visualizzatore
	for(i = 0; i < NUMCLIENTI + 1; i++){
		wait(NULL);
	}

	//Deallocazione strutture condivise
	semctl(id_sem, 0, IPC_RMID, 0);
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
