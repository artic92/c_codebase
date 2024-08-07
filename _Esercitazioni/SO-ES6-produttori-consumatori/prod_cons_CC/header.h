			/*----HEADER FILE----*/


#define SPAZIO_DISP  0
#define NUM_MESS  1
#define num_produzioni 8
#define num_consumi 8
#define DIM 5

typedef long msg;

void Wait_Sem(int, int );
void Signal_Sem (int, int );

void Produttore(int*,msg*,int);
void Consumatore(int*,msg *,int);
