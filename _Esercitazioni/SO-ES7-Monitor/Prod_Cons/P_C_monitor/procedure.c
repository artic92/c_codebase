			/*-----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../monitor/monitor.h"
#include "header.h"



void InizioConsumo(Monitor* m, cond* c){
#ifdef DEBUG_
	printf("<%d> InizioConsumo \n",getpid());
#endif
	enter_monitor(m);
	if (!c->ok_consumo) wait_condition(m,OK_CONSUMO);
}

void FineConsumo(Monitor* m, cond* c){
#ifdef DEBUG_
	printf("<%d> FineConsumo \n",getpid());
#endif
	c->ok_produzione = 1;
	c->ok_consumo = 0;
        signal_condition(m,OK_PRODUZIONE);
	
}


void InizioProduzione(Monitor * m, cond* c){
#ifdef DEBUG_
	printf("<%d> InizioProduzione \n",getpid());
#endif

	enter_monitor(m);
	if (!c->ok_produzione) wait_condition(m,OK_PRODUZIONE);


}

void FineProduzione (Monitor * m, cond* c){
#ifdef DEBUG_
	printf("<%d> FineProduzione \n",getpid());
#endif
	c->ok_produzione = 0;
	c->ok_consumo = 1;
	signal_condition(m,OK_CONSUMO);
	
}

void Produttore(Monitor * m, cond* c, msg* mess) {
	       InizioProduzione(m,c);
	       /*********Produzione********/
               struct timeval t1;
               struct timezone t2;
               gettimeofday(&t1,&t2);//valore diverso ad ogni produzione
               msg val =t1.tv_usec;
	       *mess = val;	  
	       printf("<%d> Valore PRODOTTO =<%d> \n",getpid(), *mess);
	       FineProduzione(m,c);
}



void Consumatore (Monitor * m, cond* c, msg* mess) {

		InizioConsumo(m,c);
		/*********Consumo********/
	        printf("<%d> Valore CONSUMATO =<%d> \n",getpid(), *mess);
		FineConsumo(m,c);
}

