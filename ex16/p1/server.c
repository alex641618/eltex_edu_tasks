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

#define PERMISSIONS_QUEUE 0660
#define BUFFER_SIZE 100

int main(int argc, char* argv[]) {
	mqd_t queue;
	char buffer[BUFFER_SIZE] = "hi", buffer0[BUFFER_SIZE];
	int ret = 0;
	unsigned int pfc = 1, pfs = 2; 
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 2;
	attr.mq_msgsize = 100;
	attr.mq_curmsgs = 0;

	queue = mq_open("/message1", O_RDWR | O_CREAT, PERMISSIONS_QUEUE, &attr);

	if (queue < 0) { 
		perror("error");
		return -1;
	}

	errno=0;
	printf("start client\n");
	ret = mq_receive(queue, buffer0, BUFFER_SIZE, &pfs);

	if (ret<0) {
		perror("error");
		printf("ret = %d\n", ret);
		mq_close(queue);
		return -1;
	}
	printf("client: %s\n", buffer0);

	errno=0;
	ret = mq_send(queue, buffer, strlen(buffer)+1, pfc);
	if (ret!=0) {
		perror("error");
		printf("ret = %d\n", ret);
		mq_close(queue);
		return -1;
	}
	printf("sended in messege queue: %s\n", buffer);

	mq_close(queue);
	mq_unlink("/message1");
	return 0;
}