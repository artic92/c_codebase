#ifndef __HEADER_H__
#define __HEADER_H__

//Informazioni necessarie per la ftok
#define PATH "./"
#define CHAR 'a'
#define CHAR2 'b'

#define CLIENT 1
#define SERVER 2
#define TERMINATE 3

//Definizione struttura del messaggio
typedef struct
{
	long type;
	pid_t pid;
} Messaggio;

#endif /* __HEADER_H__ */

