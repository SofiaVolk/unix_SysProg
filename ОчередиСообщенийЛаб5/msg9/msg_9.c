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
	long long int msgqid_send, msgqid_resv; /*Queue ID = msgget()*/
	
	pid_t childs[10];
	
	msgqid_send = msgget(IPC_PRIVATE,IPC_CREAT | 0666);
	if(msgqid_send == -1) {
		perror("msgget error");
		exit(1);
	}
	msgqid_resv = msgget(IPC_PRIVATE,IPC_CREAT | 0666);
	if(msgqid_resv == -1) {
		perror("msgget error");
		exit(1);
	}
	

	char *msgqid_char_send, *msgqid_char_resv;
	msgqid_char_send = convert(msgqid_send); /*resp*/
	msgqid_char_resv = convert(msgqid_resv); /*requ*/


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
		char *argv[] = { "child1", i_char, msgqid_char_send, msgqid_char_resv, NULL };

		
		switch(fork_res) {
			case -1:
				perror("fork() failed");
				exit(1);
			case  0:
				execv("child1",argv);
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
	
	int ch = 0;	

	while(1) {
		ch = 0;
		if(child_closed == 10) break;
		while(ch < 10) {
			char *to_send;
			/*Get message from request queue*/
			rcv_res = msgrcv(msgqid_resv,&buf,MAX_SZ,childs[ch],IPC_NOWAIT);
			
			if(rcv_res != -1) {
				/*Get request*/
				to_send = process_buf(buf.mtext);/*Get message from child*/

				buf_s.mtype = childs[ch]; /*type = child_pid*/

				strcpy(buf_s.mtext, to_send);
			/*Send message to response queue*/
				snd_res = msgsnd(msgqid_send,&buf_s,strlen(buf_s.mtext)+1,0);/*send*/
				memset(buf_s.mtext,'\0',sizeof(buf_s.mtext));
				if(snd_res == -1) {
					perror("parent send failed");
					printf("\n");
				}

		  }
		ch++;	
		}
	}
	msgctl(msgqid_send,IPC_RMID,NULL);
	msgctl(msgqid_resv,IPC_RMID,NULL);
}
