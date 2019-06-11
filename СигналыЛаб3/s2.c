#include <stdio.h>
#include <signal.h>
#include <unistd.h>

char str[] = "\nsignal SIGINT\n";

void action(int sig){
    write(1, str, sizeof(str));
}

int main() {
    struct sigaction act;
    act.sa_handler = action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
//act.sa_flags = SA_RESETHAND;
    act.sa_flags = SA_RESTART;

    sigaction(SIGINT, &act, 0);

    /*while(1){
        printf("TEST TEXT\n");
        sleep(1);
    }*/
//getchar();
pause();
}

