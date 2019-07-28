#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include "header.h"
#include "semafori.h"

#define NUMVOLTE 4

int main (int argc, char *argv[])
{
	printf("[%d] Server ready...\n", getpid());
	
	int i;
	
	//Allocazione IPCS
	key_t chiave_shared = ftok(PATH, SHM);
	int id_shared = shmget(chiave_shared, 2*sizeof(int), IPC_CREAT|0664);
	int* valore1 = (int*)(shmat(id_shared, NULL, 0));
	int* valore2 = valore1 + 1;
	
	key_t chiave_queue = ftok(PATH, QUEUE);
	int id_queue = msgget(chiave_queue, IPC_CREAT|0664);
	
	key_t chiave_sem = ftok(PATH, SEM);
	int id_sem = semget(chiave_sem, 1, IPC_CREAT|0664);
	
	//Inizializzazione semaforo
	semctl(id_sem, MUTEX, SETVAL, 1);
	
	Messaggio richiesta;
	
	//Ripeti NUMVOLTE volte
	for(i = 0; i < NUMVOLTE; i++){
		
		//Attendi di ricevere un messaggio
		int risp = msgrcv(id_queue, &richiesta, sizeof(Messaggio)-sizeof(long), 0, 0);
		if(risp < 0){
			perror("Errore ricezione\n\n");
			_exit(0);
		}
		
		printf("[%d] Server - Ricevuto (%d, %d) dal processo %d\n\n", getpid(), richiesta.valore1, richiesta.valore2, richiesta.processo);
		
		//Attendi 1 secondo
		sleep(1);
		
		//Accedi alla sezione critica
		Wait_Sem(id_sem, MUTEX);
		
		//Modifica i valori
		(*valore1) = richiesta.valore1;
		(*valore2) = richiesta.valore2;
		
		printf("[%d] Valori modificati (%d, %d); provenienti dal processo %d\n\n", getpid(), *valore1, *valore2, richiesta.processo);
		
		//Esci dalla sezione critica
		Signal_Sem(id_sem, MUTEX);
		
/*		printf("Sono uscito dall sez. critica %d\n", getpid());*/
		
		
	}
	return 0;
}
