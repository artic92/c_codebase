#ifndef __HEADER_H__
#define __HEADER_H__

//Definizione parametri ftok
#define PATH "."
#define CHARSEM 'a'
#define CHARSHM1 'b'
#define CHARSHM2 'c'

//Definizione alias nome semafori
#define SPAZIO_DISP 0
#define MEX_DISP 1

//Definizione stati del buffer
#define OCCUPATO 0
#define IN_USO 1
#define LIBERO 2

//Definizione struttura di sincronizzazione
typedef struct 
{
	int valore;
	int stato;
} buffer;

#endif /* __HEADER_H__ */

