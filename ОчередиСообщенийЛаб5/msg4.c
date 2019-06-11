#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
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
	int type = strtol(argv[2],NULL,10); 

	if(msgqid == -1) {
		perror("msgget error");
		exit(1);
	}

	int rcv_res = msgrcv(msgqid,&buf,10,type,0);

	if(rcv_res == -1) {
		perror("rcv_res"); printf("%d\n", errno);
	}

	printf("%s\n",buf.mtext);
}
