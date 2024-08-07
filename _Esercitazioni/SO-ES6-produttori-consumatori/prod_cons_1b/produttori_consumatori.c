/*************PROBLEMA DEI PRODUTTORI-CONSUMATORI***********/

/*Il programma sincronizza nell'accesso ad una zona di memoria condivisa un processo
  consumatore ed un processo produttore. La zona di memoria è costituita da un unico
  buffer.

  Header file:header.h
  Programma chiamante:produttori_consumatori.c
  Modulo delle procedure:procedure.c
  Nome del file eseguibile:start
  Direttive per la compilazione dei moduli:Makefile

*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"




int main(){
     int mutex,sem,id_buffer,id_stato,k,numsem,num_produttori,num_consumatori;
     int status,indice;
     key_t chiave_buffer=IPC_PRIVATE,chiave_stato=IPC_PRIVATE;
     key_t chiave_sem=IPC_PRIVATE;
     msg *ptr_sh;
     struct sembuf sem_buf;
     pid_t pid;
     int *stato;


      num_produttori=1;
      num_consumatori=1;
      int num_processi=num_produttori+num_consumatori;


     //richiesta del buffer
     id_buffer=shmget(chiave_buffer, DIM*sizeof(msg),IPC_CREAT|0664);
     printf(" id_buffer=%d \n",id_buffer);
     ptr_sh=(msg *) (shmat(id_buffer,0,0));

     //richiesta dei  semafori  ed inizializzazione
       mutex=semget(chiave_sem,2,IPC_CREAT|0664);
     //MSG_DISP: segnala al consumatore che è presente un msg nel buffer
       semctl(mutex,MSG_DISP,SETVAL,0);
     //SPAZIO_DISP:segnala al produttore che il msg è stato prelevato -->buffer libero
       semctl(mutex,SPAZIO_DISP,SETVAL,1);


     //generazione di scrittori e lettori
     for (k=0;k<num_processi;k++) {

	pid=fork();

	if (pid==0)  {                //processo figlio
	  if (k==0) {
	     printf("sono il figlio produttore. Il mio pid %d \n",getpid());
	     sleep(2);
	     Produttore(ptr_sh,mutex);
    	  } else {
             printf("sono il figlio consumatore. Il mio pid %d \n",getpid());
             sleep(1);
             Consumatore(ptr_sh,mutex);
          }
          _exit(0);
	}
     }


     for (k=0; k<num_processi;k++){
       pid=wait(&status);
       if (pid==-1)
		perror("errore");
	else
		 printf ("Figlio n.ro %d e\' morto con status= %d \n",pid,status);
     }
     
	shmctl(id_buffer,IPC_RMID,0);
     semctl(mutex,0,IPC_RMID);


     return 1;
}
