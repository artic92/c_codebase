#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

#define NUMPRODUZIONI 15

void inizializzaMonitor(struct Monitor* m){
	int i;

	for(i = 0; i < NUMCONSEGNE; i++){
		m->c[i].pizza = 0;
		m->c[i].stato = VUOTO;
		m->c[i].cliente = 0;
	}

	m->ok_produzione = NUMCONSEGNE;
	m->ok_consegna = 0;

	pthread_mutex_init(&(m->mutex), NULL);
	pthread_cond_init(&(m->ok_consegna_cond), NULL);
	pthread_cond_init(&(m->ok_produzione_cond), NULL);
}

void distruggiMonitor(struct Monitor* m){
	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_consegna_cond));
	pthread_cond_destroy(&(m->ok_produzione_cond));
}

//Ricerca una consegna nello stato VUOTO, la pone in PRONTO
void aggiungi_consegna(struct Monitor* m, int pizza, int cliente){
	pthread_mutex_lock(&(m->mutex));
	//Se non ci sono consegne disponibili il thread si mette in attesa
	while(m->ok_produzione == 0)
		pthread_cond_wait(&(m->ok_produzione_cond), &(m->mutex));
	(m->ok_produzione)--;//Wait(SPAZIO_DISP)

	int indice = 0;
	while(indice < NUMCONSEGNE && m->c[indice].stato != VUOTO)
		indice++;

	m->c[indice].pizza = pizza;
	m->c[indice].stato = PRONTO;
	m->c[indice].cliente = cliente;

	//Sveglia eventuali fattorini in attesa
	(m->ok_consegna)++;//Signal(MEX_DISP)
	pthread_cond_signal(&(m->ok_consegna_cond));
	pthread_mutex_unlock(&(m->mutex));
}

//Ricerca la prima consegna nello stato PRONTO, la pone nello stato IN_CONSEGNA e restituirne l'indice al chiamante
int inizia_consegna(struct Monitor* m){
	pthread_mutex_lock(&(m->mutex));
	//Il fattorino si deve bloccare se non ci sono pizze da consegnare
	while(m->ok_consegna == 0)
		pthread_cond_wait(&(m->ok_consegna_cond), &(m->mutex));
	(m->ok_consegna)--;//Wait(MEX_DISP)

	int indice = 0;
	while(indice < NUMCONSEGNE && m->c[indice].stato != PRONTO)
		indice++;

	m->c[indice].stato = IN_CONSEGNA;

	pthread_mutex_unlock(&(m->mutex));
	return indice;
}

//Pone lo stato della consegna indice_consegna in VUOTO
void fine_consegna(struct Monitor* m, int indice_consegna){
	pthread_mutex_lock(&(m->mutex));

	m->c[indice_consegna].stato = VUOTO;

	//Sveglia eventuali pizzaioli in attesa
	(m->ok_produzione)++;
	pthread_cond_signal(&(m->ok_produzione_cond));
	pthread_mutex_unlock(&(m->mutex));
}

//Flussi di esecuzione per i threads fattorino e i threads pizzaiolo
void* fattorino(void* c){
	struct Monitor* m = (struct Monitor*) c;
	int i;

	srand(time(NULL));

	for(i = 0; i < NUMCONSEGNE; i++){

		int indice = inizia_consegna(m);

		//Attende un tempo da 1 a 3 secondi
		int sleep_time = (rand() % 4) + 1;

		printf("Sto consegnando la pizza %d al cliente %d; tempo necessario: %d min\n\n", indice, m->c[indice].cliente, sleep_time);

		sleep(sleep_time);

		printf("Consegna terminata\n\n");

		fine_consegna(m, indice);

	}

	pthread_exit(NULL);
}

void* pizzaiolo(void* c){
	struct Monitor* m = (struct Monitor*) c;
	int i;

	srand(time(NULL));

	for(i = 0; i < NUMPRODUZIONI; i++){

		//Immette un numero casuale da 1 a 2
		int pizza = (rand() % 2) + 1;

		//Scelgo numeri casuali finchè non escono quelli giusti
		while(pizza < 1 && pizza > 2){
			printf("Tipo di pizza inesistente(%d), scelgo ancora\n\n", pizza);
			pizza = (rand() % 1) + 1;
		}

		int cliente = i;

		aggiungi_consegna(m, pizza, cliente);

		if(pizza == 1)
			printf("Prodotta pizza Margherita\n\n");
		else
			printf("Prodotto Calzone\n\n");

		printf("Aggiunta consegna da effettuare al cliente %d\n\n", cliente);
	}

	pthread_exit(NULL);
}
