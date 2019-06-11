#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_SZ 45

int main(int argc, char *argv[]) {
	pid_t my_pid = getpid();
	//setpgrp();
	
	long long int msgqid = msgget(IPC_PRIVATE,IPC_CREAT | 0666);
	if(msgqid == -1) {
		perror("msgget error");
		exit(1);
	}

	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf, buf_s;

	buf.mtype = msgqid; /*Message has type that is equal msg queue id*/
	
	int digit = my_pid % 3 + 1; /*to send*/

	*(buf.mtext) = (char)digit + '0';

	int snd_res, rcv_res;
	long long int parent_qid = strtol(argv[2],NULL,10);

	//sleep(2); /*8 пункт*/

	snd_res = msgsnd(parent_qid,&buf,strlen(buf.mtext)+1,IPC_NOWAIT);
	//printf("%d\n",snd_res);
	if(snd_res == -1) {
		perror("child send failed()");
		printf("\n");
		exit(1);
	}

	int my_num = strtol(argv[1], NULL, 10) + 1;	

	rcv_res = msgrcv(parent_qid,&buf_s,MAX_SZ,my_num,0);
	
	strcpy(buf.mtext, "Hello, client!");

	snd_res = msgsnd(parent_qid,&buf,strlen(buf.mtext)+1,0);
	if(snd_res == -1) {
		perror("child send failed()");
		printf("\n");
	}

	rcv_res = msgrcv(msgqid,&buf_s,MAX_SZ,msgqid,0);
	if(rcv_res != -1) {
		printf("\nchild #%s recieved message: %s", argv[1], buf_s.mtext); 
	}

	return 0;
}
