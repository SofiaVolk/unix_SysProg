#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
        char buf[] = "0Lorem_0ipsum_2Lorem_2ipsum_3Lorem_3ipsum";
        char b[512];
        int fd, n, n1, rbytes, rb;

	if((fd = creat(argv[1], 0600)) != -1){
            if((n = write(fd, buf, sizeof(buf)-1)) != -1){
                n1 = n / 2;

                if((fd = open(argv[1], O_RDWR)) != -1)
                    if((rbytes = read(fd, b, n) != 0)){
                        write(1, b, n);
			memset(b, 0, sizeof(b));

                        rbytes = pread(fd, b, n1+1, n1); //(,,count,offset)
			printf("\nPointer in the middle: %s", b);
			memset(b, '\0', sizeof(b));
                        
			lseek(fd, 0, SEEK_END);
			rbytes = read(fd, b, n1);
			printf("\nSEEK_END+0: %s", b);

			int pos = lseek(fd, -10, SEEK_SET);
			rbytes = read(fd, b, n1);
			printf("\nSEEK_SET-10: %s", b);
			perror("error ");
                    }
                    else
                        perror("Read failed");
                else
                    perror("Open failed");
            }
            else
                perror("Write failed");
	}
		
	return 0;
}
