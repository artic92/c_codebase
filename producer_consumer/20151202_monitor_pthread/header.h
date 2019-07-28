#ifndef __HEADER_H__
#define __HEADER_H__

#include <pthread.h>

//Definizione stati di ciascun buffer
#define LIBERO 1
#define IN_USO 2
#define	OCCUPATO 3

#define NUMOPERANDI 4
#define NUMBUFFER 5

typedef struct 
{
	int operandi[NUMOPERANDI];
	int totale_operandi;
} buffer; 

typedef struct
{
	buffer elaborazioni[NUMBUFFER];
	//Vettore di stato
	int stato[NUMBUFFER];
	
	//Variabili di sincronizzazione
	int ok_produzione;
	int ok_consumo;
	
	//Strutture per il monitor
	pthread_mutex_t mutex;
	pthread_cond_t ok_produzione_cond;
	pthread_cond_t ok_consumo_cond;
} MonitorElaborazioni;

void inizializza(MonitorElaborazioni*);
void distruggi(MonitorElaborazioni*);

int inizioProduzione(MonitorElaborazioni*);
void* richiedente(void*);
void fineProduzione(MonitorElaborazioni*, int);

int inizioElaborazione(MonitorElaborazioni*);
void* elaborazione(void*);
void fineElaborazione(MonitorElaborazioni*, int);

#endif /* __HEADER_H__ */

