#include "signum.h"

int handle_backspace(int flag)
{
	getmaxyx(stdscr, Xmax, Ymax);
	getyx(stdscr, X, Y);
	if (Y == 0 && X != 0 && flag==0) {
		flag++;
		wdelch(stdscr);
		return flag;
	}
	wdelch(stdscr);
	return flag;
}