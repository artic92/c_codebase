#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#include "header.h"

int sommaColonna(int* matshm, int offset){
	int indice = 0, somma = 0;
	int* prv = matshm;
	prv += (offset*RIGHE);
	while(indice < RIGHE){
		somma += (*prv);
		prv++;
		indice++;
	}
	return somma;
}

int sommaVettore(int* vettshm){
	int* prv = vettshm;
	int somma = 0, indice = 0;
	while(indice < COLONNE){
		somma += (*prv);
		prv++;
		indice++;
	}
	return somma;
}

void scriviRisultato(int* vettshm, int* indice, int ris){
	int* prv = vettshm;
	vettshm += (*indice);
	(*vettshm) = ris;
	(*indice)++;
}

void stampa(int* matshm){
	int* prv = matshm;
	int indice = 0;
	while(indice < GRANDEZZA){
		printf(" %d ", (*prv));
		prv++;
		indice++;
		}
	printf("\n");
}

void riempi(int* m){
	int indice = 0;
	int* prv = m;
	while(indice < GRANDEZZA){
		(*prv) = indice;
		prv++;
		indice++;
		}
}

void waitSem(int semid, int semnum){
	struct sembuf s;
	s.sem_num = semnum;
	s.sem_op = -1;
	s.sem_flg = 0;
	semop(semid, &s, 1);
}

void signalSem(int semid, int semnum){
	struct sembuf s;
	s.sem_num = semnum;
	s.sem_op = 1;
	s.sem_flg = 0;
	semop(semid, &s, 1);
}

void waitforzeroSem(int semid, int semnum){
	struct sembuf s;
	s.sem_num = semnum;
	s.sem_op = 0;
	s.sem_flg = 0;
	semop(semid, &s, 1);
}
