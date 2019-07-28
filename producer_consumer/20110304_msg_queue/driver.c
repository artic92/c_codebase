#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include "header.h"

//Non terminato

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;
	pthread_t thread[NUMCLIENT];

	//Alloco le code delle richieste
	key_t chiave_richieste = ftok(PATH, CHARREQ), chiave_risposte = ftok(PATH, CHARREP);
	int queue_req = msgget(chiave_richieste, IPC_CREAT|0664);
	int queue_rep = msgget(chiave_risposte, IPC_CREAT|0664);

	//Creo i thread client
	for(i = 0; i < NUMCLIENT; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Client\n", getpid());
			sleep(1);
			client();
			_exit(0);
		}
	}

	//Creo server
	pid = fork();
	if(pid == 0){
		sleep(1);
		execvp("./server", NULL);

		perror("Exec fallita!\n");
		_exit(0);
	}

	for(i = 0; i < NUMCLIENT; i++){
		pid = wait(&status);
/*		printf("Processo client %d terminato con stato %d\n", pid, status);*/
	}

	//Invio messaggio di terminazione al server
	Messaggio m;
	m.pid = 5;
	m.valore1 = -1;
	m.valore2 = -1;

	msgsnd(queue_req, &m, sizeof(Messaggio)-sizeof(long), 0);

	pid = wait(&status);

	//Deallocazione risorse IPC
	msgctl(queue_req, IPC_RMID, NULL);
	msgctl(queue_rep, IPC_RMID, NULL);

	return 0;
}
