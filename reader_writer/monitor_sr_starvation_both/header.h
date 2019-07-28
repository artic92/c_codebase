#ifndef __HEADER_H__
#define __HEADER_H__

#include "monitor.h"

#define NONPIOVOSO 0
#define PIOVOSO 1

typedef struct {
	int temperatura; //Valore compreso tra -50 e +50
	unsigned int umidita; //Valore compreso tra 0 e 100
	unsigned short pioggia; 
} meteo;

//Definizione variabili conditions e struttura di sincronizzazione
#define NUMCOND 2
#define OK_LETTURA 0
#define OK_SCRITTURA 1

typedef struct
{
	Monitor monitor;
	meteo w;
	int occupato;
	int num_lettori;
} LettScrittMeteo;

//Inizializzazione della struttra e relativa deallocazione
void inizializza(LettScrittMeteo*);
void distruggi(LettScrittMeteo*);

//Incrementa il numero di lettori, se è il primo lettore blocca gli scrittori
void inizioLettura(LettScrittMeteo*);
//Legge le informazioni meteo
void lettore(LettScrittMeteo*);
//Decrementa il numero di lettori, se è l'ultimo lettore sblocca gli scrittori
void fineLettura(LettScrittMeteo*);

//Incrementa il numero di scrittori, se è il primo scrittori blocca gli scrittori e i lettori
void inizioScrittura(LettScrittMeteo*);
//Scrittura delle condizioni meteo
void scrittore(LettScrittMeteo*);
//Decrementa il numero di scrittori, se è l'ultimo scrittore sblocca gli scrittori o i lettori oppure esce
void fineScrittura(LettScrittMeteo*);

#endif /* __HEADER_H__ */

