#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SZ 50

int main(int argc, char *argv[]) {
	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf;

	long long int msgqid = strtol(argv[1],NULL,10); 

	int res_cnlt = msgctl(msgqid,IPC_RMID,0);
	if(res_cnlt == -1) {
		perror("removing failed"); printf("%d\n", errno);
	}

	
}
