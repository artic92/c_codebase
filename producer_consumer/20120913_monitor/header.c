#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "monitor.h"
#include "header.h"

void inizializza_prod_cons(PriorityProdCons * p){
	int i;
	p->testa = p->coda = 0;
	p->ok_consumo = 0;
	p->ok_produzione = DIM;
	for(i = 0; i < 3; i++)
		p->buffer[i] = 0;
	init_monitor(&(p->m), NUMCOND);
}

void produci_alta_prio(PriorityProdCons * p){
	enter_monitor(&(p->m));
	while(p->ok_produzione == 0)
		wait_condition(&(p->m), OK_PRODUZIONE_ALTAP);
	(p->ok_produzione)--;//wait(SPAZIO_DISP)
	(p->num_produttori_alta_prio)++;


	srand(time(NULL)+getpid());
	int val = (rand() % 12) + 1;//valore casuale da 0 a 12
	p->buffer[p->testa] = val;
	p->testa = (p->testa + 1) % DIM;
	printf("Produzione ALTA priorità, valore prodotto: %d, testa %d\n", val, p->testa);

	(p->num_produttori_alta_prio)--;
	(p->ok_consumo)++;//signal(MEX_DISP)
	signal_condition(&(p->m), OK_CONSUMO);
	leave_monitor(&(p->m));
}

void produci_bassa_prio(PriorityProdCons * p){
	enter_monitor(&(p->m));
	while(p->ok_produzione == 0 || p->num_produttori_alta_prio > 0)
		wait_condition(&(p->m), OK_PRODUZIONE_BASSAP);
	(p->ok_produzione)--;//wait(SPAZIO_DISP)

	srand(time(NULL)+getpid());
	int val = (rand() % 25) + 13;//valore casuale da 13 a 25
	p->buffer[p->testa] = val;
	p->testa = (p->testa + 1) % DIM;
	printf("Produzione BASSA priorità, valore prodotto: %d, testa %d\n", val, p->testa);

	(p->ok_consumo)++;//signal(MEX_DISP)
	signal_condition(&(p->m), OK_CONSUMO);
	leave_monitor(&(p->m));
}

void consuma(PriorityProdCons * p){
	enter_monitor(&(p->m));
	while(p->ok_consumo == 0)
		wait_condition(&(p->m), OK_CONSUMO);
	(p->ok_consumo)--;//wait(MEX_DISP)

	int val = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	printf("Valore consumato: %d, coda %d\n", val, p->coda);

	(p->ok_produzione)++;//signal(SPAZIO_DISP)

	if(queue_condition(&(p->m), OK_PRODUZIONE_ALTAP) > 0)
		signal_condition(&(p->m), OK_PRODUZIONE_ALTAP);
	else if(queue_condition(&(p->m), OK_PRODUZIONE_BASSAP) > 0)
		signal_condition(&(p->m), OK_PRODUZIONE_BASSAP);

	leave_monitor(&(p->m));
}

void rimuovi_prod_cons(PriorityProdCons * p){
	remove_monitor(&(p->m));
}
