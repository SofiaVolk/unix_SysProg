#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int pid, wfd, rfd, rb, wb, i = 0;
    char str[15] = "testTEXT ";
    struct flock lock;

    switch(pid=fork()) {
		case -1:
			perror("Fork failed");
			exit(1);
		case 0:
			if ((wfd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0){
        			printf("File opening failed wfd\n");
        			exit(1);
    			}

			while(i++ < 100){
				if((wb = write(wfd, str, sizeof(str))) < 0) {
					perror("Write error");
					kill(getppid(), SIGKILL);
					exit(1);
				}
			}
    				
    			close(wfd);
    			exit(0);	
		default:

    			if ((rfd = open(argv[1], O_RDONLY)) < 0){
        			printf("File opening failed rfd\n");
        			exit(1);
    			}
			char buf1[15];

    			while ((rb  = read(rfd, buf1, sizeof(buf1))) > 0){
			//sleep(1);
				if(rb == -1) { 
					perror("Read error"); 
					kill(pid,SIGKILL); 
					exit(1); 
				}
				write(1, buf1, rb);
			}

    			close(rfd);
    			exit(0);	

	}	
    
}
