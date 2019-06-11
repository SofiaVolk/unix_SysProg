#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/signal.h>

static char str[] = "signal SIGUSR1\n";

void action(int sig){
	write(1, str, sizeof(str));
	sleep(1);
}

int main() {
    pid_t pid;
    int i = 0, j = 0, k = 0, n = 0, m = 0, status;

    signal(SIGUSR1, action);

    switch (pid = fork()) {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            
            printf("child process %d\n", getpid());
            while(i++ < 1000000) {
                printf("i=%d\n",i);
                while(j++ < 10000000)
                    while(k++ < 10000000)
                        while(n++ < 10000000)
                            while(m++ < 10000000)
                                ;
            }
	    //pause();
            exit(0);
        default:
            printf("PARENT process %d\n", getpid());
            sleep(1);
            kill(pid, SIGUSR1);

            wait(&status);
            printf("Status: %d\n", status);
            exit(0);
    }
}
