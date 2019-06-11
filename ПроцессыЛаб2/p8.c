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
    char buf[256];

    fd = open(argv[1], O_RDONLY);

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            fd_child = creat("/home/sonya/CLionProjects/proc8/text_child.txt", 0777);
            cn = read(fd, buf, sizeof(buf));
            write(fd_child, buf, cn);
            close(fd_child);
            exit(0);
        default:
            printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            fd_parent = creat("/home/sonya/CLionProjects/proc8/text_parent.txt", 0777);
            pn = read(fd, buf, sizeof(buf));
            write(fd_parent, buf, pn);
            close(fd_parent);
            wait(&status);
    }
    close(fd);

    fd_child = open("/home/sonya/CLionProjects/proc8/text_child.txt", O_RDONLY);
    read(fd_child, buf, sizeof(buf));
    printf("\ntext_child.txt:\n %s", buf);

    fd_parent = open("/home/sonya/CLionProjects/proc8/text_parent.txt", O_RDONLY);
    read(fd_parent, buf, sizeof(buf));
    printf("\ntext_parent.txt:\n %s", buf);

    return 0;
}

