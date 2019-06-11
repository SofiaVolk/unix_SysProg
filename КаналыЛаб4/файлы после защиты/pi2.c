#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int fp[2], b = 0;
    char buf[512] = "\0";

    if (pipe(fp) == 0) {
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    close(fp[0]);//конец канала  для чтения
                    printf("child process %d\n", getpid());

                    b = read(0, buf, sizeof(buf));
                    printf("child read %d bytes \n", b);
                    b = write(fp[1], buf, b);

                    close(fp[1]);
                    exit(0);
                default:
                    close(fp[1]);//конец канала для записи
                    printf("PARENT process %d\n", getpid());

                    while((b = read(fp[0], buf, 1)) != 0){
                        write(1, buf, b);
                        printf("\n");
                     }

                    close(fp[0]);
                    exit(0);
            }
    } else perror("Pipe failed");
}
