#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/signal.h>

int main() {
    pid_t pid;
    int status;
    signal(SIGUSR1, SIG_DFL);

    switch (pid = fork()) {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            printf("child process %d\n", getpid());
            pause();
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
