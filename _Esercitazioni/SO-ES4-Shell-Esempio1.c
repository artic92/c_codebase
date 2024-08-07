#include<stdio.h>
#include<unistd.h>
#include<sys/types.h> 
#include <sys/wait.h>

int main(){

    int pid,status;

    char programma[100];
    char parametro[100];

    printf("inserisci il percorso dell'eseguibile \n");
    scanf("%s",programma);
    printf("il percorso da te inserito è %s\n",programma);

    printf("inserisci i parametri \n");
    scanf("%s",parametro);
    printf("il parametro da te inserito è %s\n",parametro);

    pid=fork();

    if(pid==0){
        execl(programma,programma,parametro,NULL);
        printf("exec fallita\n");
	}
	else if(pid>0){
        wait(&status);
	}
	else printf("errore");

    return 0;
}