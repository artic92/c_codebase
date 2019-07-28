#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

void inizializza(LettScrittMeteo* m){
	init_monitor(&(m->monitor), NUMCOND);
	m->occupato = 0;
	m->num_lettori = 0;
}

void distruggi(LettScrittMeteo* m){
	remove_monitor(&(m->monitor));
}

void inizioLettura(LettScrittMeteo* m){
	enter_monitor(&(m->monitor));

	//Se ci sono scrittori il lettore si deve bloccare
	if(m->occupato == 1 && m->num_lettori == 0)
		wait_condition(&(m->monitor), OK_LETTURA);

	(m->num_lettori)++;

	//Se è il primo lettore blocca gli scrittori
	if(m->num_lettori == 1)				//NON CAPISCO XKÈ IL PROF NON METTE L'IF
		m->occupato = 1;

	signal_condition(&(m->monitor), OK_LETTURA); //Vedi esercitazione monitor
}

//Legge le informazioni meteo
void lettore(LettScrittMeteo* m){

	inizioLettura(m);

	printf("CONDIZIONI METEO ATTUALI:\n");
	printf("\tTemperatura: %d C°\n", m->w.temperatura);
	printf("\tUmidità: %d %\n", m->w.umidita);
	printf("\tPioggia: ");
	if(m->w.pioggia == PIOVOSO)
		printf("SI\n\n");
	else if(m->w.pioggia == NONPIOVOSO)
		printf("NO\n\n");
	else
		printf("errore\n\n");

	fineLettura(m);
}

void fineLettura(LettScrittMeteo* m){
	enter_monitor(&(m->monitor));

	(m->num_lettori)--;

	//Se è l'ultimo lettore sblocca gli scrittori
	if(m->num_lettori == 0){
		m->occupato = 0; //Se non ci sono scrittori devo pur sbloccare il monitor
		signal_condition(&(m->monitor), OK_SCRITTURA);
	}else{
		//Se ci sono altri lettori non c'è bisogno di sbloccare il monitor
		leave_monitor(&(m->monitor));
	}
}

void inizioScrittura(LettScrittMeteo* m){
	enter_monitor(&(m->monitor));

	//Se ci sono scrittori o lettori si deve bloccare
	if(m->occupato == 1)
		wait_condition(&(m->monitor), OK_SCRITTURA);
	m->occupato = 1;

	leave_monitor(&(m->monitor));
}

void scrittore(LettScrittMeteo* m){

	inizioScrittura(m);

	srand(time(NULL) + getpid());

	m->w.temperatura = rand() % 51;
	m->w.umidita = rand() % 101;
	m->w.pioggia = rand() % 2;

	printf("Aggiornamento condizioni meteo\n\n");

	fineScrittura(m);
}

void fineScrittura(LettScrittMeteo* m){
	enter_monitor(&(m->monitor));

	if(queue_condition(&(m->monitor), OK_SCRITTURA) > 0)
		signal_condition(&(m->monitor), OK_SCRITTURA);
	else if(queue_condition(&(m->monitor), OK_LETTURA) > 0)
		signal_condition(&(m->monitor), OK_LETTURA);
	else{
		m->occupato = 0;
		leave_monitor(&(m->monitor));
	}
}
