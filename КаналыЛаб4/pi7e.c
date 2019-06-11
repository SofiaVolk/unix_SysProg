#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fifo, wfd, n;
    char buf[15] = "\0";


    if ((wfd = open(argv[1], O_WRONLY)) < 0){
        printf("FIFO opening failed\n");
        exit(1);
    }
    while((n  = read(0, buf, sizeof(buf))) > 0)
    	write(wfd, buf, n);

    close(wfd);
    //unlink(argv[1]);
    exit(0);
}
