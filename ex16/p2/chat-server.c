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

#define PERMISSIONS_QUEUE 0660
#define BUFFER_SIZE 100

short cexit=0;


void sig_winch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

struct message {
    long message_type;
    char buf [200];
};

struct reg {
    long message_type;
    pid_t regc;
};


void sig_handler(int sig) {
	cexit=1;
}

int main(int argc, char* argv[]) {
	int queue;
	char chatfile[10] = "./history";
	char cmd[BUFFER_SIZE], buf[BUFFER_SIZE];
	pid_t reg;
	key_t key;
	int i=0, ret = 0, f=0;
	long add = 1, del = 2, log = 3;
	struct reg adds;
	struct reg dels;
	struct reg logs;
	struct message message;
	pid_t *list = malloc(10*sizeof(pid_t));
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT);
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);
	char *pwd = getenv("PWD");
	errno=0;
	FILE *pf = fopen(chatfile, "a+");
	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	noecho();
	curs_set(0);
	if (pf == NULL) {
		perror("fopen");
		endwin();
		return -1;
	}

	errno=0;
	key = ftok(argv[0], 'a');
	if (key < 0) {
		perror("ftok");
		endwin();
		return -1;
	}

	errno=0;
	queue = msgget(key, IPC_CREAT | PERMISSIONS_QUEUE);
	if (queue == -1) { 
		perror("msgget");
		printf("ret= %d\n", queue);
		fclose(pf);
		endwin();
		return -1;
	}
	sprintf(cmd, "xterm -e '%s/iface chat-server' &", pwd);
	system(cmd);
	errno=0;
	ret = msgrcv(queue, &logs, 0, log, IPC_NOWAIT);
	if (ret<0 && errno!=EAGAIN && errno!=ENOMSG) {
		perror("msgrcv");
		printf("ret = %d\n", ret);
		msgctl(queue, IPC_RMID, NULL);
		fclose(pf);
		endwin();
		return -1;
	}
	while (errno == ENOMSG) {
		errno=0;
		ret = msgrcv(queue, &logs, sizeof(pid_t), log, IPC_NOWAIT);
		if (ret<0 && errno!=EAGAIN && errno!=ENOMSG) {
			perror("msgrcv");
			printf("ret = %d\n", ret);
			msgctl(queue, IPC_RMID, NULL);
			fclose(pf);
			endwin();
			return -1;
		}
	}

	int whilec = 0;
	int deb=0;
	while (cexit==0) {
/*Получение сообщений от клиенртов*/
		for (f=0; f<i; f++) {
			ret = msgrcv(queue, &message, 200, list[f], IPC_NOWAIT);
			if (ret<0 && errno!=EAGAIN && errno!=ENOMSG) {
				perror("msgrcvFOR");
				printf("ret = %d\n", ret);
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
				endwin();
				kill(logs.regc, SIGINT);
				return -1;
			} else if (ret > 0 ) {
				sprintf(buf, "client%d: %s", list[f], message.buf);
				ret = write(fileno(pf), buf, strlen(buf)+1);
				if (ret < 0) {
					perror("writeFOR");
					msgctl(queue, IPC_RMID, NULL);
					fclose(pf);
					endwin();
					kill(logs.regc, SIGINT);
				}
			}
		}


/*Добавление клиента в чат*/
		errno=0;
		ret = msgrcv(queue, &adds, sizeof(pid_t), add, IPC_NOWAIT);
		if (ret<0 && errno!=EAGAIN && errno!=ENOMSG) {
			perror("msgrcv1");
			printf("ret = %d\n", ret);
			msgctl(queue, IPC_RMID, NULL);
			fclose(pf);
			endwin();
			kill(logs.regc, SIGINT);
			return -1;
		} else if (ret > 0 ) {
			errno=0;
			list = (pid_t*) realloc(list, i+1*sizeof(pid_t));
			if (list==NULL) {
				perror("realloc1");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
				endwin();
				kill(logs.regc, SIGINT);
			}
			list[i] = adds.regc;
			errno=0;
			sprintf(buf, "add client%d\n", adds.regc);
			ret = write(fileno(pf), buf, strlen(buf)+1);
			if (ret < 0) {
				perror("write1");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
				endwin();
				kill(logs.regc, SIGINT);
			}
			i++;
			deb=0;
		}

/*Удаление клиента из чата*/
		ret = msgrcv(queue, &dels, sizeof(pid_t), del, IPC_NOWAIT);
			if (ret<0 && errno!=EAGAIN && errno!=ENOMSG) {
			perror("msgrcv2");
			printf("ret = %d\n", ret);
			msgctl(queue, IPC_RMID, NULL);
			fclose(pf);
			endwin();
			kill(logs.regc, SIGINT);
			return -1;
		} else if (ret > 0 ) {
			errno=0;
			sprintf(buf, "del client%d\n", dels.regc);
			ret = write(fileno(pf), buf, strlen(buf)+1);
			if (ret < 0) {
				perror("write2");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
				endwin();
				kill(logs.regc, SIGINT);
			}
			int s=i;
			for(s; s>=0; s--) {
				if (list[s-1] == dels.regc) {
					int k = s-1;
					while(k!=i) {
						list[k] = list[k+1];
						k++;
					}
				}
			}
			i--;
			deb=0;
			list = (pid_t*) realloc(list, i+1*sizeof(pid_t));
			if (list==NULL) {
				perror("realloc2");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
				endwin();
				kill(logs.regc, SIGINT);
			}
		}
		if (deb == 0 ) {
			clear();
			move(0, 0);
			printw("clients:\n");
			for (int k = 0; k<i; k++) {
				printw("%d ", list[k]);
			}
			printw("\n");
			deb = 1;
		}
		refresh();
		//whilec++;
	}

	msgctl(queue, IPC_RMID, NULL);
	fclose(pf);
	sprintf(cmd, "rm -rf %s", chatfile); // заменить на unlink()
	system(cmd);
	kill(logs.regc, SIGINT);
	endwin();
	printf("\nexit\n");
	return 0;
}