			/*----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem)     {
	struct sembuf sem_buf;
	sem_buf.sem_num=numsem;
	sem_buf.sem_flg=0;
	sem_buf.sem_op=-1;
	semop(id_sem,&sem_buf,1);   //semaforo rosso
}


  void Signal_Sem (int id_sem,int numsem)     {
	struct sembuf sem_buf;
	sem_buf.sem_num=numsem;
	sem_buf.sem_flg=0;
	sem_buf.sem_op=1;
	semop(id_sem,&sem_buf,1);   //semaforo verde
}

// Consumo
void Consumatore(int * coda, msg * buffer,int sem ){
	msg mess;
	Wait_Sem(sem, NUM_MESS);
	  mess=buffer[(*coda)]; // accesso al buffer in posizione "coda"
	  (*coda)=++(*coda) %DIM; // incremento circolare (modulo DIM)
	  printf("Messaggio letto: %ld \n",mess);
	Signal_Sem(sem, SPAZIO_DISP);// nelem=nelem-1
}

//Produzione
void Produttore(int * testa ,msg * buffer,int sem){

	Wait_Sem(sem,SPAZIO_DISP);
	  struct timeval t1;
	  struct timezone t2;
	  gettimeofday(&t1,&t2); //valore diverso ad ogni produzione
	  msg val =t1.tv_usec;
	  buffer[(*testa)]=val;
	  (*testa)=++(*testa) % DIM;
	  printf ("Produzione in corso di %ld, testa=<%d> \n",val,*testa);
	Signal_Sem(sem, NUM_MESS); // nelem=nelem+1
}
