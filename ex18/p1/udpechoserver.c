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
	char clientaddr[ADDRLENTH], ifname[IFNAMSIZ], msg[MSGLEN];
	unsigned short port;
	struct sockaddr_in client;
	struct in_addr address;
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
				printf("usage: udpechoserver -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
				return 0;
			} else {
				k++;
				printf("invalid argument\nusage: udpechoserver -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
				printf("enter -h or --help for help usege\n");
				return -1;
			}
		}

	} else {
		printf("usage: udpechoserver -a [adress] -p [port]\nif are you want use special intarface use -i [interface]\n");
		printf("enter -h or --help for help usege\n");
		return -1;
	}
	errno=0;
	int sock = socket(AF_INET, SOCK_DGRAM, 17);
	if (sock == -1) {
		perror("socket");
		return -1;
	}
	client.sin_family = AF_INET;
	client.sin_port=port;
	errno=0;
	if(inet_aton(clientaddr, &client.sin_addr)==0) {
		perror("inet_aton");
		goto cleanup;
	}
	memset(msg, 0xFF, MSGLEN);
	setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)+1);
	bind(sock, (struct sockaddr*)&client, sizeof(client));
	while (1) {
		errno=0;
		int ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&client, (socklen_t *)sizeof(client));
		printf("ret=%d\n", ret);
		printf("errno=%d\n\n", errno); 
	}

	close(sock);
	return 0;

cleanup:
	
	close(sock);
	return -1;
}