#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static int rf, wf;

int main(int argc, char *argv[]) {
	int rb=0;
	rf=-1; wf=-1;
	char str1[15] = "	CHILD:		";
	char str2[15] = "	PARENT:		";
	struct flock wlock,rlock;	

	switch(fork()) {
		case -1:
			perror("Fork failed");
			exit(1);
		case 0:
			rf=0; wf=0;
			wlock.l_type = F_WRLCK;
			wlock.l_whence = SEEK_CUR;
			wlock.l_start = 0;
			wlock.l_len = 0;

			rlock.l_type = F_RDLCK;
			rlock.l_whence = SEEK_CUR;
			rlock.l_start = 0;
			rlock.l_len = 0;

			int crf =-1, cwf = -1;
			char bufc[1024] = "\0";

			while(1){
				rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				crf = fcntl(0,F_SETLKW, &rlock);	
				cwf = fcntl(1,F_SETLKW, &wlock);

				rb = read(0, bufc, sizeof(bufc));
			      	if(bufc[0] == '\\') {
					rlock.l_type = F_UNLCK;
					wlock.l_type = F_UNLCK;
					fcntl(0,F_SETLK, &rlock);
					fcntl(1,F_SETLK, &wlock);
					sleep(1);
					continue;
			      	}	 
			      	write(1, str1, sizeof(str1));
			      	write(1, bufc, rb);
			}
			exit(0);
		default:
			wlock.l_type = F_WRLCK;
			wlock.l_whence = SEEK_CUR;
			wlock.l_start = 0;
			wlock.l_len = 0;

			rlock.l_type = F_RDLCK;
			rlock.l_whence = SEEK_CUR;
			rlock.l_start = 0;
			rlock.l_len = 0;
			int prf=-1, pwf=-1;
			char bufp[1024] = "\0";

			while(1){
				rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				prf = fcntl(0,F_SETLKW, &rlock);	
				pwf = fcntl(1,F_SETLKW, &wlock);
	
				rb = read(0, bufp, sizeof(bufp));
				if(bufp[0] == '\\') {
					rlock.l_type = F_UNLCK;
					wlock.l_type = F_UNLCK;
					fcntl(0,F_SETLK, &rlock);
					fcntl(1,F_SETLK, &wlock);
					sleep(1);
					continue;
				} 
				write(1, str2, sizeof(str2));
				write(1, bufp, rb);
			}
				
			wait(NULL);
			exit(0);	
	}		
}
