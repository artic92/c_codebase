#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include "monitor.h"
#include "semafori.h"

void initMonitor(Monitor *m, int numVar){
	int i;
	m->mutex = semget(CHIAVE, 1, PERMESSI);
	semctl(m->mutex, 0, SETVAL, 1);

	m->idCond = semget(CHIAVE, numVar, PERMESSI);
	for(i = 0; i < numVar; i++)
		semctl(m->idCond, i, SETVAL, 0);

	m->idCondCounts = shmget(CHIAVE, numVar*sizeof(int), PERMESSI);
	m->condCounts = (int *)shmat(m->idCondCounts, NULL, 0);
	for(i = 0; i < numVar; i++)
		m->condCounts[i] = 0;

	m->numCondVar = numVar;
}

void removeMonitor(Monitor *m){
	semctl(m->mutex, 0, IPC_RMID);
	semctl(m->idCond, 0, IPC_RMID);
	shmctl(m->idCondCounts, 0, IPC_RMID);
}

void enterMonitor(Monitor *m){
	waitSem(m->mutex, 0);
}

void leaveMonitor(Monitor *m){
	signalSem(m->mutex, 0);
}

void wait_cond(Monitor *m, int condid){
	(m->condCounts[condid])++;
	signalSem(m->mutex, 0);
	waitSem(m->idCond, condid);
	(m->condCounts[condid])--;
}

void signal_cond(Monitor *m, int condid){
	if(m->condCounts[condid] > 0)
		signalSem(m->idCond, condid);
	else
		signalSem(m->mutex, 0);
}
