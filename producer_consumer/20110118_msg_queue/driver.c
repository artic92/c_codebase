#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

//Tempo impiegato 1h:38m

int main (int argc, char *argv[])
{
	pid_t pid;
	int i, status;

	//Inizializzazione code per la comunicazione col server
	key_t chiave_balancer = ftok(PATH, CHARB);
	int queue_balancer = msgget(chiave_balancer, IPC_CREAT|0664);

	key_t chiave_server = ftok(PATH, CHARS);
	int queue_server = msgget(chiave_server, IPC_CREAT|0664);

	int num_client = 8, num_server = 3;

	//Generazione client
	for(i = 0; i < num_client; i++){
		pid = fork();
		if(pid == 0){
			printf("<%d> - Client\n", getpid());
			execvp("./client", NULL);

			perror("Exec fallita!\n");
			_exit(0);
		}
	}

	//Generazione server
	for(i = 0; i < num_server; i++){
		pid = fork();
		if(pid == 0){
			if(i == 0){
				execvp("./server1", NULL);

				perror("Exec fallita!\n");
				_exit(0);
			}else if(i == 1){
				execvp("./server2", NULL);

				perror("Exec fallita!\n");
				_exit(0);
			}else{
				execvp("./server3", NULL);

				perror("Exec fallita!\n");
				_exit(0);
			}
		}
	}

	//Generazione balancer
	pid = fork();
	if(pid == 0){
		execvp("./balancer", NULL);

		perror("Exec fallita!\n");
		_exit(0);
	}

	//Aspetto terminazione client
	for(i = 0; i < num_client; i++){
		pid = wait(&status);
		printf("Processo client <%d> terminato con stato %d\n", pid, status);
	}

	//Invio messaggio di terminazione al balancer che a sua volta lo inoltrerà ai servers
	Messaggio m;
	m.type = TERMINATE;
	msgsnd(queue_balancer, &m, sizeof(Messaggio)-sizeof(long), 0);

	//Aspetto la terminazione del balancer e dei servers
	for(i = 0; i < (num_server + 1); i++){
		pid = wait(&status);
		printf("Processo <%d> terminato con stato %d\n", pid, status);
	}

	//Deallocazione code
	msgctl(queue_balancer, IPC_RMID, NULL);
	msgctl(queue_server, IPC_RMID, NULL);

	return 0;
}
