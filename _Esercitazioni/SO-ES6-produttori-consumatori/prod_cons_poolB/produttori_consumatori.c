/****PROBLEMA DEI PRODUTTORI-CONSUMATORI*****/
/*Il programma gestisce l'accesso di processi consumatori e di processi
  produttori ad una zona di memoria condivisa (id_buffer).Tale zona �
  intesa quale pool di buffer su cui i produttori possono depositare il
  messaggio prodotto e da cui i processi consumatori possono prelevare.
  La gestione del pool di buffer avviene a mezzo di un ulteriore vettore
  condiviso (id_stato)

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
	
	num_produttori=10;
	num_consumatori=10;
	int num_processi=num_produttori+num_consumatori;

	//richiesta del buffer
	id_buffer=shmget(chiave_buffer, DIM*sizeof(msg),IPC_CREAT|0664);
	printf(" id_buffer=%d \n",id_buffer);
	ptr_sh=(msg *) (shmat(id_buffer,0,0));

	//richiesta del vettore di stato ed inizializzazione
	id_stato=shmget(chiave_stato, DIM*sizeof(int),IPC_CREAT|0664);
	printf(" id_stato=%d \n",id_stato);
	stato=(int *) ( shmat(id_stato,0,0));
	int ci;
	// inizializzazione del vettore di stato a tutti VUOTI
	for (ci=0;ci<DIM;ci++)
		stato[ci]=VUOTO;

	//richiesta di  semafori ed inizializzazione

	mutex=semget(chiave_sem,2,IPC_CREAT|0664);
	//MUTEXP: mutex sulle variabili del produttore
	semctl(mutex,MUTEXP,SETVAL,1);
	//MUTEXC: mutex sulle variabili del consumatore
	semctl(mutex,MUTEXC,SETVAL,1);

	sem=semget(chiave_sem,2,IPC_CREAT|0664);
	//PROD: spazio_disponibile
	semctl(sem,PROD,SETVAL,DIM);
	//CONS: messaggio_disponibile
	semctl(sem,CONS,SETVAL,0);


	// generazione produttori e consumatori

	for (k=0;k<num_processi;k++) {

		pid=fork();

		if (pid==0)  {                //processo figlio
			if (k%2==0) {
				printf("sono il figlio produttore. Il mio pid %d \n",getpid());
				sleep(4);
				Produttore(stato,ptr_sh,sem,mutex,k);
			} else {
				printf("sono il figlio consumatore. Il mio pid %d \n",getpid());
				sleep(1);
				Consumatore(stato,ptr_sh,sem,mutex);
			}
			_exit(0);
		}
	}

	for (k=0; k<num_processi;k++){
	pid=wait(&status);
	if (pid==-1)
		perror("errore");
	else
		 printf ("Figlio n.ro %d e\' morto con status= %d \n ",pid,status);
	}

	// rimozione memoria condivisa e semafori
	shmctl(id_buffer,IPC_RMID,0);
	shmctl(id_stato,IPC_RMID,0);
	semctl(mutex,0,IPC_RMID);
	semctl(sem,0,IPC_RMID);

	return 1;
}
