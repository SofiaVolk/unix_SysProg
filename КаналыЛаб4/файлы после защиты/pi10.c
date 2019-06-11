#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define __ERROR { perror("Opening or creating failed"); printf("File: %s; line: %d\n",__FILE__,__LINE__-1);\
		  exit(1); }

int main(int argc, char *argv[]) {
	int rb=0;
	char buf[15];
	char str1[15] = "	CHILD\n";
	char str2[15] = "	PARENT\n";	

	switch(fork()) {
		case -1:
			perror("Fork failed");
			exit(1);
		case 0:
			while(1) {
			      rb = read(0, buf, sizeof(buf));//sleep(1);
			      write(1, str1, sizeof(str1));
			      write(1, buf, rb);
			}
			exit(0);
		default:	
			while(1) {
				rb = read(0, buf, sizeof(buf));
				write(1, str2, sizeof(str2));
	      			write(1, buf, rb);
			}	
			wait(NULL);
			exit(0);	
	}		
}
