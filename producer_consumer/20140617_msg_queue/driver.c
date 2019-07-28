#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

#define NUMCLIENT 3

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	//Allocazione code
	key_t chiave_comandi = ftok(PATH, CHARC);
	int id_queue_comandi = msgget(chiave_comandi, IPC_CREAT|0664);

	key_t chiave_risposte = ftok(PATH, CHARR);
	int id_queue_risposte = msgget(chiave_risposte, IPC_CREAT|0664);

	//Creazione processi client
	for(i = 0; i < NUMCLIENT; i++){
		pid = fork();
		if(pid == 0){
			execvp("./client", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Creazione processo server
	pid = fork();
	if(pid == 0){
		execvp("./server", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	//Attesa terminazione processi
	for(i = 0; i < NUMCLIENT; i++){
		pid = wait(&status);
		sleep(1);
		printf("Processo %d terminato con stato %d\n\n", pid, status);
	}

	//Invio messaggio di terminazione al server
	Messaggio_richiesta m;
	m.processo = getpid();
	strcpy(m.messaggio,"exit");
	msgsnd(id_queue_comandi, &m, sizeof(Messaggio_richiesta)-sizeof(long), 0);

	//Attesa terminazione server
	pid = wait(&status);
	printf("Processo server %d terminato con stato %d\n\n", pid, status);

	//Deallocazione coda
	msgctl(id_queue_comandi, IPC_RMID, NULL);
	msgctl(id_queue_risposte, IPC_RMID, NULL);

	return 0;
}
