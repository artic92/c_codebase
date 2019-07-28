#include <stdio.h>
#include <unistd.h>
#include "header.h"

//Implementazione con monitor di HOARE

//Ritorna l'indice del primo scaffale pronto per la fornitura
int inizioFornitura(Monitor* m, Cond* c, Scaffale* s){
	enter_monitor(m);
	int indice = 0;
	if(c->livello_scorte == NELEM)
		wait_condition(m, OK_FORNITURA);
	while(indice < NELEM && s[indice].stato != LIBERO)
		indice++;
	s[indice].stato = IN_USO;
	leave_monitor(m);
	return indice;
}

//Effettura la fornitura
void fornitura(Monitor* m, Cond* c, Scaffale* s){
	int indice = inizioFornitura(m, c, s);
	s[indice].id_fornitore = getpid();
	sleep(2);
	printf("FORNITURA effettuata dal fornitore %d; indice: %d\n", s[indice].id_fornitore, indice);
	s[indice].stato = OCCUPATO;
	fineFornitura(m, c);
}

//Segnala l'avvenutra fornitura
void fineFornitura(Monitor* m, Cond* c){
	enter_monitor(m);
	(c->livello_scorte)++;//signal(MEX_DISP)
	signal_condition(m, OK_ACQUISTO);
	leave_monitor(m);
}

//Ritorna l'inidice del primo scaffare pronto per l'acquisto'
int inizioAcquisto(Monitor* m, Cond* c, Scaffale* s){
	enter_monitor(m);
	int indice = 0;
	if(c->livello_scorte == 0)
		wait_condition(m, OK_ACQUISTO);
	while(indice < NELEM && s[indice].stato != OCCUPATO)
		indice++;
	s[indice].stato = IN_USO;
	leave_monitor(m);
	return indice;
}

//Effettura l'acquisto
void acquisto(Monitor* m, Cond* c, Scaffale* s){
	int indice = inizioAcquisto(m, c, s);
	printf("ACQUISTO effettuato, merce venduta dal fornitore %d; indice:%d\n", s[indice].id_fornitore,  indice);
	sleep(2);
	s[indice].id_fornitore = 0;
	s[indice].stato = LIBERO;
	fineFornitura(m, c);
}

//Segnala l'avvenuto acquisto
void fineAcquisto(Monitor* m, Cond* c){
	enter_monitor(m);
	(c->livello_scorte)--;//signal(SPAZIO_DISP)
	signal_condition(m, OK_FORNITURA);
	leave_monitor(m);
}
