#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int spgrp;

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            //sleep(3);
            spgrp = setpgrp();
            printf("child\t%d\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()), spgrp);
            pause();
            printf("child process finished\n");
            exit(0);
        default:
            printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            pause();
            printf("PARENT process finished\n");
            exit(0);
    }
}
