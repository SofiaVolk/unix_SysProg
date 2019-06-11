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

	if ((wfd = open(FIFO, O_WRONLY)) < 0){
		printf("FIFO opening failed\n");
		exit(1);
	}    
    	write(wfd, buf, sizeof(buf));
    
    	close(wfd);
    	unlink(FIFO);
    	exit(0);
}
