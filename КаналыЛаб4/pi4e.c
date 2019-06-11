#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    int b = 0;
    char buf[512]; // = "\0";

    b = read(0, buf, sizeof(buf));
    b = write(1, buf, b);

    return 0;
}
