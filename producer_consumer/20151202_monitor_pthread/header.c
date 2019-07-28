#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"

#define NUMRICHIEDENZE 3
#define NUMELABORAZIONI 6

void inizializza(MonitorElaborazioni* m){
	int i;

	for(i = 0; i < NUMBUFFER; i++){
		m->elaborazioni[i].totale_operandi = 0;
		m->stato[i] = LIBERO;
	}

	//Inizialmente il vettore di buffer è libero
	m->ok_produzione = NUMBUFFER;
	m->ok_consumo = 0;

	pthread_mutex_init(&(m->mutex), NULL);
	pthread_cond_init(&(m->ok_produzione_cond), NULL);
	pthread_cond_init(&(m->ok_consumo_cond), NULL);
}

void distruggi(MonitorElaborazioni* m){
	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_produzione_cond));
	pthread_cond_destroy(&(m->ok_consumo_cond));
}

//Effettua la somma degli operandi del più piccolo buffer OCCUPATO attendendo un tempo pari al numero di operandi
void* elaborazione(void* v){
	MonitorElaborazioni* m = (MonitorElaborazioni*) v;
	int j;

	for(j = 0; j < NUMELABORAZIONI; j++){
		int indice = inizioElaborazione(m);

		int operandi = m->elaborazioni[indice].totale_operandi;
		int somma = 0, i;

		printf("[Elaboratore] Mi metto in attesa per %d secondi\n\n", operandi);
		sleep(operandi);

		for(i = 0; i < operandi; i++){
			somma += m->elaborazioni[indice].operandi[i];
			printf("Buffer %d; operando[%d]: %d\n\n", indice, i, m->elaborazioni[indice].operandi[i]);
		}

		printf("[Elaboratore] La somma del buffer %d è: %d\n\n", indice, somma);

		fineElaborazione(m, indice);
	}
	pthread_exit(NULL);
}

//Inserisce per 3 volte un numero casuale (da 2 a 4) di operandi (valori da 0 a 10) nel primo buffer LIBERO indicato da indice
void* richiedente(void* v){
	MonitorElaborazioni* m = (MonitorElaborazioni*) v;
	int j;

	for(j = 0; j < NUMRICHIEDENZE; j++){
		int indice = inizioProduzione(m);

		int i;
		srand(time(NULL) + j);
		int num_operandi = (rand() % 4) + 1;

		if(num_operandi < 2 || num_operandi > 4){
			printf("Errore scelta operandi(%d)\n\n", num_operandi);
			m->stato[indice] = LIBERO;
			pthread_exit(NULL);
		}

		printf("[Richiedente] Inserisco %d operandi nel buffer %d\n\n", num_operandi, indice);
		for(i = 0; i < num_operandi; i++){
			m->elaborazioni[indice].operandi[i] = (rand() % 10) + 1;
		}

		m->elaborazioni[indice].totale_operandi = num_operandi;

		fineProduzione(m, indice);
	}
	pthread_exit(NULL);
}

//Ricerca il primo buffer LIBERO, pone il suo stato in IN_USO e restituisce l'indice al produttore
int inizioProduzione(MonitorElaborazioni* m){
	pthread_mutex_lock(&(m->mutex));
	while(m->ok_produzione == 0)//Se il buffer è pieno il produttore si deve bloccare
		pthread_cond_wait(&(m->ok_produzione_cond), &(m->mutex));
	(m->ok_produzione)--;//Wait(SPAZIO_DISP)

	int indice = 0;
	while(indice < NUMBUFFER && m->stato[indice] != LIBERO)
		indice++;

	m->stato[indice] = IN_USO;
	pthread_mutex_unlock(&(m->mutex));
	return indice;
}

//Setta il buffer nella posizione indice in OCCUPATO, sveglia eventuali consumatori in attesa
void fineProduzione(MonitorElaborazioni* m, int indice){
	pthread_mutex_lock(&(m->mutex));
	m->stato[indice] = OCCUPATO;
	(m->ok_consumo)++;//Signal(MEX_DISP)
	pthread_cond_broadcast(&(m->ok_consumo_cond));
	pthread_mutex_unlock(&(m->mutex));
}

//Ricerca il primo buffer OCCUPATO con il MINORE NUMERO di OPERANDI, pone il suo stato in IN_USO e restituisce l'indice al produttore
int inizioElaborazione(MonitorElaborazioni* m){
	pthread_mutex_lock(&(m->mutex));
	//Il consumatore si deve bloccare se il buffer è vuoto
	while(m->ok_consumo == 0)
		pthread_cond_wait(&(m->ok_consumo_cond), &(m->mutex));
	(m->ok_consumo)--;//Wait(MEX_DISP)

	//Ricerca il buffer col minor numero di operandi
	int indice = 0, trovato = 0, min_operandi = 1000;
	while(indice < NUMBUFFER && trovato == 0){
		if(m->stato[indice] == OCCUPATO){
			if(m->elaborazioni[indice].totale_operandi < min_operandi)
				trovato = 1;
			else
				indice++;
		}else
			indice++;
	}

	pthread_mutex_unlock(&(m->mutex));
	return indice;
}

//Setta il buffer nella posizione indice in LIBERO, sveglia eventuali produttori in attesa
void fineElaborazione(MonitorElaborazioni* m, int indice){
	pthread_mutex_lock(&(m->mutex));
	m->stato[indice] = LIBERO;
	(m->ok_produzione)++;//Signal(SPAZIO_DISP)
	pthread_cond_broadcast(&(m->ok_produzione_cond));
	pthread_mutex_unlock(&(m->mutex));
}
