#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fd, n, rbytes, wbytes;
	char buf[] = "TestText\n";
	char b[512];
	
	unsigned int access_mode = strtol(argv[2], NULL, 8);
	
	if((fd = creat(argv[1], access_mode)) != -1){
		printf("Create successfully: %s\n", argv[1]);

		if((n = write(fd, buf, sizeof(buf)-1)) != -1){
			printf("Let's open and read %s:\n", argv[1]);

			if((fd = open(argv[1], O_RDONLY)) != -1){

				if((rbytes = read(fd, b, sizeof b)) != 0){

					if((wbytes = write(1, b, rbytes)) != rbytes)
						printf("Write error: %s\n", strerror(errno));
				}
				else
					printf("Read failed: %s\n", strerror(errno));
					
				if(close(fd) != -1){
					printf("Let's open %s with r+w mode:\n", argv[1]);
					if((fd = open(argv[1], O_RDWR)) != -1)
						write(1, b, rbytes);
					else 
						printf("Open failed: (%d) %s.\n", errno, strerror(errno));
				}
				else
					printf("Close failed: %s\n", strerror(errno));	
			}
			else
				printf("Open failed: %s\n",strerror(errno));
		}
		else
			printf("%d Write failed: %s\n", n, strerror(errno));			
	}
	else
		perror("Creation failed");
	
	return 0;
}
