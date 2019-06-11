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

int child_closed = 0;

void handler(int signal) {
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0) {
		child_closed++;
	}
}

long long int get_child_qid(char *b, int *pos) {
	int qid = 0;
	while(*b != '_') {
		(*pos)++;
		int a = *b -'0';
		qid += a;
		qid *= 10;
		b++;
	}
	(*pos)++;
	return qid/10;
}
		


char *process_buf(char *b) {
	if(*b == '1')
		return "Yes";
	if(*b == '0')
		return "No";
	return "Unknown";
}

char *convert(long long int n) {
	int r = 0, m=n;
	while (n != 0) {
		n /= 10;
		r++;
	}

	char *result_string = malloc(r+1);
	
	for(int i=1; i<=r; i++) {
	    *(result_string+r-i) = (char)(m % 10) + '0';
	    m /= 10;
	    
	}
	return result_string;
}
	

int main(void) {
	long long int msgqid; /*Queue ID = msgget()*/
	
	pid_t childs[10];
	
	msgqid = msgget(IPC_PRIVATE,IPC_CREAT | 0666);
	if(msgqid == -1) {
		perror("msgget error");
		exit(1);
	}

	char *msgqid_char;
	msgqid_char = convert(msgqid);

	static struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	
	sigset_t mask;
	sigemptyset(&mask);
	act.sa_mask = mask;

	sigaction(SIGCHLD,&act,NULL);

	for(int i = 0; i < 10; i++) {
		int fork_res = fork();
		char i_char[1];
		sprintf(i_char,"%d",i);
		char *argv[] = { "child", i_char, msgqid_char, NULL };

		switch(fork_res) {
			case -1:
				perror("fork() failed");
				exit(1);
			case  0:
				execv("child",argv);
				perror("exec failed");
				exit(1);

			default:
				childs[i] = fork_res;
		}
	}

	int rcv_res, snd_res;
	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf, buf_s;
	
	while(1) {
		if(child_closed == 10) break;

		char *to_send;
		long long int cqid;
			/* Process read own queue*/
		rcv_res = msgrcv(msgqid,&buf,MAX_SZ,0,IPC_NOWAIT);
		if(rcv_res != -1) {
			int pos = 0;
			to_send = process_buf(buf.mtext);/*Create message from child*/

			buf_s.mtype = buf.mtype; /*mtype = child message queue*/
			strcpy(buf_s.mtext, to_send);

			snd_res = msgsnd(buf.mtype,&buf_s,strlen(buf_s.mtext)+1,0);/*send to child queue*/
			memset(buf_s.mtext,'\0',sizeof(buf_s.mtext));
			if(snd_res == -1) {
				perror("send failed");
				printf("\n");
			}


		}
	
	}
	
	msgctl(msgqid,IPC_RMID,NULL);
}
