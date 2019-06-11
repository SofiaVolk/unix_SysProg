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
	long long int msgqid; /*Queue ID = msgget()*/
	
	pid_t childs[3];
	
	msgqid = msgget(IPC_PRIVATE,IPC_CREAT | 0660); 
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

	for(int i = 0; i < 3; i++) {
		int fork_res = fork();
		char i_char[1];
		sprintf(i_char,"%d",i);
		char *argv[] = { "child2", i_char, msgqid_char, NULL };

		
		switch(fork_res) {
			case -1:
				perror("fork() failed");
				exit(1);
			case  0:
				execv("child2",argv);
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
		if(child_closed == 3) break;
		while(ch < 3) {
			char *to_send;
			/*Get message from child, type = child*/
			rcv_res = msgrcv(msgqid,&buf,MAX_SZ,childs[ch],IPC_NOWAIT);
			if(rcv_res != -1) {
				/*the get request*/
				to_send = process_buf(buf.mtext);

	

				buf_s.mtype = ch + 1; /*type = ch*/

				strcpy(buf_s.mtext, to_send);

				snd_res = msgsnd(msgqid,&buf_s,strlen(buf_s.mtext)+1,0);/*send*/
				memset(buf_s.mtext,'\0',sizeof(buf_s.mtext));
				if(snd_res == -1) {
					perror("parent send failed");
					printf("\n");
				}
		  	}
			
		ch++;	
		}
	}
	msgctl(msgqid,IPC_RMID,NULL);
}
