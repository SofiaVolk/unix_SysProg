#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

void action(int sig);

int main() {
    int fp1[2], fp2[2], b = 0;
    char buf[512];
    const char str1[] = "test TEXT", str2[] = "hello, world";


    /*if (pipe(fp1) == 0) {
        if (pipe(fp2) == 0) {
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    //close(fp1[1]);//конец канала 1 для записи
                    //close(fp2[0]);//конец канала 2 для чтения
                    printf("\nchild process %d\n", getpid());

                    b = write(fp2[1], str2, sizeof(str2));
                    printf("Wrote %d bytes: %s\n", b, str2);

                    b = read(fp1[0], buf, sizeof(buf));
                    printf("Read %d bytes: %s\n", b, buf);



                    exit(0);
                default:
                    close(fp1[0]);//конец канала 1 для чтения
                    close(fp2[1]);//конец канала 2 для записи
                    printf("PARENT process %d\n", getpid());

                    b = write(fp1[1], str1, sizeof(str1));
                    printf("Wrote %d bytes: %s\n", b, str1);

                    b = read(fp2[0], buf, sizeof(buf));
                    printf("Read %d bytes: %s\n", b, buf);

                    exit(0);
            }
        } else perror("Pipe2 failed");
    } else perror("Pipe1 failed");*/

	    int s, err;
	    signal(SIGPIPE, action);
	    if (pipe(fp1) == 0) {
        
            switch (fork()) {
                case -1:
                    perror("Fork failed");
                    exit(1);
                case 0:
                    close(fp1[0]);close(fp1[1]);
                    printf("\nchild process %d\n", getpid());
		    sleep(1);
		    //b = write(fp1[1], str1, sizeof(str1));
                    printf("child wrote %d bytes\n", b); 
		    perror("Error");
                    exit(0);
                default:
                    close(fp1[1]);
		    //close(fp1[0]);
                    printf("PARENT process %d\n", getpid());

                    //while((b = read(fp1[0], buf, sizeof(buf))) != 0)
		    b = read(fp1[0], buf, sizeof(buf)); perror("Error");
                    printf("PARENT read %d bytes\n", b);
		    int f = wait(&s);
		    //printf("pid %d status %d\n", f, s);
                    exit(0);
            }
    } else perror("Pipe1 failed");
}

void action(int sig){
	printf("signal %d\n", sig);
}











