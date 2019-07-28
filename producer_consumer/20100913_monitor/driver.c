#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include"header.h"

//Tempo impiegato: 1h 20m ca
//NB PROBLEMI SUGLI ACQUISTI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! indice = 100!!!!

int main (int argc, char *argv[])
{
	pid_t pid;
	int i, k, status;

	//Allocazione risorse condivise
	Monitor* m;
	int id_mon = shmget(IPC_PRIVATE, sizeof(Monitor), IPC_CREAT|0664);
	m = (Monitor*)(shmat(id_mon, NULL, 0));

	Cond* c;
	int id_cond = shmget(IPC_PRIVATE, sizeof(Cond), IPC_CREAT|0664);
	c = (Cond*)(shmat(id_cond, NULL, 0));

	Scaffale* s;
	int id_buffer = shmget(IPC_PRIVATE, NELEM*sizeof(Scaffale), IPC_CREAT|0664);
	s = (Scaffale*)(shmat(id_buffer, NULL, 0));

	//Inizializzo le risorse condivise
	init_monitor(m, NUMCOND);

	//Inizialmente non ci sono scorte
	c->livello_scorte = 0;

	for(i = 0; i < NELEM; i++){
		s[i].id_fornitore = 0;
		s[i].stato = LIBERO;
	}
	//-----------------------------------------------

	int num_fornitori, num_clienti, num_processi;
	num_clienti = num_fornitori = 10;
	num_processi = num_fornitori + num_clienti;

	for(i = 0; i < num_processi; i++){
		pid = fork();
		if(pid == 0){
			if(i%2 == 0){
				printf("Fornitore <%d>\n", getpid());
				for(k = 0; k < 15; k++){
					fornitura(m, c, s);
					sleep(1);
				}
			}else{
				printf("Cliente <%d>\n", getpid());
				for(k = 0; k < 15; k++){
					acquisto(m, c, s);
					sleep(1);
				}
			}
			_exit(0);
		}
	}

	for(i = 0; i < num_processi; i++){
		pid = wait(&status);
		printf("Processo %d terminato con stato %d\n", pid, status);
	}

	//Deallocazione risorse condivise
	remove_monitor(m);
	shmctl(id_mon, IPC_RMID, NULL);
	shmctl(id_cond, IPC_RMID, NULL);
	shmctl(id_buffer, IPC_RMID, NULL);

	return 0;
}
