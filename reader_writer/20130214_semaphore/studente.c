#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"
#include "semafori.h"

int main (int argc, char *argv[])
{
	printf("<%d> - Studente\n", getpid());

	key_t chiave_sem = ftok(PATH, CHARS), chiave_appelli = ftok(PATH, CHARA);
	int id_sem = semget(chiave_sem, NUMSEM, IPC_CREAT|0664);
	int id_shared = shmget(chiave_appelli, sizeof(Appello), IPC_CREAT|0664);
	Appello* a = (Appello*)(shmat(id_shared, NULL, 0));

	srand(time(NULL) + getpid());

	int sleep_time = (rand() % 8) + 1;

	printf("<%d> - Attendo %d secondi\n", getpid(), sleep_time);

	sleep(sleep_time);

	//Mando in starvation il docente
	Wait_Sem(id_sem, MUTEX);
	(a->num_studenti)++;
	(a->num_prenotati)++;
	if(a->num_studenti == 1)
		Wait_Sem(id_sem, SYNC);
	Signal_Sem(id_sem, MUTEX);

	printf("<%d> - Studente prenotato per l'appello del %s\n", getpid(), a->data_appello);

	Wait_Sem(id_sem, MUTEX);
	(a->num_studenti)--;
	if(a->num_studenti == 0)
		Signal_Sem(id_sem, SYNC);
	Signal_Sem(id_sem, MUTEX);

	return 0;
}
