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
	printf("процесс1\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
	switch (ret=fork()) {
		case -1:
			printf("error\n");
			return -1;
		case 0:

			printf("процесс2\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
			switch (ret=fork()) {
				case -1:
					printf("error\n");
					return -1;
				case 0:

					printf("процесс3\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
					while (1) {
						continue;
					}
					return 0;
			}
			switch (ret=fork()) {
				case -1:
					printf("error\n");
					return -1;
				case 0:

					printf("процесс4\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
					while (1) {
						continue;
					}
					return 0;
			}	
			while (1) {
				continue;
			}
			return 0;
	}
	sleep(1);
	switch (ret=fork()) {
		case -1:
			printf("error\n");
			return -1;
		case 0:

			printf("процесс5\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
			switch (ret=fork()) {
				case -1:
					printf("error\n");
					return -1;
				case 0:

					printf("процесс6\nМой pid: %d\nМой ppid: %d\n", getpid(), getppid());
					while (1) {
						continue;
					}
					return 0;
			}	
			while (1){
				continue;
			}
			return 0;
	}
	sleep(1);
	daemon(0,0);
	while (1){
		continue;
	}
	return 0;
}