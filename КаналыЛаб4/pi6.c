#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO "my_fifo_6"

int main() {
    	int fifo, rfd, n;
    	char buf[15] = "\0";

    	fifo = mkfifo(FIFO, 0777);
    	if (fifo < 0){
		printf("FIFO creation failed\n");
		exit(1);
	}
    	if ((rfd = open(FIFO, O_RDONLY)) < 0){
	//if ((rfd = open(FIFO, O_RDONLY |O_NDELAY)) < 0){
		printf("FIFO opening failed\n");
		exit(1);
	}
    	while ((n  = read(rfd, buf, sizeof(buf))) > 0)
		write(1, buf, n);
	 
    	close(rfd);
    	exit(0);
}
