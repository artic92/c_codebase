#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"
#include "semafori.h"

#define NUMCONS 5

int main (int argc, char *argv[])
{
	int i;
	
	//Allocazione semafori per la sincronizzazione
	key_t chiave_sem = ftok(PATH, CHARSEM);
	int id_sem = semget(chiave_sem, 2, IPC_CREAT|0664);
	//Eventuale terminazione se l'ipc non viene creata
	if(id_sem < 0){
		perror("Errore semaforo!\n\n");
		_exit(0);
	}
	
	//Allocazione delle shm contenente i buffer e relativo attacco
	key_t chiave_shm1 = ftok(PATH, CHARSHM1);
	int id_shm1 = shmget(chiave_shm1, sizeof(buffer), IPC_CREAT|0664);
	//Eventuale terminazione se l'ipc non viene creata
	if(id_shm1 < 0){
		perror("Errore shared!\n\n");
		_exit(0);
	}
	buffer* msg1 = (buffer*)(shmat(id_shm1, NULL, 0));
	
	key_t chiave_shm2 = ftok(PATH, CHARSHM2);
	int id_shm2 = shmget(chiave_shm2, sizeof(buffer), IPC_CREAT|0664);
		if(id_shm2 < 0){
		perror("Errore shared!\n\n");
		_exit(0);
	}
	buffer* msg2 = (buffer*)(shmat(id_shm2, NULL, 0));
	
	//Consuma NUMCONS volte
	for(i = 0; i < NUMCONS; i++){
	
		Wait_Sem(id_sem, MEX_DISP);
		//Se mi trovo nella sezione critica allora sicuramente almeno uno dei buffer 
		//è disponibile per il consumo
		if(msg1->stato == OCCUPATO){
			//Pone lo stato del buffer in IN_USO
			msg1->stato = IN_USO;
			//Consumo
			printf("Consumatore: CONSUMO del valore %d sul buffer 1\n\n", msg1->valore);
			//Pone lo stato del buffer in LIBERO
			msg1->stato = LIBERO;
			//Risveglia l'eventuale produttore
			Signal_Sem(id_sem, SPAZIO_DISP);
		}else if(msg2->stato == OCCUPATO){
			//Se il primo non è disponibile sicuramente il secondo lo sarà
			//Pone lo stato del buffer in IN_USO
			msg2->stato = IN_USO;
			//Consumo
			printf("Consumatore: CONSUMO del valore %d sul buffer 2\n\n", msg2->valore);
			//Pone lo stato del buffer in LIBERO
			msg2->stato = LIBERO;
			//Risveglia l'eventuale produttore
			Signal_Sem(id_sem, SPAZIO_DISP);
		}
		//Attesa di 2 secondi per ogni consumo
		sleep(2);
		
	}
	
	return 0;
}
