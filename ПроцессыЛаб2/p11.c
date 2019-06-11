#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    int status;
    char *f = argv[1];
    argv++;

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            execvp(f, argv);//execvp (file, argv)
            exit(0);
        default:
            //wait(&status);
            system("ls -l");
            exit(0);
    }
}
