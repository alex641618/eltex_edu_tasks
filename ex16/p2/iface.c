#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/ioctl.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

short cexit=0;

struct reg {
    long message_type;
    pid_t regc;
};

void hdl(int sig) {
	cexit = 1;
}

int main(int argc, char* argv[]) {
	char buf[200];
	int ret, log = 3;
	key_t key;
	struct sigaction act;
	struct reg logs;
	int queue;
	memset(&act, 0, sizeof(act));
	act.sa_handler = hdl;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT);
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);
	FILE *pf = fopen("history", "r");
	errno=0;
	key = ftok(argv[1], 'a');
	
	if (key < 0) {
		perror("ftok");
		return -1;
	}
	errno=0;
	queue = msgget(key, 0);
	if (queue == -1) { 
		perror("msgget");
		return -1;
	}

	pid_t reg = getpid();
	logs.message_type = log;
	logs.regc = reg;
	ret = msgsnd(queue, &logs, sizeof(pid_t), IPC_NOWAIT);
	if (ret!=0) {
		perror("msgsndSTART");
		printf("ret = %d\n", ret);
		return -1;
	}

	while (cexit == 0) {
		if (fgets(buf, 200, pf) != NULL) {
			printf("%s", buf);
		} 
	}

	fclose(pf);
	return 0;
}