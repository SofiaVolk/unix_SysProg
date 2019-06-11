#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <signal.h>

#define s1 "00\n"
#define s2 "11\n"


void handler1(int signo) {
	write(1, s1, sizeof(s1));
}

void handler2(int signo) {
	write(1, s2, sizeof(s2));
}

int main(void) {
	pid_t pid, p_pid;
	static struct sigaction action, ch_action;
	sigset_t mask, omask;
	int c = 0;

    	action.sa_handler = handler1;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
    	sigaction(SIGUSR1, &action, NULL);
    	ch_action.sa_handler = handler2;
	sigemptyset(&ch_action.sa_mask);
	ch_action.sa_flags = 0;
    	sigaction(SIGUSR2, &ch_action, NULL);

	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR2); 
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK,&mask,&omask);

	pid=fork();

	switch(pid) {
	  case -1:
	 	perror("Fork failed");
	   	exit(1);
	  case  0:
	    	p_pid = getppid();
	   
	   	while(c != 5){
            		c++;
            		kill(p_pid, SIGUSR1);
            		sigsuspend(&omask);
	    	}

	    	exit(0);
	  default:

	     	while(c != 5){
		 	c++;
		 	sigsuspend(&omask);
		 	kill(pid, SIGUSR2);
	     	}

	  	wait(NULL);
	  	exit(0);
	}
}

