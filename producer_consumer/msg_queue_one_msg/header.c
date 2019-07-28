#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#include "header.h"

void sendSincr(int rts, int cts, int msg, Messaggio* mex){
	Messaggio tx, rx;
	tx.mtype = RTS;
	tx.mex = 0;
	if(msgsnd(rts, (void*)&tx, sizeof(Messaggio)-sizeof(long), 0) == -1)
		perror("Errore nell'invio della RTS!\n");
	#ifdef DEBUG
	else
		printf("DEBUG: Produttore: invio RTS\n");
	#endif
	if(msgrcv(cts, (void*)&rx, sizeof(Messaggio)-sizeof(long), CTS, 0) == -1)
		perror("Errore di ricezione della CTS!\n");
	if(msgsnd(msg, (void*)mex, sizeof(Messaggio)-sizeof(long), 0) == -1)
		perror("Errore invio messaggio!\n");	
	#ifdef DEBUG
	int ris = (*mex).mex;
	printf("DEBUG: valore del messaggio inviato: %d\n", ris);
	#endif
}

void receiveBloc(int rts, int cts, int msg, Messaggio* risp){
	Messaggio rx, tx;
	rx.mtype = CTS;
	rx.mex = 2014;
	if(msgrcv(rts, (void*)&tx, sizeof(Messaggio)-sizeof(long), RTS, 0) == -1)
		perror("Errore ricezione RTS!\n");
	if(msgsnd(cts, (void*)&rx, sizeof(Messaggio)-sizeof(long), 0) == -1)
		perror("Errore invio CTS!\n");
	#ifdef DEBUG
	else
		printf("DEBUG: Consumatore: invio CTS\n");
	#endif
	if(msgrcv(msg, (void*)risp, sizeof(Messaggio)-sizeof(long), MSG, 0) == -1)
		perror("Errore invio messaggio!\n");
	#ifdef DEBUG
	int ris = (*risp).mex;
	printf("DEBUG: valore del messaggio ricevuto: %d\n", ris);
	#endif
}
