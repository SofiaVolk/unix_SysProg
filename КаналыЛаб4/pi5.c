#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fp[2], b = 0;
    char buf[512] = "\0";

    if (pipe(fp) == 0) {
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    close(fp[0]);
                    printf("child process %d\n", getpid());
                    dup2(fp[1], 1);
                    execlp("who", "who", NULL);
                    exit(EXIT_FAILURE);
                default:
                    close(fp[1]);
                    printf("PARENT process %d\n", getpid());
                    dup2(1, fp[1]);

                    dup2(fp[0], 0);
                    execlp("wc", "wc", "-l", NULL);
                    exit(EXIT_FAILURE);
            }
    } else perror("Pipe failed");
}
