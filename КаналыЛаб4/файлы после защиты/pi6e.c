#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO "my_fifo_6"

int main() {
    	int wfd, n;
    	char buf[15] = "Hello, world\n";
char buf1[15] = "writer run\n";
char buf2[15] = "writer opened\n";
char buf3[15] = "writer wrote\n";

write(1, buf1, 15);
	if ((wfd = open(FIFO, O_WRONLY |O_NDELAY)) < 0){
	//if ((wfd = open(FIFO, O_WRONLY)) < 0){
		printf("FIFO opening failed\n"); perror("");
		exit(1);
	}  
write(1, buf2, 15);
    	if((n = write(wfd, buf, sizeof(buf))) < 0) perror("");
write(1, buf3, 15);
    	close(wfd);
    	//unlink(FIFO);
    	exit(0);
}
