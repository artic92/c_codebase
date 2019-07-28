//Processo SERVER p3

#include "header.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main (int argc, char *argv[])
{	
	printf("Processo P3 avviato\n");
	key_t chiave = ftok(PATH, LETTER);
	int queue, i, risp;
	double media1, media2, sommatoria1 = 0, sommatoria2 = 0;
	Messaggio m;
	
	queue = msgget(chiave, IPC_CREAT|0664);
	if(queue < 0){
		perror("Errore creazione coda!\n");
		_exit(-1);
	}
	
	for(i = 0; i < 22; i++){
		risp = msgrcv(queue, (void *) &m, sizeof(Messaggio)-sizeof(long), 0, 0);
		if(risp < 0){
			perror("Errore ricezione!\n");
			_exit(-1);
		}
		printf("Ricevuto messaggio dal processo <%d>, con valore <%d>\n", m.id, m.val);
		if(m.id == P1)
			sommatoria1 += m.val;
		else if(m.id == P2)
			sommatoria2 += m.val;
		else
			printf("Processo non riconosciuto!\n"); 
	}
	
	media1 = sommatoria1/11;
	media2 = sommatoria2/11;
	
	printf("<Media 1 = %d>\n", media1);
	printf("<Media 2 = %d>\n", media2);
	
	return 0;
}
