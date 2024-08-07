			/*--------HEADER FILE-------*/


#define MSG_DISP 0
#define SPAZIO_DISP 1
#define DIM 1

typedef long msg;

void Wait_Sem(int, int );
void Signal_Sem (int, int );

void Produttore(msg*,int);
void Consumatore(msg *,int);
