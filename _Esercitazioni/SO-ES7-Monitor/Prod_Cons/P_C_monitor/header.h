			/*-----HEADER FILE-----*/


enum VAR_CONDITIONS {OK_PRODUZIONE, OK_CONSUMO};

typedef long msg;

typedef struct {
	int ok_produzione;
	int ok_consumo;
} cond;
#define NUM_CONDITIONS	2
void Consumatore(Monitor*,cond*,msg*);
void Produttore(Monitor*,cond*,msg*);

void InizioConsumo(Monitor*,cond*);
void InizioProduzione(Monitor*,cond*);
void FineConsumo(Monitor*,cond*);
void FineProduzione(Monitor*,cond*);

