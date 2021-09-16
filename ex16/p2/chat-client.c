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

struct message_text {
    int uid;
    char buf [200];
};

struct message {
    long message_type;
    char buf [200];
};

struct reg {
    long message_type;
    pid_t regc;
};


void hdl(int sig) {
	cexit = 1;
}

int main(int argc, char* argv[]) {
	int queue;
	pid_t reg;
	key_t key;
	int i=0, ret = 0;
	long add = 1, del = 2; 
	struct reg adds;
	struct reg dels;
	struct message message;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = hdl;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT); 
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);
	system("rm -rf chat-client");

	if (argv[1] == NULL) {
		printf("Передайте имя бинарного файла сервера\n");
		return -1;
	}
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
		printf("try run server\n");
		return -1;
	}
	reg = getpid();
	errno=0;
	adds.message_type = add;
	adds.regc = reg;

/*оповещение сервера о присоединении*/
	ret = msgsnd(queue, &adds, sizeof(pid_t), IPC_NOWAIT);
	if (ret!=0) {
		perror("msgsndSTART");
		printf("ret = %d\n", ret);
		return -1;
	}
	//int cwhile=0;
	printf("you are client%d, write your messages\n", reg);
	while(cexit==0) {
		if (fgets(message.buf, 200, stdin) != NULL) {
			message.message_type = reg;
			ret = msgsnd(queue, &message, 200, IPC_NOWAIT);
			if (ret!=0) {
				perror("msgsndWHILE");
				printf("ret = %d\n", ret);
				return -1;
			}
		}	
	}
	dels.message_type = del;
	dels.regc = reg;
/*оповещение сервера об отсоединении*/
	ret = msgsnd(queue, &dels, sizeof(pid_t), IPC_NOWAIT);
	if (ret!=0) {
		perror("msgsndEND");
		printf("ret = %d\n", ret);
		return -1;
	}	
	printf("\nexit\n");
	return 0;
}