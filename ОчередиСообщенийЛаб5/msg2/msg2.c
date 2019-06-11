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

	if(msgqid == -1) {
		perror("msgget error");
		exit(1);
	}

	static struct msqid_ds ds;
	msgctl(msgqid,IPC_STAT,&ds);
	printf("Owner's UID: %d\n", ds.msg_perm.uid);
    	printf("Owner's GID: %d\n", ds.msg_perm.gid);
    	printf("Message stime: %d\n", ds.msg_stime);
    	printf("Message rtime: %d\n", ds.msg_rtime);
    	printf("Message ctime: %d\n", ds.msg_ctime);
    	printf("Number of messages: %d\n", ds.msg_qnum);
    	printf("Maximum number of bytes: %d\n", ds.msg_qbytes);

	int rcv_res = msgrcv(msgqid,&buf,50,type,0);

	if(rcv_res == -1) {
		perror("rcv_res");
	}

	printf("%s",buf.mtext);
}
