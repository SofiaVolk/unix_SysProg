#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char str[] = "\nsignal SIGINT\n";

void action(int sig){
    write(1, str, sizeof(str));
 //   signal(SIGINT, SIG_DFL);
}

int main() {
    signal(SIGINT, action);

  // while(1){
  //      printf("TEST TEXT\n");
  //      sleep(1);
  //  }
getchar();
//pause();
}

