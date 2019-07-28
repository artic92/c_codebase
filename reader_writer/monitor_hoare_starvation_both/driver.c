//PROBLEMA LETT/SCRITT CON STARVATION DEGLI SCRITTORI IMPLEMENTATO CON MONITOR di Hoare

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "header.h"

int main (int argc, char *argv[])
{
	pid_t pid;
	int i, j, status;

	int id_buf;
	msg *buffer;
	id_buf = shmget(IPC_PRIVATE, sizeof(msg), IPC_CREAT|0664);
	buffer = (msg*)(shmat(id_buf, NULL, 0));

	int id_cond;
	cond* c;
	id_cond = shmget(IPC_PRIVATE, sizeof(cond), IPC_CREAT|0664);
	c = (cond*)(shmat(id_cond, NULL, 0));
	c->occupato = 0;
	c->num_lettori = 0;

	int id_mon;
	Monitor* m;
	id_mon = shmget(IPC_PRIVATE, sizeof(Monitor), IPC_CREAT|0664);
	m = (Monitor*)(shmat(id_mon, NULL, 0));
	init_monitor(m, NUM_CONDITIONS);

	int num_processi, num_lettori, num_scrittori;
	num_lettori = 4;
	num_scrittori = 4;
	num_processi = num_lettori +  num_scrittori;

	for(i = 0; i < num_processi; i++){
		pid = fork();
		if(pid == 0){
			if(i%5 == 0){
				printf("%d - Processo scrittore\n", getpid());
				sleep(1);
				scrittura(m, c, buffer);
			}else{
				printf("%d - Processo lettore\n", getpid());
				sleep(1);
				lettura(m, c, buffer);
			}
			_exit(0);
		}
	}

	for(j = 0; j < num_processi; j++){
		pid = wait(&status);
		if(pid < 0)
			perror("Errore!\n");
		else
			printf("Processo %d terminato con stato %d\n", pid, status);
	}

	remove_monitor(m);
	shmctl(id_mon, IPC_RMID, 0);
	shmctl(id_cond, IPC_RMID, 0);
	shmctl(id_buf, IPC_RMID, 0);

	return 0;
}
