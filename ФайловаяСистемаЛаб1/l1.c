#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int check_fd(int fd){
	if(fd == -1){
		perror("Fail");
		printf("File descriptor = %d\n", errno);
		printf("%s\n", strerror(errno));
		return 1;
	}
	
	return 0;
}

int main(int argc, char *argv[]){	

        int fd = open(argv[1], O_RDONLY);
	if(check_fd(fd) != 1)
		printf("Open successfully: %s. File descriptor = %d\n", argv[1], fd);

	return 0;
}
