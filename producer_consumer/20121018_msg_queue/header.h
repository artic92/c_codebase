#ifndef __HEADER_H__
#define __HEADER_H__

//Dati per la ftok
#define PATH "."
#define CHAR2A 'C'
#define CHAR2R 'D'
#define	CHAR3A 'E'
#define	CHAR3R 'F'
#define CHAR4A 'G'
#define CHAR4R 'H'
#define CHAR5A 'I'
#define CHAR5R 'L'
#define CHAR6A 'A'
#define CHAR6R 'B'

#define NUMVOLTE 3

typedef struct
{
	long type;
	int op1;
	int op2;
} Messaggio;

typedef struct
{
	long type;
	int op1;
	int op2;
	int op3;
	int op4;
} Messaggio4;

typedef struct
{
	long type;
	int risultato;
} MessaggioRisposta;

#endif /* __HEADER_H__ */

