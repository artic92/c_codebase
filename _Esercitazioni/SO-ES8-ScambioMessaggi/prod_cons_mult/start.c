#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"


int main() {

	pid_t pid;
	int i;



        key_t queue  = ftok(FTOK_PATH_Q, FTOK_CHAR_Q);


        int id_queue = msgget(queue, IPC_CREAT|0644);

	if(id_queue < 0) {
		perror("Msgget fallita");
		exit(1);
	}


	pid = fork();

	if(pid==0) {

		execve("./p1", NULL, NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execve("./p2", NULL, NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	pid = fork();

	if(pid==0) {

		execve("./p3", NULL, NULL);

		perror("Exec fallita");
		exit(1);

	} else if(pid<0) {

		perror("Fork fallita");
		exit(1);
	}



	for(i=0; i<3; i++) {
		wait(NULL);
	}



        msgctl(id_queue, IPC_RMID, 0);
	return 0;
}

