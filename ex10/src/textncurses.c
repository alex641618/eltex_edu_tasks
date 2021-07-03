#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <malloc.h>
#include <string.h>

#include <sys/ioctl.h>

void sig_winch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

char *get_string(int *len) 
{
	*len = 0;
	int capacity = 1;
	char *s = (char*) malloc(capacity * sizeof(char)); 
	char c = getch();
	wechochar(curscr, c);
	while (c != '\n') 
	{ 
		
		
		s[(*len)++] = c;
		if (*len >= capacity)
		{ 
			capacity++; 
			s = (char*) realloc(s, capacity * sizeof(char)); 
		}
		//printf("dds");
		c = getch();
		//printf("ddd");
		wechochar(curscr, c);
	} 
	s[*len] = '\0'; 
	return s;
}

int open_and_write(char *filename) {
	FILE *pf, *pfw;
	int symb, counter=0;
		int x, y;
	int len, preveof=0;
	pf=fopen(filename, "r");
	pfw=fopen(filename, "a");
	//printf("kl\n");
	if (pf==NULL){
		printf("error cf");
		return -1;
	}
	int i=1;
	do {
		//fread(&symb, sizeof(char), 1, pf);
		symb = fgetc(pf);
		//printw("%d %d", symb, i);
		if (symb == EOF) {
			if (preveof==1)
				counter=0;
			getyx(stdscr, x, y);
			//printf("x=%d y=%d\n", x, y);
			continue;
		} else if (symb == '\n') {
			counter++;
			addch(symb);
			preveof=1;
			continue;
		}
		preveof=0;
		//printf("%c", symb);
		counter++;
		addch(symb);
		//wechochar(curscr, symb);
	} while (symb!= EOF);
	move(x, counter);
	while(1) {
	char *data = get_string(&len);
	fwrite((void *) data, sizeof(char), strlen(data), pfw);
	move (i, 0);
	i++;
	}

	fclose(pf);
	return 0;
}

int main(int argc, char* argv[]) {
	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	//curs_set(0);
	char *pwd = (char*) malloc(strlen(getenv("PWD"))*sizeof(char));
	pwd = getenv("PWD"); 
	char *filename = (char*) malloc((strlen(pwd)+strlen(argv[1])+1)*sizeof(char));
		filename = strcat(strcat(pwd, "/"), argv[1]);
	if (argc>1) {
		pwd = getenv("PWD");
		if(open_and_write(filename)!=0)
			printf("error\n");
	}
	else {

	}
	//wgetnstr(curscr, data, s);
	char *s = (char*) malloc(5*sizeof(char));
	//FILE *pf;
	//pf=fopen(filename, "w");
	//fgets(s, 5*sizeof(char), filename);
	//fclose(pf);
	endwin();
	return 0;
}