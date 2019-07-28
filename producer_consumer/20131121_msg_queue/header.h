#ifndef __HEADER_H__
#define __HEADER_H__

#define PATH "."

typedef struct 
{
	long processo;
	char messaggio[50];
} Messaggio;

void mittente(int, int);
void ricevente(int);

#endif /* __HEADER_H__ */

