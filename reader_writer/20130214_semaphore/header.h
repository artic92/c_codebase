#ifndef __HEADER_H__
#define __HEADER_H__

//Definizione elementi utili per la ftok
#define PATH "."
#define CHARS 'a'
#define CHARA 'b'

#define NUMSEM 2
#define SYNC 0
#define MUTEX 2

typedef struct
{
	char data_appello[20];
	int num_prenotati;
	
	int num_studenti;
} Appello;

#endif /* __HEADER_H__ */

