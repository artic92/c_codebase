				/*IMPLEMENTAZIONE DELLE PROCEDURE*/

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

//Procedura per il controllo del valore dei semafori


int leggi_valore(int id_sem, int semaforo) {

  return semctl(id_sem,semaforo,GETVAL);
}


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

//Procedure Consumatore

void Consumatore(int*stato,msg * buffer,int sem,int mutex){
	msg mess;
	int indice = RichiestaC(stato,sem,mutex);
	mess=Consumo(indice,buffer);
	printf("Messaggio letto: <%ld>, indice=%d \n",mess,indice);
	RilascioC(indice,stato,sem);
}


int RichiestaC (int*stato,int sem, int mutex){

	int indice=0;
	Wait_Sem(sem,CONS);      // Attende che vi siano messaggi da consumare
	Wait_Sem(mutex,MUTEXC);	 // Blocca eventuali altri consumatori
	// determina l'indice del primo elemento PIENO nel buffer
	while ((indice<=DIM)&&(stato[indice]!=PIENO))
		indice++;
	stato[indice]=IN_USO; // pone lo stato corrente come in uso
	Signal_Sem(mutex,MUTEXC); // Sblocca eventuali altri consumatori
	return indice;
}

msg Consumo(int indice,msg * buffer){
	// effettua il vero e proprio consumo
	msg messaggio;
	messaggio=buffer[indice];
	sleep(2);
	return messaggio;
}

void RilascioC(int indice,int*stato,int sem){
	stato[indice]=VUOTO;    //libera la cella del buffer appena consumata
	Signal_Sem(sem,PROD);   //dà il via libera ai produttori
}

//Procedure Produttore
void Produttore(int*stato,msg *buffer,int sem,int mutex,int k){
	int indice= RichiestaP (stato,sem,mutex);
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1,&t2); // valori diversI ad ogni produzione-->comodità di debugging
	msg val =t1.tv_usec;
	printf ("Produzione in corso di %ld, indice=%d, valore semaforo PROD=%d \n",val,indice,leggi_valore(sem,PROD));
	Produzione(indice,val,buffer);
	if (k==0) sleep(8);  // simula un produttore "lento" (il primo, con indice 0)
	RilascioP(indice,stato,sem);
}

int RichiestaP (int*stato,int sem, int mutex){
	int indice=0;
	Wait_Sem(sem, PROD); // Attende che vi sia spazio per produrre
	Wait_Sem(mutex,MUTEXP); // Blocca eventuali altri produttori
	// determina l'indice del primo elemento VUOTO nel buffer
	while ((indice<=DIM)&&(stato[indice]!=VUOTO))
		indice++;
	stato[indice]=IN_USO; // segna la cella come in uso
	Signal_Sem(mutex,MUTEXP); // Sblocca eventuali altri produttori
	return indice;
}

void Produzione(int indice,msg messaggio,msg *buffer){
	// effettua la vera e propria produzione
	buffer[indice]=messaggio;
}

void RilascioP (int indice,int*stato,int sem){
	stato[indice]=PIENO; //segnala che è stato prodotto un messaggio nella cella indice
	Signal_Sem(sem,CONS);//dà il via libera ai consumatori
}
