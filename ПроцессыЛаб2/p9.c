#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int fd, fd_child, fd_parent, cn, pn, status;
    char cbuf[256], pbuf[256];

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            fd = open(argv[1], O_RDONLY);
            fd_child = creat("/home/sonya/CLionProjects/proc9/text_child.txt", 0777);
            cn = read(fd, cbuf, sizeof(cbuf));
            write(fd_child, cbuf, cn);
            close(fd_child);
            exit(0);
        default:
            printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            fd = open(argv[1], O_RDONLY);
            fd_parent = creat("/home/sonya/CLionProjects/proc9/text_parent.txt", 0777);
            pn = read(fd, pbuf, sizeof(pbuf));
            write(fd_parent, pbuf, pn);
            close(fd_parent);
            wait(&status);
    }
    close(fd);

    fd_child = open("/home/sonya/CLionProjects/proc9/text_child.txt", O_RDONLY);
    read(fd_child, pbuf, sizeof(pbuf));
    printf("\ntext_child.txt:\n %s", pbuf);

    fd_parent = open("/home/sonya/CLionProjects/proc9/text_parent.txt", O_RDONLY);
    read(fd_parent, pbuf, sizeof(pbuf));
    printf("\ntext_parent.txt:\n %s", pbuf);

    return 0;
}
