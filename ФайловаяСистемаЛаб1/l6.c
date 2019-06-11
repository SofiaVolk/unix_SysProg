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
        int fd, n, n1, rbytes, f_size, ret;
struct stat file_stat;

	if((fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0600)) != -1){
            if((n = write(fd, buf, sizeof(buf)-1)) != -1){
                n1 = n / 2;

                if((fd = open(argv[1], O_RDWR)) != -1)
                    if((rbytes = read(fd, b, n) > 0)){
                    	write(1, b, n);
			memset(b, 0, sizeof(b));

                        rbytes = pread(fd, b, n1+1, n1); //(,,count,offset)
			printf("\nPointer in the middle: %s", b);
			memset(b, '\0', sizeof(b));
                        
			lseek(fd, 0, SEEK_END);
			rbytes = read(fd, b, n1);
			printf("\nSEEK_END+0: %s", b);

			lseek(fd, -10, SEEK_SET);
			if((rbytes = read(fd, b, n1)) < 0 )
				perror("error ");
			else 
				printf("\nSEEK_SET-10: %s", b);

			//sparse file
			memset(b, '\0', sizeof(b));			
			f_size = lseek(fd,0,SEEK_END);
			printf("\nFile size in bytes: %d", f_size);
			lseek(fd,8*1024,SEEK_SET);
			write(fd, buf, sizeof(buf)-1);
			
			if ((ret = stat(argv[1], &file_stat))!=0)
  				printf("stat failure error .%d", ret);
          		else{
			printf("\nSparse file size in bytes: %ld", file_stat.st_size);
			printf("\nSystem block size in bytes: %ld", file_stat.st_blksize);
			printf("\nSparse file size in blocks: %ld\n", file_stat.st_blocks);
			}
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
