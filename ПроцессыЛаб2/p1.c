#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

extern char **environ;

int main(int argc, char *argv[], char *envp[]) {
    int i = 0;
    char *var = "Hello, World";

    if (setenv("MYVAR", var, 0) != 0) {
        printf("setenv failed");
        exit(1);
    }
	printf("%s\n", getenv("MYVAR"));
unsetenv("MYVAR");


    while(*environ != NULL){
        printf("%s\n", *environ);
	environ++;
	}

    return 0;
}
