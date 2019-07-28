#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("<%d> - P4 ready...\n", getpid());
	sleep(1);

	int i;

	//Allocazione code di messaggi
	key_t chiave_operandi = ftok(PATH, CHAR4A), chiave_risultato = ftok(PATH, CHAR4R);
	int queue_operandi = msgget(chiave_operandi, IPC_CREAT|0664);
	int queue_risultato = msgget(chiave_risultato, IPC_CREAT|0664);

	//Ricezione e invio risposte
	Messaggio m;
	MessaggioRisposta r3;
	m.type = r3.type = 1;

	for(i = 0; i < NUMVOLTE; i++){

		sleep(1);

		printf("[P4] - In attesa di ricevere gli operandi\n");

		msgrcv(queue_operandi, &m, sizeof(Messaggio)-sizeof(long), 0, 0);

		printf("[P4] - Messaggio ricevuto\n");

		r3.risultato = (m.op1) * (m.op2);

		printf("[P4] - Invio risposta\n");

		msgsnd(queue_risultato, &r3, sizeof(MessaggioRisposta)-sizeof(long), IPC_NOWAIT);

		printf("[P4] - Risposta inviata\n");

	}
	return 0;
}
