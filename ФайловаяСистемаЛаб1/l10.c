#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int fd, n, i;
    char buf;

    if((fd = open(argv[1], O_RDONLY)) > 0){
        if((n = lseek(fd, -2, SEEK_END)) > 0){
            for(i = n; i >= 0; i--){
                lseek(fd, i, SEEK_SET);
                if ((read(fd, &buf, 1)) > 0)
                    printf("%c", buf);
                else perror("Read failed");
            }
        }
        else perror("File is empty");
    }
    else perror("Open failed");

    close(fd);
    return 0;
}
