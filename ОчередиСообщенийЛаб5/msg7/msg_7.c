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
long long int msgqid;

void handler(int signal) {
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0) {
		child_closed++;
	}
}

void sighandler(int signal) {
	msgctl(msgqid,IPC_RMID,NULL);
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
	 /*Queue ID = msgget()*/
	
	pid_t childs[2];
	
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

	for(int i = 0; i < 2; i++) {
		int fork_res = fork();
		char i_char[1];
		sprintf(i_char,"%d",i);
		char *argv[] = { "child7", i_char, msgqid_char, NULL };

		switch(fork_res) {
			case -1:
				perror("fork() failed");
				exit(1);
			case  0:
				execv("child7",argv);
				perror("exec failed");
				exit(1);

			default:
				childs[i] = fork_res;
		}
	}

	int rcv_res, snd_res, ch_1, ch_2;
	struct {
		long mtype;
		char mtext[MAX_SZ];
	} buf, buf_s;

	signal(SIGINT, sighandler);

	rcv_res = msgrcv(msgqid,&buf,MAX_SZ,0,0);
	ch_1 = buf.mtype;
	rcv_res = msgrcv(msgqid,&buf,MAX_SZ,0,0);
	ch_2 = buf.mtype;

	//printf("%d %d\n",ch_1,ch_2);

	buf_s.mtype = 1;
	msgsnd(msgqid, &buf_s,strlen(buf_s.mtext)+1,0);
	buf_s.mtype = 2;
	msgsnd(msgqid, &buf_s,strlen(buf_s.mtext)+1,0);

	/*получены идентификаторы очередей дочерних процессов*/
        /*ОЧЕРЕДИ СООБЩЕНИЙ СУЩЕСТВУЮТ, ПОКА ИХ НЕ УДАЛЯТ*/
	while(1) {
		
		/*Parent rcvs message from child in the own queue*/
		if(child_closed == 2) break;
		rcv_res = msgrcv(msgqid,&buf,MAX_SZ,ch_1,IPC_NOWAIT); /*get msg from chld1*/
		if(rcv_res != -1) {
			buf_s.mtype = ch_2;
			strcpy(buf_s.mtext, buf.mtext);
			
			snd_res = msgsnd(ch_2, &buf_s,strlen(buf_s.mtext)+1,0);/*send to child2 queue*/
			memset(buf_s.mtext,'\0',sizeof(buf_s.mtext));
			if(snd_res == -1) {
				perror("send failed");
				printf("\n");
			}
			continue;
		}

		rcv_res = msgrcv(msgqid,&buf,MAX_SZ,ch_2,IPC_NOWAIT); /*get msg from chld2*/
		if(rcv_res != -1) {
			buf_s.mtype = ch_1;
			strcpy(buf_s.mtext, buf.mtext);
			
			snd_res = msgsnd(ch_1, &buf_s,strlen(buf_s.mtext)+1,0);/*send to child1 queue*/
			memset(buf_s.mtext,'\0',sizeof(buf_s.mtext));
			if(snd_res == -1) {
				perror("send failed");
				printf("\n");
			}
		}
		

	}
	
	msgctl(msgqid,IPC_RMID,NULL);
}
