#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

int main (int argc, char *argv[])
{
	printf("<%d> - Server\n", getpid());
	sleep(1);

	int status;
	pid_t pid;

	//Allocazione code
	key_t chiave_comandi = ftok(PATH, CHARC);
	int id_queue_comandi = msgget(chiave_comandi, IPC_CREAT|0664);

	key_t chiave_risposte = ftok(PATH, CHARR);
	int id_queue_risposte = msgget(chiave_risposte, IPC_CREAT|0664);

	//Preparazione messaggio
	Messaggio_richiesta m1;
	Messaggio_risposta m2;

	char* comando;
	char* parametro;
	char* parametri[4];
	int i = 1;

	while(1){

		//Rimane in attesa di ricevere un messaggio
		msgrcv(id_queue_comandi, &m1, sizeof(Messaggio_richiesta)-sizeof(long), 0, 0);
		if(strcmp(m1.messaggio, "exit") == 0){
			printf("Server goodbye...\n\n");
			_exit(0);
		}

		//Esegue comando richiesto
		pid = fork();
		if(pid == 0){

			comando = strtok(m1.messaggio, " ");
			parametri[0] = comando;

			parametro = strtok(NULL, " ");
			while(parametro != NULL){
				parametri[i] = parametro;
				i++;
				parametro = strtok(NULL, " ");
			}

			parametri[3] = NULL;

			execvp(comando, parametri); //NB: l'array deve contenere anke il nome del file!!!

			perror("Errore exec!\n");
			_exit(0);

		}

		//Attende terminazione figlio
		pid = wait(&status);

		m2.processo = m1.processo;
		m2.val_ritorno = status;

		//Invia risposta al client
		msgsnd(id_queue_risposte, &m2, sizeof(Messaggio_risposta)-sizeof(long), 0);

	}

	return 0;
}
