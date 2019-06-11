#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static char str[] = "signal SIGCHLD\n";
static int action_done = 0;

void action(int sig) {
    action_done = 1;
}

int main() {
    pid_t pid;
    int i = 0, j = 0, n = 0, status;
    struct sigaction act;
    act.sa_handler = action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    //act.sa_flags = SA_RESETHAND;
    sigaction(SIGCHLD, &act, NULL);


    switch(fork()){
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            printf("child process %d\n", getpid());
            for(; i < 100; i++){
                printf("outer loop: i =  %d\n", i);
                for(; j < 1000; j++)
                    for(; n < 10000; n++)
                        ;
            }
	    //pause();
            exit(9);
        default:
            printf("PARENT process %d\n", getpid());
            pid = wait(&status);
//pid = waitpid(-1, &status, WUNTRACED);
            //sleep(5);
            if(action_done == 1){
                write(1, str, sizeof(str));
                printf("Child process %d finished with status %d   %x\n", pid, status, status);
            }
            exit(0);
    }
}
