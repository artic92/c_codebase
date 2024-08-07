			/*------IMPLEMENTAZIONE DELLE PROCEDURE--------*/

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../monitor/monitor.h"
#include "header.h"



//Procedure di inizio e fine lettura

void InizioLettura(Monitor* m,Buffer*buf){
	enter_monitor(m);
        
	if (buf->occupato && buf->numlettori==0) //se ci sono già scrittori, si deve bloccare
		wait_condition(m,SYNCHL);
	buf->occupato = 1;
	buf->numlettori=buf->numlettori+1;
	
	signal_condition(m,SYNCHL); // sblocco di eventuali altri lettori, 
				    // se la segnalazione non è fatta per 
				    // tutti dallo scrittore che rilascia
    }

void FineLettura(Monitor* m, Buffer*buf){
        enter_monitor(m);
        buf->numlettori=buf->numlettori-1;
        if (buf->numlettori==0) {// se sono finiti i lettori lo segnala agli scrittori
     		buf->occupato=0;
		signal_condition(m,SYNCHS);
	}
        else // altrimenti lascia il monitor
        	leave_monitor(m);
}



//Procedure di inizio e fine scrittura

void InizioScrittura(Monitor* m,Buffer*buf){
	enter_monitor(m);
	if (buf->occupato) //se ci sono già lettori o scrittori, si deve bloccare     	
		wait_condition(m,SYNCHS);
	buf->occupato = 1;
	leave_monitor(m);
    }

void FineScrittura(Monitor* m, Buffer*buf){
	enter_monitor(m);
	if (queue_condition(m,SYNCHS)>0) // sblocco di eventuali altri scrittori
		signal_condition(m,SYNCHS);
        else if (queue_condition(m,SYNCHL)>0) { // sblocco di eventuali altri lettori
		// while (queue_condition(m,SYNCHL) ... sblocco di tutti i lettori
		//buf->occupato =0;
     		signal_condition(m,SYNCHL);
	}
        else { // altrimenti lascia il monitor
        	buf->occupato =0;
		leave_monitor(m);
	}
	
}



void Scrittore(Monitor* m, Buffer *buf){

	InizioScrittura(m,buf);
	struct timeval t1;
    	struct timezone t2;
 	gettimeofday(&t1,&t2);    //valore diverso ad ogni produzione
    	buf->messaggio =t1.tv_usec;
	sleep(1);
    	printf ("Valore scritto: <%d> \n", (int)buf->messaggio);
	FineScrittura(m,buf);
}

void Lettore (Monitor* m, Buffer* buf) {

	InizioLettura(m,buf);

	/*********Lettura********/
	//sleep(1); // per simulare un ritardo di lettura
        printf("Valore letto=<%d>, numero lettori=%d \n",(int)buf->messaggio,buf->numlettori);
	FineLettura(m,buf);
}


