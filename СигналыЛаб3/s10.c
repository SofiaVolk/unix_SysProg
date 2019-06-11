#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>

void action(int sig) {
        int status;
	printf("beg signal %d\n", sig);
	sleep(5);
	printf("end signal %d\n", sig);
}


void actionINT(int sig) {
        int status;
	printf("INT beg signal %d\n", sig);
	sleep(5);
	printf("INT end signal %d\n", sig);
}

int main(void) {
	static struct sigaction act, oact, actINT;
	pid_t pid, ch_pid;

		sigset_t mask;
		sigemptyset(&mask);
		sigaddset(&mask, SIGINT);

		actINT.sa_handler = actionINT;
		actINT.sa_mask = mask;
		actINT.sa_flags = SA_RESTART;
		sigaction(SIGINT, &actINT, &oact);

		act.sa_handler = action;
		act.sa_mask = mask;
		act.sa_flags = SA_RESTART;
		sigaction(SIGCHLD, &act, &oact);


	switch(pid = fork()) {
	  case -1:
	 	perror("Fork failed");
	   	exit(1);
	  case  0:
	   	printf("child process %d\n", getpid());
		sleep(1);
	   	exit(0);
	  default:
		printf("PARENT process %d\nignore Ctrl+C\n", getpid());
		
		sleep(5);
		int status;
		wait(&status);
		printf("%x\n", status);
		//sigaction(SIGCHLD, &oact, NULL);
		exit(0);
	 }
}
