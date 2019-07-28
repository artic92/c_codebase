#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

//Tempo impiegato 1h:33m

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	char* a = argv[1];
	char* b = argv[2];

	if(!a | !b){
		printf("Errore input nullo\n");
		_exit(0);
	}

	printf("I caratteri inseriti sono %c e %c\n", *a, *b);

	//Alloca code di messaggi dai caratteri presi da argv
	key_t chiave_mittente = ftok(PATH, *a);
	key_t chiave_ricevente = ftok(PATH, *b);
	int id_mittente = msgget(chiave_mittente, IPC_CREAT|0664);
	int id_ricevente = msgget(chiave_ricevente, IPC_CREAT|0664);
	if(id_mittente < 0 || id_ricevente < 0){
		perror("Errore creazione code\n");
		_exit(0);
	}

	//Instanzia processo mittente
	pid = fork();
	if(pid == 0){
		mittente(id_mittente, id_ricevente);
		_exit(0);
	}

	//Instanzia processo ricevente
	pid = fork();
	if(pid == 0){
		ricevente(id_ricevente);
		_exit(0);
	}

	//Attesa terminazione processi
	for(i = 0; i < 2; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Dealloca code di messaggi
	msgctl(id_mittente, IPC_RMID, NULL);
	msgctl(id_ricevente, IPC_RMID, NULL);

	return 0;
}
