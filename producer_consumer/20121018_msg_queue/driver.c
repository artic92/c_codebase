#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define NUMPROC 6

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	pid = fork();
	if(pid == 0){
		execvp("./p1", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./p2", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./p3", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./p4", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./p5", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./p6", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	for(i = 0; i < NUMPROC; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	return 0;
}
