#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void action(int sig) {
   	int c,p; 
	while((p = waitpid(-1, &c, 0)) !=-1){
	sleep(1);

	if(p == -1) 
		perror("Error ");

	if ( WIFEXITED(c) ) // returns true if the child terminated normally
        { 
        	int exit_status = WEXITSTATUS(c);         
        	printf("Exit status of the child %d was %d\n", p, exit_status); 
        } 
}
}

int main() {

struct sigaction act;
    	act.sa_handler = action;
    	sigemptyset(&act.sa_mask);
    	act.sa_flags = SA_NODEFER;
    	sigaction(SIGCHLD, &act, 0);

  	int i, p1, c1;
   	for (i=0;i<10;i++){
		if (fork()==0){
			printf("child %d\n", getpid()); 
			sleep(5); 
			exit(1);
		}
   	}

  	while(1) pause();

    //signal(SIGCHLD, SIG_IGN);
    //signal(SIGCHLD, action);

    /*switch(fork()){
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            printf("child process %d\n", getpid());
            system("ps -l");
            exit(0);
        default:
            printf("PARENT process %d\n", getpid());
            sleep(1);
            printf("Child process finished\n");
            system("ps -l");
            exit(0);
    }*/
}
