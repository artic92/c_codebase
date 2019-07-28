#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "header.h"

#define NUMOP 8

int main (int argc, char *argv[])
{
	printf("<%d> - P1 ready...\n", getpid());
	sleep(1);

	int i, k;
	int a, b, c, d, e, f, g, h, risultato;

	//Allocazione code di messaggi
	key_t chiave_operandi2 = ftok(PATH, CHAR2A), chiave_risultato2 = ftok(PATH, CHAR2R);
	key_t chiave_operandi3 = ftok(PATH, CHAR3A), chiave_risultato3 = ftok(PATH, CHAR3R);
	key_t chiave_operandi4 = ftok(PATH, CHAR4A), chiave_risultato4 = ftok(PATH, CHAR4R);
	key_t chiave_operandi5 = ftok(PATH, CHAR5A), chiave_risultato5 = ftok(PATH, CHAR5R);
	key_t chiave_operandi6 = ftok(PATH, CHAR6A), chiave_risultato6 = ftok(PATH, CHAR6R);
	int queue_operandi2 = msgget(chiave_operandi2, IPC_CREAT|0664);
	int queue_risultato2 = msgget(chiave_risultato2, IPC_CREAT|0664);
	int queue_operandi3 = msgget(chiave_operandi3, IPC_CREAT|0664);
	int queue_risultato3 = msgget(chiave_risultato3, IPC_CREAT|0664);
	int queue_operandi4 = msgget(chiave_operandi4, IPC_CREAT|0664);
	int queue_risultato4 = msgget(chiave_risultato4, IPC_CREAT|0664);
	int queue_operandi5 = msgget(chiave_operandi5, IPC_CREAT|0664);
	int queue_risultato5 = msgget(chiave_risultato5, IPC_CREAT|0664);
	int queue_operandi6 = msgget(chiave_operandi6, IPC_CREAT|0664);
	int queue_risultato6 = msgget(chiave_risultato6, IPC_CREAT|0664);

	//Ricezione e invio risposte
	Messaggio4 m3;
	MessaggioRisposta r1, r2, r3;
	Messaggio m2, m4;
	m3.type = r1.type = r2.type = r3.type = m2.type = m4.type = 1;

	srand(time(NULL));

	for(i = 0; i < NUMVOLTE; i++){

		sleep(1);

		//Genera valori
		m2.op1 = (rand() % 10);
		m2.op2 = (rand() % 10);

		m3.op1 = (rand() % 10);
		m3.op2 = (rand() % 10);
		m3.op3 = (rand() % 10);
		m3.op4 = (rand() % 10);

		m4.op1 = (rand() % 10);
		m4.op2 = (rand() % 10);

		//Invia operandi a p2, p3 e p4
		msgsnd(queue_operandi2, &m2, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
		msgsnd(queue_operandi3, &m3, sizeof(Messaggio4)-sizeof(long), IPC_NOWAIT);
		msgsnd(queue_operandi4, &m4, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);

		printf("[P1] - In attesa dei risultati...\n");

		//Attende il risultato
		msgrcv(queue_risultato2, &r1, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);
		msgrcv(queue_risultato4, &r3, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);
		msgrcv(queue_risultato3, &r2, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);

		printf("[P1] - Calcolo risultato...\n");

		//Calcola il risultato
		risultato = (r1.risultato) + (r2.risultato) + (r3.risultato);

		//Stampa risultato
		printf("\nIl risultato dell'espressione y = (%d*%d+(%d+%d)*(%d+%d)+%d*%d) e' y = %d\n\n", m2.op1, 			m2.op2, m3.op1, m3.op2, m3.op3, m3.op4, m4.op1, m4.op2, risultato);
	}

	//Deallocazione code di messaggi con p2, p3, p4, p5 e p6
	msgctl(queue_operandi2, IPC_RMID, NULL);
	msgctl(queue_risultato2, IPC_RMID, NULL);
	msgctl(queue_operandi3, IPC_RMID, NULL);
	msgctl(queue_risultato3, IPC_RMID, NULL);
	msgctl(queue_operandi4, IPC_RMID, NULL);
	msgctl(queue_risultato4, IPC_RMID, NULL);
	msgctl(queue_operandi5, IPC_RMID, NULL);
	msgctl(queue_risultato5, IPC_RMID, NULL);
	msgctl(queue_operandi6, IPC_RMID, NULL);
	msgctl(queue_risultato6, IPC_RMID, NULL);

	return 0;
}
