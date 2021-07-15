#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <malloc.h>
#include <string.h>

#include <sys/ioctl.h>

void sig_winch(int);
void modify();
int open_and_write(char*);

int handle_backspace(); // возвращает количество стертых символов
void handle_left();
void handle_right();
void handle_up();
void handle_down();
