#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdio.h>
#include "header.h"

//Problema produttore/consumatore implementato con monitor signal_and_urgent_wait

int main (int argc, char *argv[])
{
	pid_t pid;
	int status, id_buf, i, j;
	
	int *testa, *coda;
	msg *buffer;
	id_buf = shmget(IPC_PRIVATE, (DIM*sizeof(msg)) + (2*sizeof(int)), IPC_CREAT|0664);
	testa = (int*)(shmat(id_buf, NULL, 0));
	coda = testa + 1;
	buffer = (msg*)(coda + 1);
	*testa = 0;
	*coda = 0;
	
	int num_produttori, num_consumatori, num_processi;
	num_produttori = 4;
	num_consumatori = 4;
	num_processi = num_produttori + num_consumatori;
	
	Monitor m;
	init_monitor(&m, NUM_CONDITIONS);
	
	int id_cond;
	cond *c;
	id_cond = shmget(IPC_PRIVATE, sizeof(cond), IPC_CREAT|0664);
	c = (cond*)(shmat(id_cond, NULL, 0));
	c->ok_produzione = DIM;
	c->ok_consumo = 0;
	
	for(i = 0; i < num_processi; i++){
		pid = fork();
		if(pid == 0){
			if(i%2 == 0){
				printf("<%d> - Produttore\n", getpid());
				produttore(&m, c, buffer, testa);
			}
			else{
				printf("<%d> - Consumatore\n", getpid());
				sleep(2);
				consumatore(&m, c, buffer, coda);
			}
			_exit(0);
		}
	}
	
	for(i = 0; i < num_processi; i++){
		pid = wait(&status);
		if(pid < 0)
			perror("Errore!\n");
		else
			printf("Figlio n.ro %d terminato con stato %d\n", pid, status);
	}
	
	shmctl(id_buf, IPC_RMID, 0);
	shmctl(id_cond, IPC_RMID, 0);
	remove_monitor(&m);
	
	return 0;
}
