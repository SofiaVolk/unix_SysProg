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



	buf_s.mtype = 2; /*Message has type that is equal pid*/
	*buf_s.mtext = '2';


	int stop = 0;


	int snd_res, rcv_res;
	long long int msgqid = strtol(argv[2],NULL,10);	

	static struct msgid_ds *ds;
	//int chnum = strtol(argv[1],NULL,10);
	//buf_s.mtype = chnum + 1;

	while(stop < 5) { 
		stop += 1;

		rcv_res = msgrcv(msgqid,&buf,MAX_SZ,1,0);
	/*if(rcv_res != -1) {
	  perror("child rcv");
	}*/
		write(1, "1", 1);

		snd_res = msgsnd(msgqid,&buf_s,strlen(buf.mtext)+1,0); /*request*/
		if(snd_res == -1) {
			perror("child send failed()");
			printf("\n");
		}sleep(1);

	}

	return 0;
}
