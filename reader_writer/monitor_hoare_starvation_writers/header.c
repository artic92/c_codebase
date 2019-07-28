#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include "header.h"

void inizioLettura(Monitor* m, cond* c){
	enter_monitor(m);
	
	//Se ci sono scrittori, i lettori si devono bloccare
	if(c->occupato == 1 && c->num_lettori == 0)
		wait_condition(m, OK_LETTURA);
		
	(c->num_lettori)++;
	
	//Se è il primo lettore blocca gli scrittori
	if(c->num_lettori == 1)	
		c->occupato = 1;
	
	//Sblocco di eventuali lettori bloccati ma non sbloccati dallo scrittore che sblocca soltanto il primo
	//lettore bloccato	
	signal_condition(m, OK_LETTURA); 

	leave_monitor(m);
}

void lettura(Monitor* m, cond* c, msg* mex){
	inizioLettura(m, c);

	msg val;
	val = (*mex);
	printf("<%d> - LETTO valore %d\n", getpid(), val);
	
	fineLettura(m, c);
	
}

void fineLettura(Monitor* m, cond* c){
	enter_monitor(m);
	
	(c->num_lettori)--;
	
	//XKÈ NON SBLOCCARE EVENTUALI LETTORI BLOCCATI QUI E FARLO NELL'INIZIO LETTURA?
	//Perchè i lettori possono leggere contemporaneamente. Non ha senso sbloccare un lettore solo quando
	//un altro lettore ha terminato altrimenti non si avrebbe la possibilità che più lettori possano leggere
	//contemporaneamente, allora li si sblocca all'inizio in modo che tutti possano eseguire parallelamente.
	
	//Se è l'ultmo lettore sblocca gli eventuali scrittori
	if(c->num_lettori == 0){
		c->occupato = 0;
		signal_condition(m, OK_SCRITTURA);
	}	
	
	leave_monitor(m);
}

void inizioScrittura(Monitor* m, cond* c){
	enter_monitor(m);
	
	//Se ci sono scrittori o lettori, gli scrittori si devono bloccare
	if(c->occupato == 1)
		wait_condition(m, OK_SCRITTURA);
		
	c->occupato = 1;
	
	leave_monitor(m);
}

void scrittura(Monitor* m, cond* c, msg* mex){
	inizioScrittura(m, c);

	struct timeval t1;
    	struct timezone t2;
 	gettimeofday(&t1,&t2);    //valore diverso ad ogni produzione
    	(*mex) =t1.tv_usec;

	printf("<%d> - SCRITTO valore %d\n", getpid(), *mex);
	
	fineScrittura(m, c);
}

void fineScrittura(Monitor* m, cond* c){
	enter_monitor(m);
	
	//L'INVERSIONE DEGLI IF DOVREBBE ESSERE L'UNICA COSA CHE VARIA NELLA STARVATION DEGLI SCRITTORI
	if(queue_condition(m, OK_LETTURA) > 0){			
		c->occupato = 0;
		signal_condition(m, OK_LETTURA);
	}else if(queue_condition(m, OK_SCRITTURA) > 0){
		c->occupato = 0;
		signal_condition(m, OK_SCRITTURA);
	}else{
		c->occupato = 0;
	}
	
	leave_monitor(m);
}
