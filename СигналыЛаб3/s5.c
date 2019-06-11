#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/signal.h>

int main() {
    pid_t pid;
    int i = 0, j = 0, k = 0, n = 0, m = 0, status;

    switch (pid = fork()) {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            signal(SIGUSR1, SIG_DFL);
            printf("child process %d\n", getpid());
            while(i++ < 1000000) {
                printf("i=%d\n",i);
                while(j++ < 10000000) 
                    while(k++ < 10000000) 
                        while(n++ < 10000000) 
                            while(m++ < 10000000)
				;
            }
	    //pause();
            exit(0);
        default:
            printf("PARENT process %d\n", getpid());
            sleep(1);
            kill(pid, SIGKILL);
            //pid = waitpid(-1, &status, WUNTRACED);
wait(&status);
            printf("Status: %d   %x\n", status,status);
            exit(0);
    }
}
