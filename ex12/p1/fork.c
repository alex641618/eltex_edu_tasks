#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

	pid_t ret;

	switch (ret=fork()) {
		case -1:
			printf("error");
			return -1;
		case 0:
			printf("Я процесс-потомок\nМой pid: %d\nМой ppid: %d\nkill me!\n", getpid(), getppid());
			while (1){
				continue;
			}
		default:
			printf("Я процесс-предок\nМой pid: %d\nМой ppid: %d\nPid моего процесса-потомока: %d\nKILL ME!\n", getpid(), getppid(), ret);
			sleep(1);
			daemon(0, 0);
			while (1){
				continue;
			}
}
}