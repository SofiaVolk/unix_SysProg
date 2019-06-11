#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fp1[2], fp2[2], b = 0;
    char buf[512] = "\0";

    if (pipe(fp1) == 0) {
        if (pipe(fp2) == 0) {
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    close(fp1[1]);
                    close(fp2[0]);
                    printf("child process %d\n", getpid());
                    dup2(fp2[1], 1);
                    execl("4e", "4e", NULL);
                    exit(EXIT_FAILURE);
                default:
                    close(fp1[0]);
                    //close(fp2[1]);
                    printf("PARENT process %d\n", getpid());
		    dup2(1, fp2[1]);
                    while ((b = read(fp2[0], buf, 1)) != 0) {
                        printf("PARENT read %d byte: %s\n", b, buf);
                    }
                    wait(NULL);
		    close(fp1[1]);
                    close(fp2[0]);
                    exit(0);
            }
        } else perror("Pipe 2 failed");
    }else perror("Pipe 1 failed");
}

