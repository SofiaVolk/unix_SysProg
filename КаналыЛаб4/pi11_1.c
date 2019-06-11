#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

void act(int sig){signal(SIGUSR1, SIG_IGN);}

int main(int argc, char *argv[]) {
	int f, rb=0, crf=-1, cwf=-1, prf=-1, pwf=-1, sig;
	char str1[15] = "	CHILD:		";
	char str2[15] = "	PARENT:		";
	char buf[1024] = "\0";
	struct flock wlock,rlock;

sigset_t set; 
sigemptyset(&set); 
sigaddset(&set, SIGUSR1);
signal(SIGUSR1, act);
sigprocmask(SIG_BLOCK, &set, NULL);
	
				rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				rlock.l_start = 0;
				rlock.l_len = 0;

				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				wlock.l_start = 0;
				wlock.l_len = 0;

				//while(fcntl(0,F_SETLKW, &rlock) == -1 || fcntl(1,F_SETLKW, &wlock) == -1){	
					prf = fcntl(0,F_SETLKW, &rlock);				
					pwf = fcntl(1,F_SETLKW, &wlock);//}


f = fork();
	switch(f) {
		case -1:
			perror("Fork failed");
			exit(1);
		case 0:

			while(1){
				rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				rlock.l_start = 0;
				rlock.l_len = 0;
				
				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				wlock.l_start = 0;
				wlock.l_len = 0;

				//while(fcntl(0,F_SETLKW, &rlock) == -1 || fcntl(1,F_SETLKW, &wlock) == -1){
				while(crf == -1 || cwf == -1){	
					cwf = fcntl(1,F_SETLKW, &wlock);
					crf = fcntl(0,F_SETLKW, &rlock);}

				//cwf = fcntl(1,F_SETLKW, &wlock);
				//crf = fcntl(0,F_SETLKW, &rlock);	
				
				rb = read(0, buf, sizeof(buf));
			      	if(buf[0] == '\\') {
					rlock.l_type = F_UNLCK;
					wlock.l_type = F_UNLCK;
					fcntl(0,F_SETLK, &rlock);
					fcntl(1,F_SETLK, &wlock);//kill(getppid(), SIGUSR1);
					sleep(1);
					continue;
			      	}	 
			      	write(1, str1, sizeof(str1));
			      	write(1, buf, rb);
			}
			exit(0);
		default: 
			//sigwait(&set, &sig);
			while(1){
				/*rlock.l_type = F_RDLCK;
				rlock.l_whence = SEEK_CUR;
				rlock.l_start = 0;
				rlock.l_len = 0;

				wlock.l_type = F_WRLCK;
				wlock.l_whence = SEEK_CUR;
				wlock.l_start = 0;
				wlock.l_len = 0;

				while(prf == -1 || pwf == -1){	
					prf = fcntl(0,F_SETLKW, &rlock);				
					pwf = fcntl(1,F_SETLKW, &wlock);}*/
	
				rb = read(0, buf, sizeof(buf));
				if(buf[0] == '\\') {
					rlock.l_type = F_UNLCK;
					wlock.l_type = F_UNLCK;
					fcntl(0,F_SETLK, &rlock);
					fcntl(1,F_SETLK, &wlock);
					sleep(1);
					continue;
				} 
				write(1, str2, sizeof(str2));
				write(1, buf, rb);
			}
				
			wait(NULL);
			exit(0);	
	}		
}
