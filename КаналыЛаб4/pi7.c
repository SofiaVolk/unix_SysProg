#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fifo, rfd, n;
    char buf[15] = "\0";

    fifo = mkfifo(argv[1], 0777);
    if (fifo < 0){
        printf("FIFO creation failed\n");
        exit(1);
    }
    if ((rfd = open(argv[1], O_RDONLY)) < 0){
    //if ((rfd = open(argv[1], O_RDONLY |O_NDELAY)) < 0){
    //if ((rfd = open(argv[1], O_RDONLY |O_NONBLOCK)) < 0){
        printf("FIFO opening failed\n");
        exit(1);
    }
    while ((n  = read(rfd, buf, sizeof(buf))) > 0)
        write(1, buf, n);

    close(rfd);
    exit(0);
}
