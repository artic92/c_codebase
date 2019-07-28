#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define DIM argc

int main (int argc, char *argv[])
{
	char* parametri[DIM];
	char path[15];
	int pid, st, i;
	pid = fork();
	if(pid == 0){
		strcpy(path, "/bin/");
		for(i = 0; i<DIM-1; i++)
			parametri[i]=argv[i+1];
		parametri[DIM-1]= NULL;
		strcat(path, argv[1]);
		execv(path, parametri);
		_exit(-1);
		}
	wait(&st);
	if(st == -1)
		printf("Errore exec!!");
	return 0;
}
