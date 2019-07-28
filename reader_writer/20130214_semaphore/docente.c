#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include "header.h"
#include "semafori.h"

#define NUMVOLTE 3

int main (int argc, char *argv[])
{
	printf("<%d> - Docente\n", getpid());

	key_t chiave_sem = ftok(PATH, CHARS), chiave_appelli = ftok(PATH, CHARA);
	int id_sem = semget(chiave_sem, NUMSEM, IPC_CREAT|0664);
	int id_shared = shmget(chiave_appelli, sizeof(Appello), IPC_CREAT|0664);
	Appello* a = (Appello*)(shmat(id_shared, NULL, 0));

	int i;
	char* date[3];

	date[0] = "13/02/2015";
	date[1] = "05/03/2015";
	date[2] = "20/03/2015";

	srand(time(NULL));

	for(i = 0; i < NUMVOLTE; i++){

		//Generazione data di appello
		Wait_Sem(id_sem, SYNC);
		strcpy(a->data_appello, date[i]);
		Signal_Sem(id_sem, SYNC);

		sleep(3);

		//Stampa il numero di prenotati
		Wait_Sem(id_sem, SYNC);
		printf("\nPrenotati per l'appello del %s: %d \n\n", a->data_appello, a->num_prenotati);
		a->num_prenotati = 0;
		Signal_Sem(id_sem, SYNC);

	}

	return 0;
}
