#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("<%d> - P3 ready...\n", getpid());
	sleep(1);

	int i;

	//Allocazione code di messaggi
	key_t chiave_operandi3 = ftok(PATH, CHAR3A), chiave_risultato3 = ftok(PATH, CHAR3R);
	key_t chiave_operandi5 = ftok(PATH, CHAR5A), chiave_risultato5 = ftok(PATH, CHAR5R);
	key_t chiave_operandi6 = ftok(PATH, CHAR6A), chiave_risultato6 = ftok(PATH, CHAR6R);
	int queue_operandi3 = msgget(chiave_operandi3, IPC_CREAT|0664);
	int queue_risultato3 = msgget(chiave_risultato3, IPC_CREAT|0664);
	int queue_operandi5 = msgget(chiave_operandi5, IPC_CREAT|0664);
	int queue_risultato5 = msgget(chiave_risultato5, IPC_CREAT|0664);
	int queue_operandi6 = msgget(chiave_operandi6, IPC_CREAT|0664);
	int queue_risultato6 = msgget(chiave_risultato6, IPC_CREAT|0664);

	//Ricezione e invio risposte
	Messaggio4 m;
	MessaggioRisposta r2, r5, r4;
	Messaggio m1, m2;
	m.type = r2.type = r5.type = r4.type = m1.type = m2.type = 1;

	for(i = 0; i < NUMVOLTE; i++){

		sleep(1);

		printf("[P3] - In attesa di ricevere gli operandi\n");

		msgrcv(queue_operandi3, &m, sizeof(Messaggio4)-sizeof(long), 0, 0);

		m1.op1 = m.op1;
		m1.op2 = m.op2;

		m2.op1 = m.op3;
		m2.op2 = m.op4;

		//Invia operandi a p5 e p6
		msgsnd(queue_operandi5, &m1, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
		msgsnd(queue_operandi6, &m2, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);

		//Attende il risultato
		msgrcv(queue_risultato5, &r4, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);

		printf("[P3] - Ricezione risposta da P5\n");

		msgrcv(queue_risultato6, &r5, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);

		printf("[P3] - Ricezione risposta da P6\n");

		//Calcola il risultato da inviare a p1
		r2.risultato = (r4.risultato) * (r5.risultato);

		printf("[P3] - Invio risposta\n");

		//Invio risposta a p1
		msgsnd(queue_risultato3, &r2, sizeof(MessaggioRisposta)-sizeof(long), IPC_NOWAIT);

		printf("[P3] - Risposta inviata\n");

	}
	return 0;
}
