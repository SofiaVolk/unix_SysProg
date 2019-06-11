#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/signal.h>

void action(int sig){
    printf("signal %d\n", sig);
    sleep(1);
}

int main() {
    pid_t pid;
    int i = 0, j = 0, k = 0, n = 0, m = 0, status;

    switch (pid = fork()) {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            signal(SIGALRM, action);
            alarm(2);
alarm(6);
            printf("child process %d\n", getpid());
            while(i++ < 1000000) {
                printf("i=%d\n",i);
                while(j++ < 1000000)
                    while(k++ < 1000000)
                        while(n++ < 1000000)
                            while(m++ < 1000000)
                                sleep(1);
            }
            exit(0);
        default:
            printf("PARENT process %d\n", getpid());
            wait(&status);
            printf("Status: %d\n", status);
            exit(0);
    }
}
