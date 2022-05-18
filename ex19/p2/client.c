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

#define ADDRLENTH 16
#define MSGLEN 24

short cexit=0;

typedef enum {FALSE, TRUE} bool;

void hdl(int sig) {
	printf("exit\n");
	cexit = 1;
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


int main (int argc, char* argv[]) {

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = hdl;
	sigset_t   set; 
	sigemptyset(&set);                                                             
	sigaddset(&set, SIGINT); 
	act.sa_mask = set;
	sigaction(SIGINT, &act, 0);
	bool got_iface = FALSE;

	char servaddr[ADDRLENTH], ifname[IFNAMSIZ]={0}, msg[MSGLEN];
	unsigned short port =0;
	struct sockaddr_in server;
	struct in_addr address;
	if (argc > 1) {
		int k=1;
		while (k<argc) {
			if (strcmp(argv[k], "-a") == 0) {
				if (argv[k+1]){
					if(invalid_addr(argv[k+1])){
						printf("invalid address\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
						printf("enter -h or --help for help usege\n");
						return -1;
					} else {
						strcpy(servaddr, argv[k+1]);
						printf("servaddr = %s\n", servaddr);
						k+=2;
					}
				} else {
					printf("invalid address\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
					printf("enter -h or --help for help usege\n");
					return -1;
				}
			} else if (strcmp(argv[k], "-p") == 0) {
				if (argv[k+1]){
					errno=0;
					port = (unsigned short)strtol(argv[k+1], NULL, 0);
					if (port == 0) {
						printf("invalid port\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
						printf("enter -h or --help for help usege\n");
						return -1;
					}
					printf("port = %hu\n", port);
					k+=2;
				} else {
					printf("invalid port\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
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
							printf("invalid ifname\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
							printf("enter -h or --help for help usege\n");
							return -1;
						}
					} else {
						strcpy(ifname, argv[k+1]);
						printf("ifname = %s\n", ifname);
						got_iface = TRUE;
						k+=2;
					}
				} else {
					printf("invalid ifname\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
					printf("enter -h or --help for help usege\n");
					return -1;
				}
			} else if (strcmp(argv[k], "-h") == 0 || strcmp(argv[k], "--help") == 0) {
				printf("usage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
				return 0;
			} else {
				k++;
				printf("invalid argument\nusage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
				printf("enter -h or --help for help usege\n");
				return -1;
			}
		}

	} else {
		printf("usage: client -a [server_address] -p [server_port]\nif are you want use special intarface use -i [interface]\n");
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
	server.sin_port=htons(port);
	errno=0;
	if(inet_aton(servaddr, &server.sin_addr)==0) {
		perror("inet_aton");
		goto cleanup;
	}
	memset(msg, 0x00, MSGLEN);
	if (got_iface)
		setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)+1);
	else 
		printf("using default interface\n");

	errno=0;
	int ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
		if (errno!=0) {
			printf("ret=%d\n", ret);
			perror("error");
			goto cleanup;
		}

		errno=0;
		ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, &tolen);
		if (ret<0)
			perror("recvfrom");
		else {
			printf("recieved: %d\n", ntohs(*(int*)msg));
			port = ntohs(*(int*)msg);
			server.sin_port=htons(port);
		}

		errno=0;
		//sleep(2);
		ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
		if (errno!=0 && cexit == 0) {
			printf("ret=%d\n", ret);
			printf("%d\n", __LINE__);
			perror("error");
			goto cleanup;
		}
		printf("sended: %d\n", ntohs(*(int*)msg));

	while (cexit!=1) {
		//memset(msg, 0x00, MSGLEN);
		printf("enter your message\n");

		if(scanf("%s", msg) > 0){
			errno=0;
			ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
				if (errno!=0 && cexit == 0) {
					printf("ret=%d\n", ret);
					printf("%d\n", __LINE__);
					perror("error");
					goto cleanup;
				}
				printf("MSGLEN = %d\n", MSGLEN);
		}
		
		if (cexit == 0 ) {
			ret=recvfrom(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, &tolen);
			if (ret<0)
				perror("recvfrom");
			else {
				printf("recieved: %s\n", msg);
			}
		}
		if (strcmp(msg, "END") == 0) {
			goto cleanup;
		}
	}
	if (cexit == 1) {
		strcpy(msg, "END");
		errno=0;
		ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
		if (errno!=0) {
			printf("ret=%d\n", ret);
			perror("error");
			goto cleanup;
		}
		goto cleanup;
	}

	strcpy(msg, "END");
	errno=0;
	ret=sendto(sock, msg, MSGLEN, 0, (struct sockaddr *)&server, tolen);
	if (errno!=0) {
			printf("ret=%d\n", ret);
			perror("error");
			goto cleanup;
		}
	close(sock);
	return 0;

cleanup:
	
	close(sock);
	return -1;

}