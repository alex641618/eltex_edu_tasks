#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>   

#define BUFFER_SIZE 100

int main(int argc, char* argv[]) {
	mqd_t queue;
	char buffer[BUFFER_SIZE] = "hello";
	int ret=0;
	unsigned int pfc = 1, pfs = 2;

	errno=0;
	queue = mq_open("/message1", O_RDWR);
	if (queue < 0) { 
		perror("error");
		return -1;
	}

	errno=0;
	ret = mq_send(queue, buffer, strlen(buffer)+1, pfs);
	if (ret!=0) {
		perror("error");
		printf("ret = %d\n", ret);
		mq_close(queue);
		return -1;
	}
	printf("sended in messege queue: %s\n", buffer);

	//printf("start server\n");
	errno=0;
	ret = mq_receive(queue, buffer, BUFFER_SIZE, &pfc);
	if (ret<0) {
		perror("error");
		printf("ret = %d", ret);
		mq_close(queue);
		return -1;
	}

	printf("server: %s\n", buffer);

	mq_close(queue);
	return 0;
}