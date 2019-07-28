#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>

void inc(int *shmp){
    (*shmp)++;
}

int main (int argc, char *argv[])
{
    int* intp;
    int shmid, i, pid;
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0640);
    if(shmid == -1){
        printf("Errore creazione shm!");
        return 0;
    }
    else{
        intp = (int*)shmat(shmid, NULL, 0);
        if(intp == (void*)-1){
            printf("Errore attacco shm!");
            return 0;
        } else {
            // Initialize the shared memory to zero
            *intp = 0;

            for(i = 0; i < 2; i++){
                pid = fork();
                if (pid == 0){
                    printf("Figlio n° %d\n", getpid()); // Print the child process ID
                    inc(intp);
                    _exit(0);
                }
            }
            for(i = 0; i < 2; i++)
                wait(NULL);

            printf("Valore della shm: %d\n", *intp);
            shmctl(shmid, IPC_RMID, 0);
        }
    }
    return 0;
}
