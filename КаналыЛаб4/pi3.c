#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fp1[2], fp2[2], b = 0, n =0;
    char buf[512] = "\0";

    if (pipe(fp1) == 0) {
        if (pipe(fp2) == 0) {
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    close(fp2[0]);
                    close(fp1[1]);
                    printf("child process %d\n", getpid());

                    b = read(0, buf, sizeof(buf));
                    n = write(fp2[1], buf, b);
                    printf("child wrote %d bytes \n", n);
                    memset(buf, 0, sizeof(buf));
                    sleep(1);

                    /*while((b = read(fp1[0], buf, 1)) == 0){
                        printf("child read %d byte: %s\n", b, buf);
                    }*/

                    for(; n > 0; n--){
                        b = read(fp1[0], buf, 1);
                        printf("child read %d byte: %s\n", b, buf);
                    }

                    close(fp1[0]);
                    close(fp2[1]);
                    exit(0);
                default:
                    close(fp2[1]);
                    close(fp1[0]);
                    printf("PARENT process %d\n", getpid());

                    //while((b = read(fp2[0], buf, 1)) != 0){
                    while(read(fp2[0], buf, 1)){
                        printf("PARENT read %d byte: %s\n", 1, buf);
                        write(fp1[1], buf, 1);
                    }
                    wait(NULL);
                    close(fp1[1]);
                    close(fp2[0]);
                    exit(0);
            }
        } else perror("Pipe 2 failed");
    } else perror("Pipe 1 failed");
}
