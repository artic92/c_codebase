#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "semafori.h"
#include "header.h"

void prenotazione(int semid, Posto* vettore_posti, int* disponibilita){
	//---------------------------------Inizio prenotazione-------------------------
	//Aspetta un tempo casuale da 0 a 5 secondi
	srand(time(NULL)+getpid());
	int slp = rand() % 6;
	printf("Processo cliente n.ro %d: aspetto %d secondo/i\n", getpid(), slp);
	sleep(slp);
	
	//Effettua una qualsiasi prenotazione da 0 a 4 posti, scelta attraverso rand
	int num_prenotazioni = (rand() % 4) + 1;
	printf("Processo cliente n.ro %d: tento di effettuare %d prenotazione/i\n", getpid(), num_prenotazioni);
	
	int i, k, indice = 0, posizione = 0;
	int prenotazioni[num_prenotazioni];//vettore dei posti liberi
	if(num_prenotazioni > (*disponibilita)){
		printf("Disponibilità esaurita (ho tentato di allocare %d posto/i, ci sono %d posti liberi)\n", num_prenotazioni, (*disponibilita));
		_exit(1);
	}else{
		Wait_Sem(semid, SPAZIO_DISP);
		Wait_Sem(semid, MUTEX);
		for(i = 0; i < num_prenotazioni; i++){
			while(indice < NUMPOSTI && vettore_posti[indice].stato != LIBERO)
				indice++;
			vettore_posti[indice].stato = INAGGIORNAMENTO;
			prenotazioni[(posizione++)] = indice;//attenzione
			(*disponibilita)--;
			indice = 0;
		}
		Signal_Sem(semid, MUTEX);
	}
	
	//-------------------------------------Prenotazione------------------------------
	for(k = 0; k < num_prenotazioni; k++){
		int nposto = prenotazioni[k]; //indice della k-esima prenotazione
		vettore_posti[nposto].id_cliente = getpid();
		printf("[%d] Prenotazione effettuata al posto %d\n", getpid(), nposto);
		
	//----------------------------------Fine prenotazione----------------------------
		vettore_posti[nposto].stato = OCCUPATO;
	}	
}
