#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMISSIONS_QUEUE 0660
#define BUFFER_SIZE 100

short cexit=0;

/*struct message_text {
    int uid;
    char buf [200];
};*/

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
	long add = 1, del = 2; 
	struct reg adds;
	struct reg dels;
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

	errno=0;
	FILE *pf = fopen(chatfile, "a+");
	if (pf == NULL) {
		perror("fopen");
		return -1;
	}

	errno=0;
	key = ftok(argv[0], 'a');
	if (key < 0) {
		perror("ftok");
		return -1;
	}

	errno=0;
	queue = msgget(key, IPC_CREAT | PERMISSIONS_QUEUE);
	if (queue == -1) { 
		perror("msgget");
		printf("ret= %d\n", queue);
		fclose(pf);
		return -1;
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
				return -1;
			} else if (ret > 0 ) {
				sprintf(buf, "%d: %s\n", list[f], message.buf);
				ret = write(fileno(pf), buf, strlen(buf)+1);
				if (ret < 0) {
					perror("writeFOR");
					msgctl(queue, IPC_RMID, NULL);
					fclose(pf);
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
			return -1;
		} else if (ret > 0 ) {
			errno=0;
			list = (pid_t*) realloc(list, i+1*sizeof(pid_t));
			if (list==NULL) {
				perror("realloc1");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
			}
			list[i] = adds.regc;
			errno=0;
			sprintf(buf, "add client: %d\n", adds.regc);
			ret = write(fileno(pf), buf, strlen(buf)+1);
			if (ret < 0) {
				perror("write1");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
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
			return -1;
		} else if (ret > 0 ) {
			errno=0;
			sprintf(buf, "del client: %d\n", dels.regc);
			ret = write(fileno(pf), buf, strlen(buf)+1);
			if (ret < 0) {
				perror("write2");
				msgctl(queue, IPC_RMID, NULL);
				fclose(pf);
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
			}
		}
		if (deb == 0 ) {
			for (int k = 0; k<i; k++) {
				printf("%d ", list[k]);
			}
			printf("\n\n");
			deb = 1;
		}
		whilec++;
	}

	msgctl(queue, IPC_RMID, NULL);
	fclose(pf);
	sprintf(cmd, "rm -rf %s", chatfile); // заменить на unlink()
	system(cmd);
	printf("\nexit\n");
	return 0;
}