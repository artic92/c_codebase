#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "header.h"

void inizializza(struct Monitor* m){
	int i;

	for(i = 0; i < NUMDVD; i++){
		m->dvd[i].stato = DISPONIBILE;
	}

	m->dvd[0].identificativo_film = 1;
	m->dvd[0].identificativo_copia = 1;
	m->dvd[1].identificativo_film = 1;
	m->dvd[1].identificativo_copia = 2;
	m->dvd[2].identificativo_film = 2;
	m->dvd[2].identificativo_copia = 1;
	m->dvd[3].identificativo_film = 2;
	m->dvd[3].identificativo_copia = 2;
	m->dvd[4].identificativo_film = 3;
	m->dvd[4].identificativo_copia = 1;
	m->dvd[5].identificativo_film = 3;
	m->dvd[5].identificativo_copia = 2;

	for(i = 0; i < 3; i++)
		m->ok_affitto[i] = 2;//Ogni DVD ha due copie

	pthread_mutex_init(&(m->mutex), NULL);
	for(i = 0; i < 3; i++){
		pthread_cond_init(&(m->ok_affitto_cond[i]), NULL);
	}

}
void distruggi(struct Monitor* m){
	int i;

	pthread_mutex_destroy(&(m->mutex));
	for(i = 0; i < 3; i++){
		pthread_cond_destroy(&(m->ok_affitto_cond[i]));
	}
}

int affitta(struct Monitor * m, int id_film){
	pthread_mutex_lock(&(m->mutex));
	//Il processo si deve bloccare se non ci sono copie disponibili del film id_film
	while(m->ok_affitto[id_film - 1] == 0){
		printf("Mi metto in attesa della disponibilità del film %d\n\n", id_film);
		pthread_cond_wait(&(m->ok_affitto_cond[id_film - 1]), &(m->mutex));
		printf("Il film %d è nuovamente disponibile\n\n", id_film);
	}
	(m->ok_affitto[id_film - 1])--;

	//Ricerca di una copia disponibile del DVD id_film
	int trovato = 0, indice = 0;
	while(indice < NUMDVD && trovato == 0){
		if((m->dvd[indice].identificativo_film == id_film) && (m->dvd[indice].stato == DISPONIBILE))
			trovato = 1;
		else
			indice++;
	}

	//Affitto del DVD
	m->dvd[indice].stato == AFFITTATO;
	int copia = m->dvd[indice].identificativo_copia;

	pthread_mutex_unlock(&(m->mutex));
	return copia;
}

void restituisci(struct Monitor * m, int id_film, int id_copia){
	pthread_mutex_lock(&(m->mutex));
	//Ricerca la copia id_copia del DVD id_film
	int indice = 0, trovato = 0;
	while(indice < NUMDVD && trovato == 0){
		if((m->dvd[indice].identificativo_film == id_film) && (m->dvd[indice].identificativo_copia == id_copia))
			trovato = 1;
		else
			indice++;
	}

	//Restituzione del DVD
	m->dvd[indice].stato = DISPONIBILE;

	//Sblocco eventuali processi bloccati in attesa che una copia del DVD id_film si liberi
	(m->ok_affitto[id_film - 1])++;//Signal(SPAZIO_DISP)
	pthread_cond_signal(&(m->ok_affitto_cond[id_film - 1]));
	pthread_mutex_unlock(&(m->mutex));
}

void stampa(struct Monitor * m){
	int i;
	for(i = 0; i < NUMDVD; i++){
		printf("Film %d:\n", m->dvd[i].identificativo_film);
		printf("\tCopia: %d\n", m->dvd[i].identificativo_copia);
		if(m->dvd[i].stato == DISPONIBILE)
			printf("\tStato: DISPONIBILE\n");
		else if(m->dvd[i].stato == AFFITTATO)
			printf("\tStato: AFFITTATO\n");
		}
	printf("\n\n");
}

void* cliente(void * c){
	struct Monitor* m = (struct Monitor*) c;
	int i;

	for(i = 0; i < 3; i++){
		int copia = affitta(m, i+1);
		printf("Richiesta della copia %d del film %d\n\n", copia, i+1);
		sleep(1);
		printf("Restituzione della copia %d del film %d\n\n", copia, i+1);
		restituisci(m, i+1, copia);
	}

	pthread_exit(NULL);
}

void* visualizzatore(void *c){
	struct Monitor* m = (struct Monitor*) c;
	int k;

	for(k = 0; k < 10; k++){
		pthread_mutex_lock(&(m->mutex));
		stampa(m);
		pthread_mutex_unlock(&(m->mutex));
		sleep(1);
	}

	pthread_exit(NULL);
}
