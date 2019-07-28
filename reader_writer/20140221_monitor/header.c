#include <stdio.h>
#include <unistd.h>
#include "header.h"
#include "monitor.h"

void inizializza(struct monitor_treno * m){
	m->stazione = 0;
	m->num_lettori = 0;
	m->occupato = 0;
	init_monitor(&(m->mon), NUMCOND);
}

void rimuovi(struct monitor_treno * m){
	remove_monitor(&(m->mon));
}

int leggi_stazione(struct monitor_treno * m){
	enter_monitor(&(m->mon));
	//Il lettore si deve bloccare se ci sono scrittori
	while(m->occupato == 1 && m->num_lettori == 0){
		printf("<%d> - Mi metto in attesa\n\n", getpid());
		wait_condition(&(m->mon), OK_LETTURA);
	}
	(m->num_lettori)++;
	//Se il primo lettore sblocca gli scrittori
	if(m->num_lettori == 1)
		m->occupato = 1;
	//Sblocca eventuali lettori ancora bloccati. Operazione fatta perchè lo scrittore sblocca solo il primo
	//lettore
	signal_condition(&(m->mon), OK_LETTURA);
	leave_monitor(&(m->mon));

	//Lettura
	int stazione = m->stazione;

	enter_monitor(&(m->mon));
	(m->num_lettori)--;
	//Se è l'ultimo lettore sblocca gli scrittori
	if(m->num_lettori == 0){
		m->occupato = 0;
		signal_condition(&(m->mon), OK_SCRITTURA);
	}
	leave_monitor(&(m->mon));
	return stazione;
}

void scrivi_stazione(struct monitor_treno * m, int stazione){
	enter_monitor(&(m->mon));
	//Lo scrittore si deve bloccare se ci sono lettori o altri scrittori
	while(m->occupato == 1)
		wait_condition(&(m->mon), OK_SCRITTURA);
	m->occupato = 1;
	leave_monitor(&(m->mon));

	m->stazione = stazione;

	enter_monitor(&(m->mon));
	if(queue_condition(&(m->mon), OK_SCRITTURA) > 0)
		signal_condition(&(m->mon), OK_SCRITTURA);
	else if(queue_condition(&(m->mon), OK_LETTURA) > 0)
		signal_condition(&(m->mon), OK_LETTURA);
	else
		m->occupato = 0;
	leave_monitor(&(m->mon));
}
