			/*IMPLEMENTAZIONE DELLE PROCEDURE*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
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

void Consumatore(msg *ptr_sh,int mutex){

       msg mess;
       Wait_Sem(mutex,MSG_DISP);
       mess=*ptr_sh;
       printf("Cons <%d>: Messaggio letto: %ld, valore semaforo MSG_DISP=%d \n",getpid(),mess,leggi_valore(mutex,MSG_DISP));
       Signal_Sem(mutex,SPAZIO_DISP);

}

//Procedure Produttore

void Produttore(msg*ptr_sh,int mutex){

    struct timeval t1;
    struct timezone t2;
    gettimeofday(&t1,&t2);
    msg val =t1.tv_usec;
    Wait_Sem(mutex,SPAZIO_DISP);
    printf ("Prod <%d>: Produzione in corso (%ld), valore semaforo SPAZIO_DISP=%d \n",getpid(),val,leggi_valore(mutex,SPAZIO_DISP));
    *ptr_sh=val;
    Signal_Sem(mutex,MSG_DISP);

}

