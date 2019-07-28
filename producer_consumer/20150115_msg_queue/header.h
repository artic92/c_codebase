#ifndef __HEADER_H__
#define __HEADER_H__

//Definizione parametri ftok
#define PATH "."
#define SHM 'a'
#define QUEUE 'b'
#define SEM 'c'

#define NUMCLIENT 3
#define NUMSERVER 3

//Definizione semaforo binario
#define MUTEX 0

//Definizione struttura messaggio
typedef struct
{
	long processo;
	int valore1;
	int valore2;
} Messaggio;
	

#endif /* __HEADER_H__ */

