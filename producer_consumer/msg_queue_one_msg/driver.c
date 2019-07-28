//PROBLEMA PRODUTTORE CONSUMATORE IMPLEMENTATO CON PROTOCOLLO SINCRONO

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "header.h"

int main (int argc, char *argv[])
{
	int permessi = (IPC_CREAT | IPC_EXCL | 0666), elimina = IPC_RMID;
	int chiave = IPC_PRIVATE, rts, cts, msg, i, pid, stato, p, num_produttori=1, num_consumatori=1;
	int num_processi=num_produttori+num_consumatori;

	Messaggio mex, risp;
	mex.mtype = MSG;
	mex.mex = 2014;
	risp.mtype = MSG;
	risp.mex = 0;

	rts = msgget(chiave, permessi);
	cts = msgget(chiave, permessi);
	msg = msgget(chiave, permessi);

	if (rts < 0 | cts < 0 | msg < 0)
	{
		perror("Creazione non riuscita! Uscita in corso...\n");
		return 0;
	}

	for(i = 0; i < num_processi; i++){
		#ifdef DEBUG
		printf("DEBUG: il padre crea figli!\n");
		#endif
		pid = fork();
		if(pid == 0){
			if(i%2 == 0){
				for (p = 0; p < num_produzioni; p++){
					sleep(1);
					mex.mex++;
					if(i == 2){
						mex.mex++;
						}
					printf("Produttore %d: produco un messaggio %d\n", i, mex.mex);
					sendSincr(rts, cts, msg, &mex);
					printf("Produttore %d: messaggio %d inviato!\n", i, mex.mex);
					}
				}else{
					for (p = 0; p < num_consumi; p++){
						sleep(3);
/*						printf("Consumatore %d: in attesa del messaggio...\n", i);*/
						receiveBloc(rts, cts, msg, &risp);
						printf("Consumatore %d: messaggio ricevuto!\n", i);
						printf("Consumatore %d: il messaggio ricevuto e' %d\n", i, risp.mex);
					}
				}
				_exit(0);
			}
	}

	for(i = 0; i < num_processi; i++)
		wait(NULL);

	msgctl(msg, elimina, NULL);
	msgctl(rts, elimina, NULL);
	msgctl(cts, elimina, NULL);
	return 0;
}
