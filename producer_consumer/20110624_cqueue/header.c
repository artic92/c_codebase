#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

#define NUMSCHEDULING 25
#define DIMDISCO 20

void utente(Monitor* m, Cond* c, Richiesta* buffer, int* coda){
	int k;

	for(k = 0; k < NUMRICHIESTE; k++){
		enter_monitor(m);
		if(c->ok_produzione == 0){
			printf("Utente in attesa di spazio\n\n");
			wait_condition(m, OK_PRODUZIONE);
		}
		(c->ok_produzione)--;//wait(SPAZIO_DISP)

		//Preparo la richiesta
		Richiesta r;
		srand(time(NULL)+getpid());

		buffer[(*coda)].posizione = (rand() % 19) +1;//Intero casuale da 1 a 19
		buffer[(*coda)].processo = getpid();

		printf("[%d] Richiesta di scrittura di %d nella posizione %d\n\n", getpid(), buffer[(*coda)].processo, buffer[(*coda)].posizione);

		(*coda) = ((*coda) + 1) % DIM;

		(c->ok_consumo)++;//signal(MEX_DISP)
		signal_condition(m, OK_CONSUMO);
		leave_monitor(m);
	}
}


void schedulatore(Monitor* m, Cond* c, Richiesta* buffer, int* testa){
	pid_t disco[DIMDISCO];
	int i, j;
	int posizione_corrente = 0;

	for(i = 0; i < NUMSCHEDULING; i++){
		enter_monitor(m);
		if(c->ok_consumo == 0){
			printf("Schedulatore in attesa di richieste...\n\n");
			wait_condition(m, OK_CONSUMO);
		}
		(c->ok_consumo)--;//Wait(SPAZIO_DISP)

		int posizione = buffer[(*testa)].posizione;
		int sleep_time = abs(posizione - posizione_corrente);

		posizione_corrente = posizione;

		//Scrivo sul disco la richiesta
		disco[posizione] = buffer[(*testa)].processo;

		printf("Scrittura di %d in posizione %d; attendo %d secondi...\n\n", disco[posizione], posizione, sleep_time);
		sleep(sleep_time);

		//Aggiorno la testa della coda circolare
		(*testa) = ((*testa) + 1) % DIM;

		(c->ok_produzione)++;//Signal(MEX_DISP)
		signal_condition(m, OK_PRODUZIONE);
		leave_monitor(m);
	}
}
