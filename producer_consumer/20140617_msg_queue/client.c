#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

#define NUMMEX 5

int main (int argc, char *argv[])
{
	printf("<%d> - Client\n", getpid());
	sleep(1);
	
	int i;
	
	//Allocazione code
	key_t chiave_comandi = ftok(PATH, CHARC);
	int id_queue_comandi = msgget(chiave_comandi, IPC_CREAT|0664);
	
	key_t chiave_risposte = ftok(PATH, CHARR);
	int id_queue_risposte = msgget(chiave_risposte, IPC_CREAT|0664);
	
	//Allocazione array di comandi
	char* comandi[NUMMEX];
	comandi[0] = "mkdir prova";
	comandi[1] = "rm prova/test";
	comandi[2] = "touch prova/test";
	comandi[3] = "ls";
	comandi[4] = "cp prova/test prova/test2";
	
	//Preparazione messaggio
	Messaggio_richiesta m1;
	m1.processo = getpid();
	Messaggio_risposta m2;
	
	for(i = 0; i < NUMMEX; i++){
	
		//Preparazione messaggio
		strcpy(m1.messaggio, comandi[i]);
		
		//Invia richiesta al server
		msgsnd(id_queue_comandi, &m1, sizeof(Messaggio_richiesta)-sizeof(long), 0);
		
		printf("[%d] - Inviato comando: %s\n\n", getpid(), comandi[i]);
		
		//Attende ricezione di una risposta
		msgrcv(id_queue_risposte, &m2, sizeof(Messaggio_risposta)-sizeof(long), getpid(), 0);
		
		//Stampo output della risposta
		printf("[%d] - Codice di risposta al comando: %d\n\n", getpid(), m2.val_ritorno);
		
	}
	
	
	return 0;
}
