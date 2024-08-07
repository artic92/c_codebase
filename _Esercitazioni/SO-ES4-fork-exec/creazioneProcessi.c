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
			
	 	printf("\nSono il processo figlio\n");
		printf("Mio PID: %d\n",getpid());
		printf("PID di mio padre: %d\n\n",getppid());
		//sleep(20);
		_exit(3);
	}
	else if (pid>0){
		printf("\nSono il processo padre\n");
		printf("Mio PID: %d\n",getpid());
		printf("PID di mio padre: %d\n\n",getppid());
		//sleep(10);
		//wait(NULL);
		wait(&st);
		printf("il figlio %d ha terminato l'esecuzione ",pid);
		if ((char)st==0)
			printf("con stato: %d\n\n",st>>8);
		else
			printf("involontariamente!\n\n");
		
	}
	return 0;
}
	 	

