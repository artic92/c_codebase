			/*----------HEADER FILE---------*/


#define SYNCHS 0
#define SYNCHL 1
#define NUM_CONDITIONS 2

typedef long msg;

            
typedef struct {
 	  	 int numlettori;
		 int occupato;
 	  	 msg messaggio;
} Buffer;


void InizioLettura (Monitor*, Buffer*);
void FineLettura(Monitor*, Buffer*);
void InizioScrittura(Monitor*, Buffer*);
void FineScrittura(Monitor*, Buffer*);
void Lettore(Monitor*, Buffer*);
void Scrittore(Monitor*, Buffer*);
