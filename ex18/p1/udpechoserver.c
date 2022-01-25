#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>

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

void hdl(int sig) {
	printf("exit\n");
	cexit = 1;
}


int main (int argc, char* argv[]) {
	struct sigaction act;
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
				printf("usage: udpechoserver -p [port]\nif are you want use special intarface use -i [interface]\n");
				return 0;
			} else {
				k++;
				printf("invalid argument\nusage: udpechoserver -p [port]\nif are you want use special intarface use -i [interface]\n");
				printf("enter -h or --help for help usege\n");
				return -1;
			}
		}

	} else {
		printf("usage: udpechoserver -p [port]\nif are you want use special intarface use -i [interface]\n");
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
	memset(msg, 0xFF, MSGLEN);
	errno=0;
	int ret=bind(sock, (struct sockaddr*)&server, sizeof(server));
	
	if (ret!=0){
		perror("bind");
		goto cleanup;
	}
	
	while (1) {

		if (cexit==1)
			break;

		errno=0;
		int ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, &fromlen);
		if (ret<0)
			perror("recvfrom");
		else
			printf("recieved\n");

		if (cexit==1)
			break;

		errno=0;
		ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, fromlen);

		if (ret<0)
			perror("sendto");
		else
			printf("sended\n");

	}

	close(sock);
	return 0;

cleanup:
	
	close(sock);
	return -1;
}