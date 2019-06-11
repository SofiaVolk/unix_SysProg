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

#define MAX_SZ 45

int main(void) {
	long long int msgqid = msgget((key_t)1234,IPC_CREAT | 0660); 
	if(msgqid == -1) {
		perror("msgget error");
		exit(1);
	}

	
	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf;

	buf.mtype = 100;
	strcpy(buf.mtext,"long long int long");
	msgsnd(msgqid,&buf,strlen(buf.mtext)+1,0);
	memset(buf.mtext,'\0',sizeof(buf.mtext));

	buf.mtype = 200;
	strcpy(buf.mtext,"char");
	msgsnd(msgqid,&buf,strlen(buf.mtext)+1,0);
	memset(buf.mtext,'\0',sizeof(buf.mtext));

	buf.mtype = 300;
	strcpy(buf.mtext,"float");
	msgsnd(msgqid,&buf,strlen(buf.mtext)+1,0);
	memset(buf.mtext,'\0',sizeof(buf.mtext));
	
	printf("msgqid = %lld", msgqid);

	exit(0);
}
