#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include "header.h"

int main (int argc, char *argv[])
{
	int sums, matrice, mutex, i, pidp, pid, risultato = 0, sumCol, somma = 0;
	int *sumshm, *matshm, *spazioVuoto, *rimanenti;

	mutex = semget(IPC_PRIVATE, 3, IPC_CREAT | IPC_EXCL | 0622);
	sums = shmget(IPC_PRIVATE, (2*sizeof(int))+(COLONNE*sizeof(int)), IPC_CREAT | IPC_EXCL | 0622);
	matrice = shmget(IPC_PRIVATE, GRANDEZZA*sizeof(int), IPC_CREAT | IPC_EXCL | 0622);

	semctl(mutex, MUTEXP, SETVAL, 1);
	semctl(mutex, SPAZIO_DISP, SETVAL, COLONNE);
	semctl(mutex, MEX_DISP, SETVAL, 0);

	rimanenti = (int*)(shmat(sums,NULL, 0));
	(*rimanenti) = COLONNE;
	spazioVuoto = rimanenti;
	spazioVuoto++;
	(*spazioVuoto) = 0;
	sumshm = spazioVuoto;
	sumshm++;
	matshm = (int*)shmat(matrice,NULL, 0);
	riempi(matshm);

	#if DEBUG
	stampa(matshm);
	#endif

	pidp = fork();
	if (pidp == 0){
		for(i = 0; i < COLONNE; i++){
			pid = fork();
			if (pid == 0){
				sumCol = sommaColonna(matshm, i);
				printf("Processo %d: risultato somma: %d\n", i, sumCol);
				waitSem(mutex, SPAZIO_DISP);
				waitSem(mutex, MUTEXP);
				#if DEBUG
				printf("Valore MUTEXP dopo wait: %d\n", semctl(mutex, MUTEXP, GETVAL, 0));
				printf("Processo %d: valore rimanenti prima: %d\n", i, (*rimanenti));
				#endif
				(*rimanenti)--;
				scriviRisultato(sumshm, spazioVuoto, sumCol);
				if((*rimanenti) == 0)
					signalSem(mutex, MEX_DISP);
				#if DEBUG
				printf("Processo %d: valore rimanenti dopo: %d\n", i, (*rimanenti));
				#endif
				signalSem(mutex, MUTEXP);
				#if DEBUG
				printf("Valore MUTEXP dopo signal: %d\n", semctl(mutex, MUTEXP, GETVAL, 0));
				//printf("Processo %d: ho terminato!", i); non eseguita!
				#endif
				_exit(0);
				}
		}
		#if DEBUG
		printf("Processo P: in attesa dei figli\n");
		#endif
		waitSem(mutex, MEX_DISP);
		somma = sommaVettore(sumshm);
		#if DEBUG
		printf("DEBUG: Processo P: la somma e' %d\n", somma);
		#endif
		(*sumshm) = somma;
		_exit(0);
	}else if (pidp > 0){
		wait(NULL);
		risultato = (*sumshm);
		printf("Processo DRIVER: il risultato e': %d\n", risultato);
	}else
		perror("Errore fork!");

	semctl(mutex, IPC_RMID, 0);
	shmctl(matrice, IPC_RMID, 0);
	shmctl(sums, IPC_RMID, 0);
	return 0;
}
