#ifndef __HEADER_H__
#define __HEADER_H__

//Definizione dati utili per la ftok
#define PATH "./"
#define CHARB 'a' //Mailbox client-balancer
#define CHARS 'b' //Mailbox balancer-servers

//Definizione tipi di messaggio
#define CLIENT 1
#define SERVER1 2
#define SERVER2 3
#define SERVER3 4
#define TERMINATE 5 //NB NON METTERE MAI ZERO!!!!!!!!

//Definizione struttura messaggio
typedef struct
{
	long type;
	pid_t pid;
}Messaggio;

#endif /* __HEADER_H__ */

