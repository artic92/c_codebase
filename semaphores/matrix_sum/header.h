#ifndef HEADER_H
#define HEADER_H

#define SPAZIO_DISP 0
#define MEX_DISP 1
#define MUTEXP 2

#define DEBUG 0

#define COLONNE 4
#define RIGHE 4
#define GRANDEZZA RIGHE*COLONNE

int sommaColonna(int* matshm, int offset);
int sommaVettore(int* vettshm);
void scriviRisultato(int* vettshm, int* indice, int ris);
void stampa(int* matshm);
void riempi(int* m);

void waitSem(int semid, int semnum);
void signalSem(int semid, int semnum);
void waitforzeroSem(int semid, int semnum);

#endif
