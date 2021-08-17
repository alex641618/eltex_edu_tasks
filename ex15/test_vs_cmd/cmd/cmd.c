//#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include <signal.h>
#include <malloc.h>
#include <curses.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>

struct COMMAND {
	char util[50];
	char args[50][20];
};

int main() {
	int i=0, count=0;
	char command[200], **acommand = (char **) malloc(100 * sizeof(char*));
	if (acommand == NULL) {
		printf("error1 malloc");
		return -1;
	}
	int j;

	//acommand[0] = (char*) malloc(30*sizeof(char));
	//if (acommand[0] == NULL) {
	//	printf("error2 malloc\n");
	//	return -1;
	//}
	while (strcmp(command, "exit") != 0) {
		printf("# ");
		char c = getchar();
		while (c != '\n') {
			command[i] = c;
			i++;
			c = getchar();
		}
		command[i] = '\0';
		c = '\0';

		printf("[echo cmd]	%s\n", command);
		char *ret = strtok(command, "|");
		int size = strlen(ret)+1;
		//if (ret != NULL) count++;
		while (ret != NULL) {
		//	printf("wilestart\n");
			//printf("ret = %s\n", ret);
			//printf("size = %d\n", size);
			acommand = (char**) realloc(acommand, size*sizeof(char*)); //????????????? не выделяется достаточно памяти
			if (acommand == NULL) {
				printf("error1 realloc");
				return -1;
			}
				acommand[count] = (char*) realloc(acommand[count], (strlen(ret)+1)*sizeof(char));
				if (acommand[count] == NULL) {
					printf("error2 realloc%d\n", count);
					return -1;
				}
			strcpy(acommand[count], ret);
			ret = strtok(NULL, "|");
			count++;
			if (ret != NULL) size = size + strlen(ret)+1;
		}
		size = 0;
		for (j=0; j<count; j++) {
			printf("%s\n", acommand[j]);
			//printf("%d\n", j);
		}
		//printf("whilefinish\n");
		count=0;
		i=0;
	}

	return 0;
}