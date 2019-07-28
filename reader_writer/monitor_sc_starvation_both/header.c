#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void inizioLettura(Monitor* m, cond* c){
	enter_monitor(m);

	//I lettori si bloccano SOLO se ci sono scrittori
	while(c->occupato == 1 && c->num_lettori == 0)
		wait_condition(m, OK_LETTURA);

	//se la metti prima della wait la variabile viene incrementata DUE volte dallo stesso
	//processo poikè stiamo utilizzando un signal_and_continue
	(c->num_lettori)++;

	if(c->num_lettori == 1)
		c->occupato = 1;

	signal_condition(m, OK_LETTURA);

	leave_monitor(m);
}

void lettore(Monitor* m, cond* c, msg* mex){
	inizioLettura(m, c);
	sleep(1);
	msg val = (*mex);
	printf("<%d> - LETTO valore %d, # lettori:%d\n", getpid(), val, c->num_lettori);

	fineLettura(m, c);
}

void fineLettura(Monitor* m, cond* c){
	enter_monitor(m);

	(c->num_lettori)--;

	if(c->num_lettori == 0){
		c->occupato = 0;
		signal_condition(m, OK_SCRITTURA);
	}

	leave_monitor(m);
}

void inizioScrittura(Monitor* m, cond* c){
	enter_monitor(m);

	while(c->occupato == 1)
		wait_condition(m, OK_SCRITTURA);

	c->occupato = 1;

	leave_monitor(m);
}

void scrittore(Monitor* m, cond* c, msg* mex){
	inizioScrittura(m, c);
	sleep(2);
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1, &t2);
    	(*mex) =t1.tv_usec;
	printf("<%d> - SCRITTO valore %d\n", getpid(), (*mex));

	fineScrittura(m, c);
}

void fineScrittura(Monitor* m, cond* c){
	enter_monitor(m);

	if(queue_condition(m, OK_SCRITTURA) > 0)
		signal_condition(m, OK_SCRITTURA);
	else if(queue_condition(m, OK_LETTURA) > 0)
		signal_condition(m, OK_LETTURA);

	c->occupato = 0;

	leave_monitor(m);
}
