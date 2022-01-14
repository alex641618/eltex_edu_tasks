#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
//#include <linux/ioctl.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ADDRLENTH 16
#define MSGLEN 24

int main (int argc, char* argv[]) {
	char servaddr[ADDRLENTH], ifname[IFNAMSIZ]={0}, msg[MSGLEN];
	unsigned short port =0;
	struct sockaddr_in server;
	struct in_addr address;
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
	int sock = socket(AF_INET, SOCK_DGRAM, 17);
	int tolen = sizeof(server);
	if (sock == -1) {
		perror("socket");
		return -1;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port=port;
	errno=0;
	if(inet_aton(servaddr, &server.sin_addr)==0) {
		perror("inet_aton");
		goto cleanup;
	}
	memset(msg, 0xFF, MSGLEN);
	if (ifname == 0)
		setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)+1);
	else 
		printf("using default interface\n");
	while (1) {
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