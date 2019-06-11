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

	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf, buf_s;



	buf.mtype = my_pid; /*Message has type that is equal pid*/
	
	int digit = my_pid % 3 + 1; /*to send*/
	int stop = 0;

	*(buf.mtext) = (char)digit + '0';

	int snd_res, rcv_res;
	long long int msgqid = strtol(argv[2],NULL,10);	

	static struct msgid_ds *ds;
	int chnum = strtol(argv[1],NULL,10);
	buf_s.mtype = chnum + 1;

	while(stop < 5) { 
	stop += digit;

	/*Кто последний отправил сообщение*/
	snd_res = msgsnd(msgqid,&buf,strlen(buf.mtext)+1,0); /*request*/
	if(snd_res == -1) {
		perror("child send failed()");
		printf("\n");
	}


	rcv_res = msgrcv(msgqid,&buf_s,MAX_SZ,chnum + 1,0);
	if(rcv_res != -1) {
	  printf("\nchild #%s recieved message: %s", argv[1], buf_s.mtext); 
	}
	}

	return 0;
}
