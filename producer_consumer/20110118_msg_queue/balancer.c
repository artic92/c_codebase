#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("[%d] Balancer ready...\n", getpid());
	
	key_t chiave_client = ftok(PATH, CHARB);
	int queue_client = msgget(chiave_client, IPC_CREAT|0664);
	
	key_t chiave_server = ftok(PATH, CHARS);
	int queue_server = msgget(chiave_server, IPC_CREAT|0664);
	
	int indice = 0;
	Messaggio mex_client, mex_server;
	
	while(1){
		
		msgrcv(queue_client, &mex_client, sizeof(Messaggio)-sizeof(long), 0, 0);
		
		if(mex_client.type == TERMINATE){
			mex_server.pid = TERMINATE;
			
			mex_server.type = SERVER1;
			msgsnd(queue_server, &mex_server, sizeof(Messaggio)-sizeof(long), 0);
			
			mex_server.type = SERVER2;
			msgsnd(queue_server, &mex_server, sizeof(Messaggio)-sizeof(long), 0);
			
			mex_server.type = SERVER3;
			msgsnd(queue_server, &mex_server, sizeof(Messaggio)-sizeof(long), 0);
			
			printf("[Balancer] Goodbye...\n");
			_exit(0);
		}
		
		//Invia in resto modulo 3 ai server
		if(indice % 3 == 0){
			mex_server.type = SERVER1;
			mex_server.pid = mex_client.pid;
			indice++;
		}else if(indice % 3 == 1){
			mex_server.type = SERVER2;
			mex_server.pid = mex_client.pid;
			indice++;			
		}else if(indice % 3 == 2){
			mex_server.type = SERVER3;
			mex_server.pid = mex_client.pid;
			indice++;			
		}else
			printf("Errore invio ai servers!\n");
			
		msgsnd(queue_server, &mex_server, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
		
	}
	
	return 0;
}
