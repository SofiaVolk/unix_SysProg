#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid, cur_pid, cur1_pid, cur2_pid;
    int n = 0;
    char *s;

    pid = fork();
    printf("current pid %d\n", pid);

    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            cur_pid = getpid();
            printf("child with pid %d\n", cur_pid);
            exit(0);
        default:
            cur_pid = getpid();
            printf("PARENT with pid %d\n", cur_pid);
            exit(0);
    }
}
