#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define __ERROR { perror("Opening or creating failed"); printf("File: %s; line: %d\n",__FILE__,__LINE__-1); exit(1); }

int copy_file(int fd, int fd_copy){
    long int n = 0, wb = 0, rb = 0;
    char buf[1];

    while((n=read(fd,buf,sizeof(buf))) > 0){
	rb += n; 
	wb += write(fd_copy,buf,n);
    }
            
    if(wb != rb)
	return 1;
        
    return 0;
}

int main(int argc, char *argv[]) {
	int fd=0, fd_copy=1;
	
	if(argc > 1) { 
		fd = open(argv[1],O_RDONLY);
		if(fd < 0) __ERROR;

		fd_copy = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0600);
		if(fd_copy < 0) __ERROR;

		if(dup2(fd,0) == -1){
			perror("Duplicating is failed"); 
			exit(1);  
		}

		if(dup2(fd_copy,1) == -1){
			perror("Duplicating is failed"); 
			exit(1); 
		}
		
		close(fd); 
		close(fd_copy);
	} 

	if(copy_file(0, 1))
		printf("Copy failed\n");
		
 	return 0;
}
