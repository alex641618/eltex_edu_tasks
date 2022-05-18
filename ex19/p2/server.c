#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

// port spase 7777...65535
#define MAXPORT 65535
#define STARTPORT 7777
#define ADDRLENTH 16
#define MINTHREAD 5
#define MSGLEN 24
#define MAXCLIENT 100 // no more 57759

const unsigned short NUMPORT = (MAXPORT-STARTPORT) + 1;

typedef enum {FALSE, TRUE} bool;

bool cexit = FALSE;
bool got_iface = FALSE;

typedef struct message {
	unsigned short port_server_thread;
} message_s;

typedef struct args {
	char clientaddr[ADDRLENTH];
	char ifname[IFNAMSIZ];
	unsigned short port;
} args_s;

args_s arg;

sem_t semaphore;
typedef struct port {
	unsigned short port;
	bool flag;
	bool got_client;
	bool ready_thread;
} port_s;

port_s* ports = NULL;

typedef struct thread_info {
	pthread_t thread_id;
	int       thread_num;
} thread_info_s;

void hdl(int sig) {
	printf("exit\n");
	cexit = TRUE;
}

int count_threads(){
	int i = 0;
	int counter = 0;

	for (i=0; i<NUMPORT; i++){
		if(ports[i].flag == TRUE){
			counter++;
		}
	}

	return counter;
}

int find_free_childserver(){
	int i = 0;

	for (i=0; i<MINTHREAD; i++){
		if(ports[i].got_client == FALSE){
			return i;
		}
	}

	return -1;
}

static void *tfunc(void *arguments) {
	struct thread_info* stru;
	int valp;
	sem_wait(&semaphore);
	int my_num = count_threads();// critical zone
	ports[my_num].flag = TRUE;// critical zone
	ports[my_num].got_client = FALSE; // critical zone
	sem_post(&semaphore);
	thread_info_s* thread;
	struct sockaddr_in server_thread;
	struct sockaddr_in client_thread;
	bzero(&client_thread, sizeof(client_thread));
	stru = (thread_info_s*) arguments;
	char cmd[300], msg[MSGLEN];
	int fromlen_thread = sizeof(client_thread);
	int i, status;
	bool check, f = FALSE;

	int sock_thread = socket(AF_INET, SOCK_DGRAM, 17);

	server_thread.sin_family = AF_INET;
	server_thread.sin_addr.s_addr = htonl(INADDR_ANY);
	server_thread.sin_port = htons(stru[my_num].thread_num);

	//sprintf(cmd, "echo id=%ld_my_num=%d_port=%d\n", stru[my_num].thread_id, my_num, ntohs(server_thread.sin_port));
	//system(cmd);

	errno=0;
	int ret=bind(sock_thread, (struct sockaddr*)&server_thread, sizeof(server_thread));
	if (ret!=0){
		sprintf(cmd, "echo bind\ error%d\n", errno);
		system(cmd);
		goto cleanup_thread;
	}

	while(cexit == FALSE){

recvfrom:
		sem_wait(&semaphore);
		ports[my_num].ready_thread = TRUE;// critical zone
		sem_post(&semaphore);
		ret=recvfrom(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, &fromlen_thread); // 1пс получает порт 
		if (ret<0){
			sprintf(cmd, "echo recvfrom\ error!");
			system(cmd);
			goto cleanup_thread;
		}
		else {
			if (f == FALSE){
				sem_wait(&semaphore);
				check = ports[my_num].got_client;// critical zone
				sem_post(&semaphore);
				f = TRUE;
			}

			if (check == TRUE){
				sprintf(cmd, "echo %ld\ recieved:\ %s\ \[port\ %d\]", stru[my_num].thread_id, msg, ntohs(server_thread.sin_port));
				system(cmd);
			} else {
				if (ntohs(*(int*)msg) == (int)ntohs(server_thread.sin_port)){
					sprintf(cmd, "echo %ld\ recieved:\ %d\ \[port\ %d\]", stru[my_num].thread_id, ntohs(*(int*)msg), ntohs(server_thread.sin_port));
					system(cmd);
					sem_wait(&semaphore);
					ports[my_num].got_client = TRUE;// critical zone
					sem_post(&semaphore);
					check = TRUE;
				} else { 
					if(my_num < MINTHREAD)
						goto recvfrom;
					else
						goto cleanup_thread;
				}
			}
			
		}

		if (strcmp(msg, "END") == 0) {
			ret=sendto(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, fromlen_thread);
			if (ret<0) {
				sprintf(cmd, "echo sendto\ error!");
				system(cmd);
				goto cleanup_thread;
			}
			else {
				sprintf(cmd, "echo %ld\ sended:\ %s\ \[port\ %d\]", stru[my_num].thread_id, msg, ntohs(server_thread.sin_port));
				system(cmd);
			}

			if (my_num < MINTHREAD) {
				sem_wait(&semaphore);
				ports[my_num].got_client = FALSE; // critical zone
				sem_post(&semaphore);
				f = FALSE;
				check = FALSE;
				goto recvfrom;

			}

			close(sock_thread);
			status = 0;

			sem_wait(&semaphore);
			ports[my_num].got_client = FALSE; // critical zone
			ports[my_num].flag = FALSE;
			sem_post(&semaphore);

			pthread_exit((void *)&status);

		} else {
			strcpy(msg, "ok");
			errno=0;
			ret=sendto(sock_thread, msg, MSGLEN, 0, (struct sockaddr *)&client_thread, fromlen_thread); // 2пс отправляет ответ
			if (ret<0) {
				sprintf(cmd, "echo sendto\ error!");
				system(cmd);
				goto cleanup_thread;
			}
			else {
				sprintf(cmd, "echo %ld\ sended:\ %s\ \[port\ %d\]", stru[my_num].thread_id, msg, ntohs(server_thread.sin_port));
				system(cmd);
			}
		}

	}

	close(sock_thread);
	status = 0;

	sem_wait(&semaphore);
	ports[my_num].got_client = FALSE; // critical zone
	sem_post(&semaphore);

	if(my_num >= 5){
		sem_wait(&semaphore);
		ports[my_num].flag = FALSE;
		sem_post(&semaphore);
	}

	pthread_exit((void *)&status);

cleanup_thread:
	
	sprintf(cmd, "echo clen");
	system(cmd);

	close(sock_thread);
	status = 1;

	sem_wait(&semaphore);
	ports[my_num].got_client = FALSE; // critical zone
	sem_post(&semaphore);

	if(my_num >= 5){
		sem_wait(&semaphore);
		ports[my_num].flag = FALSE; // critical zone
		sem_post(&semaphore);
	}

	pthread_exit((void *)&status);
}
int invalid_addr(char* addr) {
	int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
	if(sscanf(addr, "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4){
		return -1;
	}
	if (a1 < 0 || a1 > 255){
		return -1;
	}
	if (a2 < 0 || a2 > 255){
		return -1;
	}
	if (a3 < 0 || a3 > 255){
		return -1;
	}
	if (a4 < 0 || a4 > 254){
		return -1;
	}

	return 0;
}

int invalid_iface(char* iface) {
	struct if_nameindex *ni;
	int i;
	ni = if_nameindex();

	if (ni == NULL) {
		perror("if_nameindex");
		return -2;
	}

	for (i = 0; ni[i].if_index != 0 && ni[i].if_name != NULL; i++){
		if(strcmp(iface, ni[i].if_name) == 0){
			return 0;
		}
	}
	return -1;
}

int parse_arg(int argc, char* argv[]){
	if (argc > 1) {
		int k=1;
		while (k<argc) {
			if (strcmp(argv[k], "-a") == 0) {
				if (argv[k+1]){
					if(invalid_addr(argv[k+1])){
						printf("invalid address\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
						printf("enter -h or --help for help usege\n");
						return -1;
					} else {
						strcpy(arg.clientaddr, argv[k+1]);
						printf("clientaddr = %s\n", arg.clientaddr);
						k+=2;
					}
				} else {
					printf("invalid address\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
					printf("enter -h or --help for help usege\n");
					return -1;
				}
			} else if (strcmp(argv[k], "-p") == 0) {
				if (argv[k+1]){
					errno=0;
					arg.port = (unsigned short)strtol(argv[k+1], NULL, 0);
					if (arg.port == 0) {
						printf("invalid port\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
						printf("enter -h or --help for help usege\n");
						return -1;
					}
					printf("port = %hu\n", arg.port);
					k+=2;
				} else {
					printf("invalid port\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
					printf("enter -h or --help for help usege\n");
					return -1;
				}
			} else if (strcmp(argv[k], "-i") == 0) {
				if (argv[k+1]){
					int ret = invalid_iface(argv[k+1]);
					if (ret){
						if (ret == -2){
							printf("error when checking interfaces. Plese try again\n");
						} else {
							printf("invalid ifname\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
							printf("enter -h or --help for help usege\n");
							return -1;
						}
					} else {
						strcpy(arg.ifname, argv[k+1]);
						printf("ifname = %s\n", arg.ifname);
						got_iface = TRUE;
						k+=2;
					}
				} else {
					printf("invalid ifname\nusage: server -p [port]\nif are you want use special intarface use -i [interface]\n");
					printf("enter -h or --help for help usege\n");
					return -1;
				}
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

	return 0;
}

int ports_create(){
	int i;

	errno=0;
	ports = malloc(NUMPORT * sizeof(port_s));
	if (ports == NULL){
		perror("malloc");
		return -1;
	}

	for (i=0; i < NUMPORT; i++){
		ports[i].port = STARTPORT + i;
		ports[i].flag = FALSE;
		ports[i].got_client = FALSE;
	}

	return 0;
}

int all_global_free(){

	if (ports != NULL) {
		free(ports);
		ports = NULL;
	}


}

int main (int argc, char* argv[]) {
	
	int i;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = hdl;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT); 
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);

	struct sockaddr_in server;

	struct sockaddr_in client;
	bzero(&client, sizeof(client));

	if(parse_arg(argc, argv) == -1){
		goto cleanup;
	}

	if(ports_create()){
		printf("error ports_create\n");
		goto cleanup;
	}
	
	errno=0;
	int sock = socket(AF_INET, SOCK_DGRAM, 17);
	if (sock == -1) {
		perror("socket");
		return -1;
	}

	if (got_iface){
		setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, arg.ifname, strlen(arg.ifname)+1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port=htons(arg.port);

	int fromlen = sizeof(client);
	char msg[MSGLEN];
	message_s* pmsg;
	memset(msg, 0x00, MSGLEN);
	pmsg = (struct message*) msg;

	errno=0;
	int ret=bind(sock, (struct sockaddr*)&server, sizeof(server));
	if (ret!=0){
		perror("bind");
		goto cleanup;
	}

	struct thread_info* tinfo = NULL;

	errno = 0;
	tinfo = malloc(MAXCLIENT * sizeof(struct thread_info));
	if (tinfo == NULL){
		perror("tinfo");
		goto cleanup;
	}

	for (i=0; i<MINTHREAD; i++){
		tinfo[i].thread_num = ports[i].port;
	}

	sem_init(&semaphore, 0, 1);
	for (i=0; i<MINTHREAD; i++) {
		//sleep(2);
		pthread_create(&tinfo[i].thread_id, NULL, tfunc, tinfo);
		printf("create %ld\n", tinfo[i].thread_id);
	}
	

	int tnum = 0, tnum_prev = 0;
	while (cexit == FALSE){
	
		int j; 
		errno=0;
		ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, &fromlen);
		printf("msg = %d\n", ntohs(*(int*)msg));
		if (ret<0) {
			perror("recvfrom");
			goto cleanup;
		}
		else {
			

			printf("RECIEVED: %s\n", msg);

			sem_wait(&semaphore);
			i = find_free_childserver();// critical zone
			sem_post(&semaphore);

			

		if (i == -1){
			bool flg, ready;
			sem_wait(&semaphore);
			tnum = count_threads();// critical zone
			sem_post(&semaphore);

			if (tnum < MAXCLIENT){	
				for (j=0; j<MAXPORT; j++){
					sem_wait(&semaphore);
					flg = ports[j].flag;// critical zone
					sem_post(&semaphore);
					if (flg == FALSE){
						sem_wait(&semaphore);
						tinfo[tnum].thread_num = ports[j].port;// critical zone
						sem_post(&semaphore);
						pthread_create(&tinfo[tnum].thread_id, NULL, tfunc, tinfo);
						printf("create %ld\n", tinfo[tnum].thread_id);
						pmsg->port_server_thread = htons(STARTPORT+j);
new_ready:			
						sem_wait(&semaphore);
						ready = ports[j].ready_thread;// critical zone
						sem_post(&semaphore);
						if(ready == FALSE){
							goto new_ready;
						}
						break;
					}
				}
			} else continue;
		}

			
			if (i>=0)
				pmsg->port_server_thread = htons(STARTPORT+i);
		}


		errno=0; 
		ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, fromlen);

		/*sem_wait(&semaphore);
		ports[i].got_client = TRUE; // critical zone
		sem_post(&semaphore);*/
		//sleep(1);
		if (ret<0) {
			perror("sendto");
			goto cleanup;
		}
		else
			printf("SENDED: %d\n", ntohs(*(int*)msg));
	}
	all_global_free();

	if (tinfo != NULL) {
		free(tinfo);
		tinfo = NULL;
	}

	sem_destroy(&semaphore);

	close(sock);

	return 0;

cleanup:
	
	all_global_free();
	
	if (tinfo != NULL) {
		free(tinfo);
		tinfo = NULL;
	}

	sem_destroy(&semaphore);

	close(sock);

	return -1;


}