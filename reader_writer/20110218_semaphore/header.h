#ifndef __HEADER_H__
#define __HEADER_H__

#define NUMCLIENTI 50
#define	NUMPOSTI 80

#define PATH "./"
#define CHARS 'a'
#define CHARSH 'b'

//Non ci sono consumatori, quindi solo due semafori
#define MUTEX 0
#define SPAZIO_DISP 1

#define LIBERO 0
#define OCCUPATO 1
#define INAGGIORNAMENTO 2

typedef struct 
{
	unsigned int id_cliente;
	unsigned int stato;
} Posto;

void inizioPrenotazione(int, Posto*, int*);
void prenotazione(int, Posto*, int*);
void finePrenotazione(int, Posto*, int*);

#endif /* __HEADER_H__ */

