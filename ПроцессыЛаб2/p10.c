#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    int i = 0;
    char *env[] = {"LANGUAGE=en_US", NULL};

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            execle("/home/sonya/CLionProjects/proc10_1/cmake-build-debug/proc10_1", "l1", "l2", "l3", NULL, env);//int execle (path, arg0, arg1, ..., argn, (char*) 0, envp)
            exit(0);
        default:
            printf("number of arguments: %d\n",argc);
            printf("list of arguments: ");
            while(argv[i] != NULL)
                printf("%s ", argv[i++]);

            i = 0;
            printf("\nenvironment:\n");
            while(envp[i] != NULL)
                printf("%s\n", envp[i++]);
    }

    return 0;
}
