#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

static int id = 0;

void inizializza(struct monitor_treno * m){
	m->id_treno = id++;
	m->stazione = 0;
	m->num_lettori = 0;
	m->occupato = 0;
	m->coda_lettori = 0;
	m->coda_scrittori = 0;
	pthread_mutex_init(&(m->mutex), NULL);
	pthread_cond_init(&(m->ok_lettura_cond), NULL);
	pthread_cond_init(&(m->ok_scrittura_cond), NULL);
}

void rimuovi(struct monitor_treno * m){
	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_lettura_cond));
	pthread_cond_destroy(&(m->ok_scrittura_cond));
}

int leggi_stazione(struct monitor_treno * m){
	pthread_mutex_lock(&(m->mutex));
	//Il lettore si blocca se ci sono scrittori
	while(m->occupato == 1 && m->num_lettori == 0){
		(m->coda_lettori)++;
		pthread_cond_wait(&(m->ok_lettura_cond), &(m->mutex));
		(m->coda_lettori)--;
	}
	(m->num_lettori)++;
	//Se è il primo lettore blocca gli scrittori
	if(m->num_lettori == 1)
		m->occupato = 1;

	pthread_cond_signal(&(m->ok_lettura_cond));
	pthread_mutex_unlock(&(m->mutex));

	int stazione = m->stazione;

	pthread_mutex_lock(&(m->mutex));
	(m->num_lettori)--;
	//Se è l'ultimo lettore sblocca lo scrittore
	if(m->num_lettori == 0){
		m->occupato = 0;
		pthread_cond_signal(&(m->ok_scrittura_cond));
	}
	pthread_mutex_unlock(&(m->mutex));
	return stazione;
}

void scrivi_stazione(struct monitor_treno * m, int stazione){
	pthread_mutex_lock(&(m->mutex));
	//Lo scrittore si blocca se ci sono lettori
	while(m->occupato == 1){
		(m->coda_scrittori)++;
		pthread_cond_wait(&(m->ok_scrittura_cond), &(m->mutex));
		(m->coda_scrittori)--;
	}
	m->occupato = 1;
	pthread_mutex_unlock(&(m->mutex));

	m->stazione = stazione;

	pthread_mutex_lock(&(m->mutex));
	if(m->coda_scrittori > 0)
		pthread_cond_signal(&(m->ok_scrittura_cond));
	else if(m->coda_lettori > 0)
		pthread_cond_signal(&(m->ok_lettura_cond));
	m->occupato = 0;
	pthread_mutex_unlock(&(m->mutex));
}

void* capotreno(void* c){
	struct monitor_treno* m = (struct monitor_treno*) c;
	int i, stazione = 1;

	for(i = 0; i < 10; i++){

		scrivi_stazione(m, stazione);

		printf("Capotreno del treno %d: modificata posizione treno %d in posizione %d\n", m->id_treno+1, m->id_treno+1, stazione);
		stazione++;

		sleep(3);

	}

	pthread_exit(NULL);
}

void* viaggiatore(void* c){
	struct monitor_treno* treno = (struct monitor_treno*) c;
	int i, stazione, sleep_time;

	for(i = 0; i < 3; i++){

		stazione = leggi_stazione(treno);

		printf("\nIl treno %d è alla stazione %d\n", treno->id_treno+1, stazione);

		sleep_time = (rand() % 6) + 1;

		printf("Attendo %d secondi\n\n", sleep_time);

		sleep(sleep_time);
	}

	pthread_exit(NULL);
}
