#include "semafori.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void waitSem(int semid, int semnum){
	struct sembuf ops;
	ops.sem_num = semnum;
	ops.sem_op = -1;
	ops.sem_flg = 0;
	semop(semid, &ops, 1);
}

void signalSem(int semid, int semnum){
	struct sembuf ops;
	ops.sem_num = semnum;
	ops.sem_op = 1;
	ops.sem_flg = 0;
	semop(semid, &ops, 1);
}

