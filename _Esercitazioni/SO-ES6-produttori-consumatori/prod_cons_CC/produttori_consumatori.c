/****PROBLEMA DEI PRODUTTORI-CONSUMATORI*****/
/*Il programma gestisce l'accesso di processi consumatori e di processi
  produttori ad una zona di memoria condivisa (id_buffer).Tale zona è
  intesa quale pool di buffer su cui i produttori possono depositare il
  messaggio prodotto e da cui i processi consumatori possono prelevare.
  La gestione del buffer avviene mediante coda circolare

  Header file:header.h
  Programma chiamante:produttori_consumatori.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile

  */


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main(){
	int sem,id_buffer,k,i,numsem,num_produttori,num_consumatori,status;
	key_t chiave_buffer=IPC_PRIVATE;
	key_t chiave_sem=IPC_PRIVATE;
	msg *ptr_sh;
	struct sembuf sem_buf;
	pid_t pid;
	int *stato;


	num_produttori=1;
	num_consumatori=1;
	int num_processi=num_produttori+num_consumatori;


	//richiesta del buffer --> segmento di memoria condivisa
	id_buffer=shmget(chiave_buffer,( DIM*sizeof(msg) +(2*sizeof(int))),IPC_CREAT|0664);
	printf(" id_buffer=%d \n",id_buffer);


	// NOTA: anche le variabili che memorizzano testa e coda sono memorizzate nella
	//	 memoria condivisa
	// Il primo elemento della memoria condivisa e' il puntatore testa della coda
	int *testa= (int *) ( shmat(id_buffer,0,0));
	// Il secondo elemento e' il puntatore coda della coda circolare
	int * coda= testa+1; // testa è un puntatore, per cui testa+1 equivale a testa+sizeof(int)
	// I rimanenti elementi costituiscono i messaggi
	ptr_sh= (msg *) (coda+1);
	//inizializzazione del valore dei puntatori
	(*testa)=0;
	(*coda)=0;


	//richiesta di 2  semafori ed inizializzazione
	sem=semget(chiave_sem,2,IPC_CREAT|0664);
	//Spazio disponibile nella coda circolare
	semctl(sem,SPAZIO_DISP,SETVAL,DIM); // all'inizio, tanto SPAZIO_DISP quanto è grande il buffer (DIM)
	//Numero dei messaggi presenti nella coda circolare
	semctl(sem,NUM_MESS,SETVAL,0); // all'inizio, ci sono NUM_MESS=0 messaggi



	//generazione di produttori e consumatori
	for (k=0;k<num_processi;k++) {

		pid=fork();

		if (pid==0)  {                //processo figlio
			if (k%2==0) {
	     			printf("sono il figlio produttore. Il mio pid %d \n",getpid());
	     			sleep(1);
       				for (i=0;i<num_produzioni;i++)
      					Produttore(testa,ptr_sh,sem);


			} else {
				printf("sono il figlio consumatore. Il mio pid %d \n",getpid());
				sleep(6);
				for (i=0;i<num_consumi;i++)
					Consumatore(coda,ptr_sh,sem);
			}
			_exit(0);
		}


	}

	for (k=0; k<num_processi;k++){

	//Attesa di terminazione da parte del processo padre
	pid=wait(&status);
	if (pid==-1)
		perror("errore");
	else
		printf ("Figlio n.ro %d e\' morto con status= %d \n ",pid,status);
	}

	//Cancellazione del buffer condiviso e dei semafori
	shmctl(id_buffer,IPC_RMID,0);
	semctl(sem,0,IPC_RMID);

	return 1;
}
