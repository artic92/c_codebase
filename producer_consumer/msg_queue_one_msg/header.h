#ifndef HEADER_H_
#define HEADER_H_

#include <sys/types.h>

#define RTS 1
#define CTS 2
#define MSG 3

#define num_produzioni 2
#define num_consumi 2

typedef struct msgbuf{
	long mtype;
	int mex;
}Messaggio;

void sendSincr(int rts, int cts, int msg, Messaggio* mex);
void receiveBloc(int rts, int cts, int msg, Messaggio* risp);

#endif
