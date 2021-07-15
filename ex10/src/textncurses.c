#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <malloc.h>
#include <string.h>

#include <sys/ioctl.h>

int flag=0;

void sig_winch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

char *get_string(int *len) 
{
	*len = 0;
	int fl=0, x1=0, y1=0, ymax=0, xmax=0, i=0;
	int capacity = 1;
	char *s = (char*) malloc(capacity * sizeof(char)); 
	int c;
	keypad(stdscr, 1);
	do{ 	
		c = wgetch(stdscr); 
		//define_key("KEY_LEFT", 0x104);
			switch(c) { 
			case KEY_BACKSPACE:
				if (fl==1) {
					move(x1-1, ymax-1);
					i--;
					while (inch() == ' ') {
					getyx(stdscr, x1, y1);
					move(x1, y1-1);
					}
					move(x1, y1);
					getyx(stdscr, x1, y1);
					fl=0;
				}
				getmaxyx(stdscr, xmax, ymax);
				getyx(stdscr, x1, y1);
				if (y1 == 0 && x1 != 0) {
					wdelch(stdscr);
					fl=1;
				}
				//else if (y1==0 && x1==0)
				//	continue;
				wdelch(stdscr);
				//refresh();
				//capacity--;
				//s = (char*) realloc(s, capacity * sizeof(char));
				//s[*len-1] = '\0';
				continue;
			case KEY_LEFT:
				x1=0; y1=0;
				getyx(stdscr, x1, y1);
				move(x1, y1-1);
				continue;
			case KEY_RIGHT: 
				x1=0; y1=0;
				getyx(stdscr, x1, y1);
				move(x1, y1+1);
				continue;
			case KEY_UP:
				x1=0; y1=0;
				getyx(stdscr, x1, y1);
				move(x1-1, y1);
			case KEY_DOWN:
				x1=0; y1=0;
				getyx(stdscr, x1, y1);
				move(x1+1, y1);
			case '\n':
				i++;
				move (i, 0);
				continue;
			}
		s[(*len)++] = (char) c;
		if (*len >= capacity)
		{ 
			capacity++; 
			s = (char*) realloc(s, capacity * sizeof(char)); 
		}
	} while (1);
	//s[*len] = '\0'; 
	return s;
}

int open_and_write(char *filename) {
	FILE *pf, *pfw;
	int symb, counter=0;
		int x, y;
	int len, preveof=0;
	pf=fopen(filename, "a+");
	//printf("kl\n");
	if (pf==NULL){
		printf("error cf");
		return -1;
	}
	int i=0;
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
			i++;
			continue;
		}
		preveof=0;
		//printf("%c", symb);
		counter++;
		addch(symb);
		//wechochar(curscr, symb);
	} while (symb!= EOF);
	move(x, counter);
	while(flag == 0) {
		char *data = get_string(&len);
		//write(fileno(pf), data, strlen(data));
		i++;
		move (i, 0);
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