#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

int main (int argc, char *argv[])
{
	int i, status;
	pid_t pid;

	key_t id_shared;
	msg* buffer;
	id_shared = shmget(IPC_PRIVATE, sizeof(msg), IPC_CREAT|0664);
	buffer = (msg*)(shmat(id_shared, NULL, 0));

	key_t id_cond;
	cond* c;
	id_cond = shmget(IPC_PRIVATE, sizeof(cond), IPC_CREAT|0664);
	c = (cond*)(shmat(id_cond, NULL, 0));
	c->num_lettori = 0;
	c->occupato = 0;

	key_t id_mon;
	Monitor* m;
	id_mon = shmget(IPC_PRIVATE, sizeof(Monitor), IPC_CREAT|0664);
	m = (Monitor*)(shmat(id_mon, NULL, 0));
	init_monitor(m, NUMCOND);

	int num_lettori, num_scrittori, num_processi;
	num_lettori = num_scrittori = 4;
	num_processi = num_lettori + num_scrittori;

	for(i = 0; i < num_processi; i++){
		pid = fork();
		if(pid == 0){
			if(i%2 == 0){
				printf("<%d> - Scrittore\n", getpid());
				sleep(1);
				scrittore(m, c, buffer);
			}else{
				printf("<%d> - Lettore\n", getpid());
				sleep(1);
				lettore(m, c, buffer);
			}
			_exit(0);
		}
	}

	for(i = 0; i < num_processi; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	remove_monitor(m);
	shmctl(id_shared, IPC_RMID, NULL);

	return 0;
}
