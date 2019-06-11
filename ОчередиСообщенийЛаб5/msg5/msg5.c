#include <sys/types.h>
#include <unistd.h>
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
	long long int key =  ftok (argv[1],*argv[2]);
	int type = strtol(argv[3],NULL,10);

	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf;

	long long int msgqid = msgget(key, 0660); 

	int res_cnlt = msgctl(msgqid,IPC_RMID,NULL);
	if(res_cnlt == -1) {
		perror("removing failed");
	}

	
}
