/*****PROBLEMA DEI PRODUTTORI-CONSUMATORI: soluzione mediante Monitor*****/
/*Il programma sincronizza processi produttori e processi consumatori nell'accesso ad
  una zona di memoria condivisa utilizzando il costrutto Monitor.

  Header file:header.h
  Programma chiamante:p_c_monitor.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile
  Nome del file eseguibile:p_c_monitor
  Data: 04/01/2004*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../monitor/monitor.h"
#include "header.h"

int main(){
     int id_shared, numconsumatori,numproduttori;
     int status,numprocessi;
     pid_t pid;
     key_t chiave;
     msg* mess;
     Monitor mio_Monitor;
     numconsumatori=numproduttori=5;
     int num_processi=numconsumatori+numproduttori,k;
     cond* conditions;

     //richiesta del buffer
     id_shared=shmget(chiave,sizeof(cond)+sizeof(msg),IPC_CREAT|0664);
     mess=(msg *) (shmat(id_shared,0,0));
     conditions = (cond*) (mess+1);
     conditions->ok_produzione=1;
     conditions->ok_consumo=0;
     //Inizializzazione Monitor
     init_monitor (&mio_Monitor, NUM_CONDITIONS);

//generazione di scrittori e lettori
     for (k=0;k<num_processi;k++) {

	pid=fork();


	if (pid==0)  {                //processo figlio
	  if ( (k%2)==0) {
	     printf("sono il figlio Produttore. Il mio pid %d \n",getpid());
		//sleep(1);
	         Produttore(&mio_Monitor,conditions,mess);
	   }else {
             printf("sono il figlio Consumatore. Il mio pid %d \n",getpid());
             sleep(1);
		Consumatore(&mio_Monitor,conditions,mess);

	    }
          exit(0);
	}


     }

     for (k=0; k<num_processi;k++){
       pid=wait(&status);
       if (pid==-1)
		perror("errore");
	else
		 printf ("Figlio n.ro %d e\' morto con status= %d \n ",pid,status);
     }
     shmctl(id_shared,IPC_RMID,0);
     remove_monitor(&mio_Monitor);
     return 1;
}
