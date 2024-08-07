			/*----HEADER FILE----*/


/*************DEFINZIONE DELLE COSTANTI************/

#define VUOTO 2
#define PIENO 3
#define IN_USO 4
#define MUTEXP 0
#define MUTEXC 1
#define PROD 0
#define CONS 1
#define DIM 5

typedef long msg;



/***PROTOTIPI DELLE FUNZIONI UTILIZZATE***/


void Wait_Sem(int, int );
void Signal_Sem (int, int );

int RichiestaP(int*,int,int);
void Produzione(int , msg,msg*);
void RilascioP (int,int*,int);
int RichiestaC (int*,int , int);
msg Consumo(int,msg *);
void RilascioC(int,int*,int);

void Produttore(int*,msg*,int,int,int);
void Consumatore(int*,msg *,int,int);
