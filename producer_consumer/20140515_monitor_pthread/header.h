#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

#define NUMCONSEGNE 5

//Definizione degli stati
#define VUOTO 0
#define PRONTO 1
#define IN_CONSEGNA 2

struct consegna
{
	int pizza;			//MARGHERITA = 1 oppure CALZONE = 2
	int stato;			
	int cliente;			//Numero incrementale
};

struct Monitor
{
	struct consegna c[NUMCONSEGNE];
	
	//Variabili di stato per la sincronizzazione
	int ok_produzione;
	int ok_consegna;
	
	//Strumenti di sincronizzazione per il monitor
	pthread_mutex_t mutex;
	pthread_cond_t ok_consegna_cond;
	pthread_cond_t ok_produzione_cond;
};

//Metodi per allocare e deallocare la struttura condivisa monitor
void inizializzaMonitor(struct Monitor*);
void distruggiMonitor(struct Monitor*);

//Ricerca una consegna nello stato VUOTO, la pone in PRONTO
void aggiungi_consegna(struct Monitor* m, int pizza, int cliente);
//Ricerca la prima consegna nello stato PRONTO, porlo nello stato IN_CONSEGNA e restituirne l'indice al chiamante
int inizia_consegna(struct Monitor* m);
//Pone lo stato della consegna indice_consegna in VUOTO
void fine_consegna(struct Monitor* m, int indice_consegna);

//Flussi di esecuzione per i threads fattorino e i threads pizzaiolo
void* fattorino(void*);
void* pizzaiolo(void*);

#endif /* __HEADER_H__ */

