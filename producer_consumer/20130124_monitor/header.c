#include <unistd.h>
#include <stdio.h>
#include "header.h"

void inizializza(struct Monitor* m){
	int i;

	for(i = 0; i < TOTDVD; i++)
		m->dvd[i].stato = DISPONIBILE;

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

	//Inizialmente tutte le copie dei film sono disponibili
	for(i = 0; i < 3; i++)
		m->ok_affitto[i] = 2;

	init_monitor(&(m->mon), NUMCOND);
}

void distruggi(struct Monitor* m){
	remove_monitor(&(m->mon));
}

//Restituisce l'id_copia del film id_film ponendo il suo stato in AFFITTATO, se non la trova si mette in attesa
int affitta(struct Monitor * m, int id_film){
	enter_monitor(&(m->mon));
	//Se nessuna copia del film id_film è disponibile il processo si blocca;
	if(m->ok_affitto[id_film - 1] == 0){
		printf("<%d> - Mi metto in attesa della disponibilità del film %d\n\n", getpid(), id_film);
		wait_condition(&(m->mon), id_film - 1);
		printf("<%d> - Il film %d è nuovamente disponibile\n\n", getpid(), id_film);
	}
	(m->ok_affitto[id_film - 1])--;

	//Trova la prima copia del film id_film disponibile
	//NB c'è sempre almeno una copia disponibile altrimenti il processo si sarebbe bloccato prima
	int indice = 0, trovato = 0;
	while(indice < TOTDVD && trovato == 0){
		if((m->dvd[indice].identificativo_film == id_film) && (m->dvd[indice].stato == DISPONIBILE))
			trovato = 1;
		else
			indice++;
	}

	m->dvd[indice].stato = AFFITTATO;
	int copia = m->dvd[indice].identificativo_copia;

	leave_monitor(&(m->mon));
	return copia;
}
//Cambia lo stato dell'id_copia riferito ad id_film in DISPONIBILE e risveglia un eventuale processo in attesa
void restituisci(struct Monitor * m, int id_film, int id_copia){
	enter_monitor(&(m->mon));
	//Cerca la copia id_copia del film id_film
	int indice = 0, trovato = 0;
	while(indice < TOTDVD && trovato == 0){
		if((m->dvd[indice].identificativo_film == id_film) && (m->dvd[indice].identificativo_copia == id_copia))
			trovato = 1;
		else
			indice++;
	}

	m->dvd[indice].stato = DISPONIBILE;

	//Risveglia eventuali processi in attesa
	(m->ok_affitto[id_film - 1])++;
	signal_condition(&(m->mon), id_film - 1);
}

void stampa(struct Monitor * m){
	int i;

	enter_monitor(&(m->mon));
	for(i = 0; i < 6; i++){
		printf("Film %d:\n", m->dvd[i].identificativo_film);
		printf("\tCopia: %d\n", m->dvd[i].identificativo_copia);
		if(m->dvd[i].stato == DISPONIBILE)
			printf("\tStato: DISPONIBILE\n");
		else if(m->dvd[i].stato == AFFITTATO)
			printf("\tStato: AFFITTATO\n");
	}
	leave_monitor(&(m->mon));
	printf("\n");
}

