#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

//Tempo impiegato: 1h:38m

int main (int argc, char *argv[])
{
	pid_t pid;
	int i, status;

	int num_client = 5;

	//Allocazioni risorse condivise, viene allocata la sola coda per comunicare col server
	key_t chiave = ftok(PATH, CHAR);
	int id_queue = msgget(chiave, IPC_CREAT|0664);

	//Generazione dei processi client
	for(i = 0; i < num_client; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Client\n", getpid());
			execvp("./client", NULL);

			perror("Errore exec!\n");
			_exit(0);
		}
	}

	//Generazione server e printer
	pid = fork();
	if(pid == 0){
		execvp("./server", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	pid = fork();
	if(pid == 0){
		execvp("./printer", NULL);

		perror("Errore exec!\n");
		_exit(0);
	}

	//Aspetto terminazione client
	for(i = 0; i < num_client; i++){
		pid = wait(&status);
		//printf("Processo client %d terminato con stato %d\n", pid, status);
	}

	//Invio messaggio di terminazione al server
	Messaggio m;
	m.type = TERMINATE;
	msgsnd(id_queue, &m, sizeof(Messaggio)-sizeof(long), 0);

	//Aspetto terminazione server e printer
	for(i = 0; i < 2; i++){
		pid = wait(&status);
		//printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Deallocazione risorse condivise
	msgctl(id_queue, 0, NULL);

	return 0;
}
