#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
	int fd, f, rb=0, i = 0, cwf = -1, prf = -1;
	char str[4] = "text"; 
	char s;
	struct flock wlock,rlock;	

fd = creat("t11.txt", 0777);
close(fd);
f = fork();
fd = open("t11.txt", O_RDWR);

	switch(f) {
		case -1:
			perror("Fork failed");
			exit(1);
		case 0:
			while(i < 4){	
				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				wlock.l_start = 0;
				wlock.l_len = 0;

				while(cwf == -1) { cwf = fcntl(fd, F_SETLK, &wlock); }	
				s = str[i];
				i++;
			      	write(fd, &s, 1);
				write(1, "\nchild wrote 1 byte\n", sizeof("\nchild wrote 1 byte\n")); 
				wlock.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &wlock);
				sleep(1);
			}
			exit(0);
		default:

			while(i < 4){	
				rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				rlock.l_start = 0;
				rlock.l_len = 0;

				while(prf == -1) { prf = fcntl(fd, F_SETLK, &rlock); }

				if((rb = read(fd, &s, 1)) > 0){ 
					write(1, "parent read 1 byte\n", sizeof("parent read 1 byte\n"));
					write(1, &s, rb);
					i++;
				}
				rlock.l_type = F_UNLCK;
				fcntl(fd, F_SETLK, &rlock);
				sleep(1);
			}	
			wait(NULL);
			//unlink("t11.txt");
			exit(0);	
	}		
}
