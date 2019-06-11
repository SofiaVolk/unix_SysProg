#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int i, max = 0, ret;
    long int f_size[argc-1];
    struct stat file_stat;
    char *f_name;

    for (i=0; i < argc-1; i++){
        if((ret = stat(argv[i+1], &file_stat)) != 0)
            printf("stat failure error .%d", ret);
        else
            f_size[i] = file_stat.st_size;
    }

    for (i=0; i < argc-1; i++){
        if(f_size[i] > max){
            max = f_size[i];
            f_name = argv[i+1];
        }
    }
    printf("%s: %d", f_name, max);

    return 0;
}
