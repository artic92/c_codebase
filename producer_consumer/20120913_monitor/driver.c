#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

#define PROD_ALTA_PRIO 1
#define PROD_BASSA_PRIO 3
#define CONS 1
#define NUM_PROD_ALTA 3
#define NUM_PROD_BASSA 3
#define NUM_CONS 12

//Non terminato

int main (int argc, char *argv[])
{
	int i, k, status;
	pid_t pid;

	//Alloco risorsa condivisa
	key_t chiave = IPC_PRIVATE;
	int id_shared = shmget(chiave, sizeof(PriorityProdCons), IPC_CREAT|0664);
	PriorityProdCons* p = (PriorityProdCons*)(shmat(id_shared, NULL, 0));

	inizializza_prod_cons(p);

	int num_produttori = PROD_ALTA_PRIO + PROD_BASSA_PRIO, num_processi = num_produttori + CONS;

	//Creo i produttori
	for(i = 0; i < num_produttori; i++){
		pid = fork();
		if(pid == 0){
			if(i == 0){
				printf("<%d> - Produttore ad ALTA priorità\n", getpid());
				for(k = 0; k < NUM_PROD_ALTA; k++){
					produci_alta_prio(p);
					sleep(4);
				}
			}else{
				printf("<%d> - Produttore a BASSA priorità\n", getpid());
				for(k = 0; k < NUM_PROD_BASSA; k++){
					produci_bassa_prio(p);
					sleep(2);
				}
			}
			_exit(0);
		}
	}

	//Creo i/il consumatore/i
	for(i = 0; i < CONS; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Consumatore\n", getpid());
			for(k = 0; k < NUM_CONS; k++){
				consuma(p);
				sleep(1);
			}
			_exit(0);
		}
	}

	//Aspetto i processi per la terminazione
	for(i = 0; i < num_processi; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Deallocazione risorse condivise
	rimuovi_prod_cons(p);
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
