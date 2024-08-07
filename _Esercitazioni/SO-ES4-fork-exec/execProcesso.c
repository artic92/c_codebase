#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main() {
	int pid, st;
	
	pid=fork(); 
		
	if (pid==-1){
		fprintf(stderr, "generazione del processo fallita");
		return -1;
	}	
	if  (pid==0){
		execl("/bin/ls","ls","-lXh",NULL);
		fprintf(stderr,"exec fallita!!\n");
		_exit(1);
	}
	else if (pid>0){
		wait(&st);
		printf("il figlio %d ha terminato l'esecuzione ",pid);
		if ((char)st==0)
			printf("con stato: %d\n\n",st>>8);
		else
			printf("involontariamente!\n\n");
	}		 
return 0;
}
	 	

