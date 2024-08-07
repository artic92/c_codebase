/****PROBLEMA DEI LETTORI-SCRITTORI CON ATTESA INDEFINITA DI ENTRAMBI****/
/*Soluzione con costrutto monitor
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include "../monitor/monitor.h"
#include "header.h"

int main(){

	  /************DICHIARAZIONE DELLE VARIABILI***************/

     int id_shared,k;
     key_t chiave=IPC_PRIVATE;
     Monitor mio_Monitor;
     int status;
     Buffer *ptr_sh;

     pid_t pid;
     int num_processi=10;

  /************RICHIESTA DEL SEGMENTO DI MEMORIA CONDIVISA***********/

    id_shared=shmget(chiave,sizeof(Buffer),IPC_CREAT|0664);

    ptr_sh=(Buffer*) (shmat(id_shared,0,0));
    ptr_sh->numlettori=0;
    ptr_sh->occupato=0;
    ptr_sh->messaggio=0;	
     
//   richiesta del monitor
     init_monitor (&mio_Monitor, NUM_CONDITIONS);


  /****GENERAZIONE DEI PROCESSI E OPERAZIONI DI R/W****/


    //generazione di scrittori e lettori
     for (k=0;k<num_processi;k++) {

	pid=fork();

	if (pid==0)  {                //processo figlio
	  if ( (k%2)==0) {
	     printf("sono il figlio scrittore. Il mio pid %d \n",getpid());
	     Scrittore(&mio_Monitor,ptr_sh);
          }else {
             printf("sono il figlio lettore. Il mio pid %d \n",getpid());
             Lettore(&mio_Monitor,ptr_sh);
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
