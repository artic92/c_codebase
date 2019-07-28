// Programma di avvio dei processi

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include "header.h"

int main (int argc, char *argv[])
{
	int chiave = ftok(PATH, LETTER);
	int queue, i;
	pid_t pid;

	queue = msgget(chiave, IPC_CREAT|IPC_EXCL|0664);
	if(queue < 0){
		perror("Errore creazione coda!\n");
		_exit(-1);
	}

	printf("ID QUEUE: %d\n", queue);

	pid = fork();
	if(pid == 0){
		execlp("./p1", NULL, NULL);

		printf("Errore exec!\n");
		_exit(-1);
	}

	pid = fork();
	if(pid == 0){
		execlp("./p2", NULL, NULL);

		printf("Errore exec!\n");
		_exit(-1);
	}

	pid = fork();
	if(pid == 0){
		execlp("./p3", NULL, NULL);

		printf("Errore exec");
		_exit(-1);
	}

	for(i = 0; i < 3; i++)
		wait(NULL);

	msgctl(queue, IPC_RMID, NULL);

	return 0;
}
