my_kalc: my_kalc.c summ.c diff.c pro.c rat.c my_kalc.h
	gcc -fPIC summ.c diff.c pro.c rat.c -c
	gcc -shared -o libmy_kalc.so summ.o diff.o pro.o rat.o
	gcc my_kalc.c -o my_kalc -L. -lmy_kalc
	LD_LIBRARY_PATH=$(PWD) ./my_kalc
	rm -rf *.o libmy_kalc.so my_kalc