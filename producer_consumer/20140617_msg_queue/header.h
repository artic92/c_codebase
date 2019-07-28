#ifndef __HEADER_H__
#define __HEADER_H__

//Definizioni per la ftok
#define PATH "."
#define CHARC 'a'
#define CHARR 'b'

typedef	struct
{
	long processo;
	char messaggio[50];
} Messaggio_richiesta;

typedef struct 
{
	long processo;
	int val_ritorno;
} Messaggio_risposta;


#endif /* __HEADER_H__ */

