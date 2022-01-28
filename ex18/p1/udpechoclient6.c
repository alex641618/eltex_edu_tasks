#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
//#include <linux/ioctl.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ADDRLENTH 46
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


	char servaddr[ADDRLENTH], ifname[IFNAMSIZ]={0}, msg[MSGLEN];
	unsigned short port =0;
	struct sockaddr_in6 server;
	if (argc > 1) {
		int k=1;
		while (k<argc) {
			if (strcmp(argv[k], "-a") == 0) {
				strcpy(servaddr, argv[k+1]);
				printf("servaddr = %s\n", servaddr);
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
				printf("usage: udpechoclient -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
				return 0;
			} else {
				k++;
				printf("invalid argument\nusage: udpechoclient -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
				printf("enter -h or --help for help usege\n");
				return -1;
			}
		}

	} else {
		printf("usage: udpechoclient -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
		printf("enter -h or --help for help usege\n");
		return -1;
	}

	errno=0;
	int sock = socket(AF_INET6, SOCK_DGRAM, 17);
	int tolen = sizeof(server);
	if (sock == -1) {
		perror("socket");
		return -1;
	}
	bzero(&server, sizeof(server));
	server.sin6_family = AF_INET6;
	server.sin6_port=htons(port);
	errno=0;
	if(inet_pton(AF_INET6, servaddr, &server.sin6_addr)==0) {
		perror("inet_pton");
		goto cleanup;
	}
	memset(msg, 0xFF, MSGLEN);
	if (ifname)
		setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)+1);
	else 
		printf("using default interface\n");
	while (cexit!=1) {
		errno=0;
		int ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
		if (errno!=0) {
			printf("ret=%d\n", ret);
			// printf("errno=%d\n\n", errno);
			perror("error");
			goto cleanup;
		}
		printf("MSGLEN = %d\n", MSGLEN);
		sleep(2);
	}

	close(sock);
	return 0;

cleanup:
	
	close(sock);
	return -1;

}