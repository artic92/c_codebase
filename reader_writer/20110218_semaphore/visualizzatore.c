#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "header.h"

#define NUMVISUALIZZ 5

int main (int argc, char *argv[])
{
	printf("[%d] Visualizzatore ready...\n", getpid());
	
	int i, k;
	
	key_t chiave = ftok(PATH, CHARSH);
	int id_shared = shmget(chiave, NUMPOSTI*sizeof(Posto), IPC_CREAT|0664);
	int* disponibilita = (int*)(shmat(id_shared, NULL, 0));
	Posto* vettore_posti = (Posto*)(disponibilita + 1);
	
	Posto p;
	
	//# volte il visualizzatore visualizzerà i dati
	for(k = 0; k < NUMVISUALIZZ; k++){
		
		sleep(1);	
		
		for(i = 0; i < NUMPOSTI; i++){
			p.id_cliente = vettore_posti[i].id_cliente;
			p.stato = vettore_posti[i].stato;
			
			if(p.stato == LIBERO)
					printf("Posto n.ro [%d]: LIBERO\n", i);
			else if(p.stato == INAGGIORNAMENTO)
					printf("Posto n.ro [%d]: IN_AGGIORNAMENTO\n", i);
			else if(p.stato == OCCUPATO)
					printf("Posto n.ro [%d]: OCCUPATO dal cliente %d\n", i, p.id_cliente);
		}
			
	}
	
	return 0;
}
