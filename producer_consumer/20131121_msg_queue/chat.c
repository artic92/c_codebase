#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"


void mittente(int id_mittente, int id_ricevente){
	
	printf("<%d> - Mittente\n", getpid());	
	
	Messaggio mittente;
	mittente.processo = getpid();
	
	//Buffer nel quale verrà messo il messaggio
	char buffer[100];
	int ret;
	
	printf("Benvenuto nel programma di chat!\n");
	
	sleep(1);
	
	while(1){
		
		printf("Inserisci un messaggio (max 100 caratteri):\n");
		scanf("%s", buffer);	
		
		if(strcmp(buffer, "exit") == 0){
			printf("[Mittente] Chiusura in corso...\n");
			strcpy(mittente.messaggio, "exit");
			msgsnd(id_ricevente, &mittente, sizeof(Messaggio)-sizeof(long), 0);
			_exit(0);
		}
		
		strcpy(mittente.messaggio, buffer);
		
		ret = msgsnd(id_mittente, &mittente, sizeof(Messaggio)-sizeof(long), 0);
		if(ret < 0)
			_exit(0);
		
		printf("[SENDER] inviato: %s\n", mittente.messaggio);
		
	}

}

void ricevente(int id_ricevente){

	printf("<%d> - Ricevente\n", getpid());

	Messaggio ricevente;
	char risposta[100];
	int ret;
	
	sleep(1);
	
	while(1){
		
		ret = msgrcv(id_ricevente, &ricevente, sizeof(Messaggio)-sizeof(long), 0, 0);
		//Il controllo è importante per la chiusura, poichè la sys_call fallisce
		if(ret < 0)
			_exit(0);
		
		strcpy(risposta, ricevente.messaggio);
		
		if(strcmp(risposta,"exit") == 0){
			printf("[Ricevente] Chiusura in corso...\n");
			_exit(0);
		}
		
		printf("Messaggio ricevuto: %s\n", risposta);
				
	}
}
