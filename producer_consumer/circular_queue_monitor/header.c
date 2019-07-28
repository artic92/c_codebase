#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void inizioProduzione(Monitor* m, cond* c){
	enter_monitor(m);
	if(c->ok_produzione == 0) wait_condition(m, OK_PRODUZIONE);
}


void produttore(Monitor* m, cond* c, msg* buffer, int* testa){
	inizioProduzione(m, c);
	sleep(10);
	msg val = rand();
	buffer[(*testa)] = val;
	printf ("<%d> - Valore PRODOTTO: <%d>, testa = <%d>\n", getpid(), val, *testa);
	(*testa) = ((*testa) + 1) % DIM;
	fineProduzione(m, c);
}

void fineProduzione(Monitor* m, cond* c){
	c->ok_produzione = c->ok_produzione - 1;//wait(SPAZIO_D)
	c->ok_consumo = c->ok_consumo + 1;//signal(MEX_DISP)
	signal_condition(m, OK_CONSUMO);
	leave_monitor(m);
}

void inizioConsumo(Monitor* m, cond* c){
	enter_monitor(m);
	if(c->ok_consumo == 0) wait_condition(m, OK_CONSUMO);
}

void consumatore(Monitor* m, cond* c, msg* buffer, int* coda){
	inizioConsumo(m, c);
	msg mex = buffer[(*coda)];
	(*coda) = ((*coda) + 1) % DIM;
	printf("<%d> - Valore CONSUMATO: <%d>\n", getpid(), mex);
	fineConsumo(m, c);
}

void fineConsumo(Monitor* m, cond* c){
	c->ok_consumo = c->ok_consumo - 1;//wait(MEX_DISP)
	c->ok_produzione = c->ok_produzione + 1;//signal(SPAZIO_D)
	signal_condition(m, OK_PRODUZIONE);
	leave_monitor(m);
}

