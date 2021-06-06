CFLAGS = -Iheaders
vpath %.h headers
vpath %.c src
my_kalc: my_kalc.c my_kalc.h -ldl
	gcc $(CFLAGS) src/my_kalc.c -o my_kalc -ldl -lm
	./my_kalc
	rm -rf my_kalc plugin/*