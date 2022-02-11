#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <pthread.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ADDRLENTH 16
#define MSGLEN 24

short cexit=0;

struct thread_info {
	pthread_t thread_id;
	int       thread_num;
};

void hdl(int sig) {
	printf("exit\n");
	cexit = 1;
}

struct message {
	unsigned short port_server_thread;
};

static void *tfunc(void *args) {
	struct thread_info* stru;
	int status;
	pid_t tid;
	stru = (struct thread_info*) args;
	struct sockaddr_in server_thread;
	struct sockaddr_in client_thread;
	bzero(&client_thread, sizeof(client_thread));
	//struct message* pmsg;
	char msg[MSGLEN];
	//pmsg = msg;
	int fromlen_thread = sizeof(client_thread);

	int sock_thread = socket(AF_INET, SOCK_DGRAM, 17);

	tid = gettid();
	server_thread.sin_family = AF_INET;
	server_thread.sin_addr.s_addr = htonl(INADDR_ANY);
	server_thread.sin_port=stru->thread_num;
	char cmd[300];
	//sprintf(cmd, "echo .thread_num=%d\n", htons(stru->thread_num));
	system(cmd);

	errno=0;
	int ret=bind(sock_thread, (struct sockaddr*)&server_thread, sizeof(server_thread));

	if (ret!=0){
		sprintf(cmd, "echo bind\ error%d\n", errno);
		system(cmd);
		goto cleanup_thread;
	}
	while (1) {
		ret=recvfrom(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, &fromlen_thread);
		if (ret<0){
			sprintf(cmd, "echo recvfrom\ error!");;
			system(cmd);
		}
		else {
			sprintf(cmd, "echo %d\ recieved:\ %s\ \[port\ %d\]", (int)tid, msg, ntohs(server_thread.sin_port));
			system(cmd);
		}
		
		if (strcmp(msg, "END") == 0) {
			strcpy(msg, "END");
			ret=sendto(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, fromlen_thread);
			if (ret<0) {
				sprintf(cmd, "echo sendto\ error!");;
				system(cmd);
			}
			else {
				sprintf(cmd, "echo %d\ sended:\ %s\ \[port\ %d\]", (int)tid, msg, ntohs(server_thread.sin_port));
				system(cmd);
			}
			status = 0;
			close(sock_thread);
			pthread_exit((void *)&status);
		} else {
			strcpy(msg, "ok");
			errno=0;
			ret=sendto(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, fromlen_thread);
			if (ret<0) {
				sprintf(cmd, "echo sendto\ error!");;
				system(cmd);
			}
			else {
				sprintf(cmd, "echo %d\ sended:\ %s\ \[port\ %d\]", (int)tid, msg, ntohs(server_thread.sin_port));
				system(cmd);
			}
		}

	}
	status = 0;
	close(sock_thread);
	pthread_exit((void *)&status);

cleanup_thread:
	
	close(sock_thread);
	status = 1;
	pthread_exit((void *)&status);
}

int main (int argc, char* argv[]) {
	struct sigaction act;
	struct message* pmsg;
	memset(&act, 0, sizeof(act));
	act.sa_handler = hdl;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT); 
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);

	char clientaddr[ADDRLENTH]={0}, ifname[IFNAMSIZ]={0}, msg[MSGLEN];
	unsigned short port;
	struct sockaddr_in client;
	bzero(&client, sizeof(client));
	struct sockaddr_in server;

	if (argc > 1) {
		int k=1;
		while (k<argc) {
			if (strcmp(argv[k], "-a") == 0) {
				strcpy(clientaddr, argv[k+1]);
				printf("clientaddr = %s\n", clientaddr);
				k+=2;
			} else if (strcmp(argv[k], "-p") == 0) {
				errno=0;
				port = (unsigned short)strtol(argv[k+1], NULL, 0);
				if (port == 0 || port == LONG_MAX || port == LONG_MIN) {
					perror("strtol:");
				}
				printf("port = %hu\n", port);
				k+=2;
			} else if (strcmp(argv[k], "-i") == 0) {
				strcpy(ifname, argv[k+1]);
				printf("ifname = %s\n", ifname);
				k+=2;
			} else if (strcmp(argv[k], "-h") == 0 || strcmp(argv[k], "--help") == 0) {
				printf("usage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
				return 0;
			} else {
				k++;
				printf("invalid argument\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
				printf("enter -h or --help for help usege\n");
				return -1;
			}
		}

	} else {
		printf("usage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
		printf("enter -h or --help for help usege\n");
		return -1;
	}
	
	errno=0;
	int sock = socket(AF_INET, SOCK_DGRAM, 17);

	if (sock == -1) {
		perror("socket");
		return -1;
	}

	if (ifname)
		setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)+1);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port=htons(port);
	errno=0;
	int fromlen = sizeof(client);
	memset(msg, 0x00, MSGLEN);
	pmsg = (struct message*) msg;

	errno=0;
	int ret=bind(sock, (struct sockaddr*)&server, sizeof(server));
	
	if (ret!=0){
		perror("bind");
		goto cleanup;
	}
	
	struct thread_info* tinfo = NULL;
	int s, tnum = 0;;
	void *res;

	while (1) {

		if (cexit==1)
			break;

		errno=0;
		ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, &fromlen);
		if (ret<0)
			perror("recvfrom");
		else {
			printf("recieved: %s\n", msg);
			tnum++;
			tinfo = realloc(tinfo, tnum * sizeof(struct thread_info));
			tinfo[tnum-1].thread_num = htons(7777+(tnum-1));
			pmsg->port_server_thread = htons(7777+(tnum-1));
			pthread_create(&tinfo[tnum-1].thread_id, NULL, tfunc, &tinfo[tnum-1]);
			//s = pthread_join(tinfo[tnum-1].thread_id, (void**) &res);
			printf("s = %d\n", s);
		}

		if (cexit==1)
			break;

		errno=0; 
		ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, fromlen);

		if (ret<0)
			perror("sendto");
		else
			printf("sended: %d\n", ntohs(*(int*)msg));

	}

	close(sock);
	return 0;

cleanup:
	
	close(sock);
	return -1;
}
