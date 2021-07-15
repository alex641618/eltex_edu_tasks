#include "signum.h"

//int flag=0;
int ncount=0;
int X=0, Y=0;
int Xmax=0, Ymax=0;

void sig_winch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

void modify()
{
	int i=0, capacity = 1, up=0, fl=0;
	//char *s = (char*) malloc(capacity * sizeof(char)); 
	int c;
	keypad(stdscr, 1);
	do{ 	
		c = wgetch(stdscr); 
			switch(c) { 
			case KEY_BACKSPACE:
				if (fl==1){
					handle_backspace(0);
					getyx(stdscr, X, Y);
					move(X-1, Ymax-1);
					while (inch() == ' ') {
						getyx(stdscr, X, Y);
						move(X, Y-1);
						if (Y==0) {
							move(X, 0);
							goto cont;
						}
					}
					break;
				}
				up = handle_backspace(up);
				if (up==2) {
					getyx(stdscr, X, Y);
					move(X-1, Ymax-1);
					while (inch() == ' ') {
						getyx(stdscr, X, Y);
						move(X, Y-1);
						if (Y==0) {
							move(X, 0);
							goto cont;
						}
					}
					getyx(stdscr, X, Y);
					move(X, Y+1);
cont:
					up=0;
					fl=0;
					i--;
				}
				if (up==1)
					up++;
				continue;
			case KEY_LEFT:
			//	handle_left();
				continue;
			case KEY_RIGHT: 
			//	handle_right();
				continue;
			case KEY_UP:
			//	handle_up();
				continue;
			case KEY_DOWN:
			//	handle_down();	
			case '\n':
				i++;
				move (i, 0);
				fl=1;
				continue;
			default: 
				fl=0;
			}
		//s[(*len)++] = (char) c;
		//if (*len >= capacity)
		//{ 
	//		capacity++; 
	//		s = (char*) realloc(s, capacity * sizeof(char)); 
	//	}
	} while (1);
	//s[*len] = '\0'; 
	//return s;
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
		symb = fgetc(pf);
		if (symb == EOF) {
			if (preveof==1)
				counter=0;
			getyx(stdscr, x, y);
			continue;
		} else if (symb == '\n') {
			counter++;
			addch(symb);
			preveof=1;
			i++;
			continue;
		}
		preveof=0;
		counter++;
		addch(symb);
	} while (symb!= EOF);
	move(x, counter);
		modify();
	fclose(pf);
	return 0;
}

int main(int argc, char* argv[]) {
	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	char *pwd = (char*) malloc(strlen(getenv("PWD"))*sizeof(char));
	pwd = getenv("PWD"); 
	char *filename = (char*) malloc((strlen(pwd)+strlen(argv[1])+1)*sizeof(char));
		filename = strcat(strcat(pwd, "/"), argv[1]);
	//free(pwd);
	pwd = NULL;
	if (argc>1) {
		pwd = getenv("PWD");
		if(open_and_write(filename)!=0)
			printf("error\n");
	}
	else {

	}
	char *s = (char*) malloc(5*sizeof(char));
	endwin();
	return 0;
}